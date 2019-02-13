#include <stddef.h>
#include "periph_recovery.h"
#include "bx_dbg.h"

void recovery_list_add(periph_recovery_mngt_t *mngt,periph_inst_handle_t hdl)
{
    uint8_t i;
    for(i=0;i<mngt->buf_size;++i)
    {
        if(mngt->hdl[i]== hdl)
        {//already exists
            return;
        }
    }
    for(i=0;i<mngt->buf_size;++i)
    {
        if((mngt->mask & 1<< i)==0)
        {
            mngt->hdl[i] = hdl;
            mngt->mask |= 1<<i;
            break;
        }
    }
    BX_ASSERT(i<mngt->buf_size);
}

void recovery_list_remove(periph_recovery_mngt_t *mngt,periph_inst_handle_t hdl)
{
    uint8_t i;
    for(i=0;i<mngt->buf_size;++i)
    {
        if((mngt->mask & 1<< i) && mngt->hdl[i] == hdl)
        {
            mngt->hdl[i]= NULL;
            mngt->mask &= ~(1<<i);
            break;
        }
    }
    BX_ASSERT(i<mngt->buf_size);
}

void periph_recovery(periph_recovery_mngt_t *mngt)
{
    uint8_t i;
    for(i=0;i<mngt->buf_size;++i)
    {
        if(mngt->mask & 1<<i)
        {
            mngt->hdl[i]->init_func(mngt->hdl[i]);
        }
    }
}
