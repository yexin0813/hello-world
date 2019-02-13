 
#include "os_bridge.h"
#include <stdbool.h>
#include "ble_task.h"
#include "swint.h"
#define STATE_IDLE              0
#define STATE_PENDING           1


/// UART TX RX Channel
struct virtual_port_channel
{
    /// size
    uint32_t  size;
    /// buffer pointer
    uint8_t  *bufptr;
    /// call back function pointer
    void (*callback) (void*,uint8_t);
    //call back function parameter
    void *dummy;
};



/// UART environment structure
struct virtual_port_env_tag
{
    /// tx channel
    struct virtual_port_channel tx;
    /// rx channel
    struct virtual_port_channel rx;
    /// error detect
    uint8_t errordetect;
};

struct rx_state_t
{
    uint8_t *pheader;
    uint16_t rx_index;
    uint16_t rx_total_len;
    uint8_t cmd_pending;
};

static struct virtual_port_env_tag virtual_port_env;
 static struct rx_state_t vitrual_port_rx_stat;

static void osapp_msg_rx_cb()
{
    void (*callback)(void *,uint8_t) = virtual_port_env.rx.callback;
    void *data = virtual_port_env.rx.dummy;
    virtual_port_env.rx.callback = NULL;
    virtual_port_env.rx.dummy = NULL;
    callback(data,RWIP_EIF_STATUS_OK);
    msg2blestack_from_ISR();
}

void osapp_msg_start_recv(void *pMsg, uint16_t len,uint8_t indicator)
{
    virtual_port_env.rx.bufptr[0] = indicator;
    vitrual_port_rx_stat.rx_total_len = len+1;
    vitrual_port_rx_stat.cmd_pending = STATE_PENDING;
    vitrual_port_rx_stat.pheader = (uint8_t *)pMsg;
    vitrual_port_rx_stat.rx_index = 1;
    SWINT_REQ();

}

static void virtual_port_read(uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t), void *dummy)
{
    virtual_port_env.rx.callback = callback;
    virtual_port_env.rx.size = size;
    virtual_port_env.rx.bufptr = bufptr;
    virtual_port_env.rx.dummy = dummy;
    if(vitrual_port_rx_stat.cmd_pending != STATE_IDLE)
    {
        memcpy(bufptr,vitrual_port_rx_stat.pheader+vitrual_port_rx_stat.rx_index-1,size);
        vitrual_port_rx_stat.rx_index += size;
        if(vitrual_port_rx_stat.rx_index == vitrual_port_rx_stat.rx_total_len)
        {
            vitrual_port_rx_stat.cmd_pending = STATE_IDLE;
            vPortFree(vitrual_port_rx_stat.pheader);
        }
        SWINT_REQ();
    }
}


static void virtual_port_write(uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t), void *dummy)
{
    AHI_MSGBOX_t rsp;
    uint8_t *send_buff = pvPortMalloc(sizeof(uint8_t)*(size-1));
    memcpy(send_buff,bufptr+1,size-1);                        //exclude indicator
    rsp.pmsg = (AHI_MSG_t *)send_buff;
    rsp.len = size-1;
    rsp.indicator = bufptr[0];
    ble_msg_2_app(&rsp);    
    callback(dummy,RWIP_EIF_STATUS_OK);
}

static void virtual_port_flow_on(void)
{

}


static bool virtual_port_flow_off(void)
{
    return true;
}

// Creation of virtual external interface api
static const struct rwip_eif_api virtual_port_api =
{
    virtual_port_read,
    virtual_port_write,
    virtual_port_flow_on,
    virtual_port_flow_off,
};
const struct rwip_eif_api *os_get_eif_api()
{
    return &virtual_port_api;
}

void SFT_IRQHandler(void)
{
    SWINT_CLR();
    osapp_msg_rx_cb();
}


