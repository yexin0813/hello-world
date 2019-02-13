/** @addtogroup IIC
 *  @ingroup PERIPHERAL
 *  @brief IIC driver
 *  @{
 */
#ifndef APP_IIC_H__
#define APP_IIC_H__
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "iic_integration.h"
#include "_reg_base_addr.h"
#include "reg_iic.h"
#include "bx_config.h"
#include "field_manipulate.h"


/*
 * DEFINES
 ****************************************************************************************
 */
/// DMA transfer buffer size
#define APP_IIC_DMA_TXRX_BUFFER     32
/// IIC FIFO size (const) , user don't care
#define APP_IIC_RX_TX_FIFO_DEPTH    16


/*
 * ENUMERATIONS
 ****************************************************************************************
 */
/// IIC Interrupt type , user don't care
enum IIC_IRQ_ID_FIELD
{
    IIC_INTR_RX_UNDER = 0,
    IIC_INTR_RX_OVER,     // 1
    IIC_INTR_RX_FULL,
    IIC_INTR_TX_OVER,
    IIC_INTR_TX_EMPTY,
    IIC_INTR_RD_REQ,      // 5
    IIC_INTR_TX_ABORT,
    IIC_INTR_RX_DONE,
    IIC_INTR_ACTIVITY,
    IIC_INTR_STOP_DET,
    IIC_INTR_START_DET,   // 10
    IIC_INTR_GEN_CALL,
    IIC_INTR_RESTART_DET, // 12
    IIC_INTR_MAX
};

/// Internal system state , user don't care.
enum iic_sys_stat_config
{
    IIC_INIT,
    IIC_UNINIT,
    IIC_READ_START,
    IIC_READ_DONE,
    IIC_WRITE_START,
    IIC_WRITE_DONE,
};

/// IIC pin configuration
enum iic_pin_role_config
{
    /// this pin is SCL
    IIC_SCL_PIN,
    /// this pin is SDA
    IIC_SDA_PIN,
};

/// IIC working mode selection.
typedef enum 
{
    /// Working in slave mode
    IIC_SLAVE  = 0,
    /// Working in master mode
    IIC_MASTER = 1,
} app_iic_work_mode_t;

/// IIC r/w mode selection.
typedef enum 
{
    /// IIC write data to target device
    IIC_MODE_WRITE  =0,
    /// IIC read data from target device
    IIC_MODE_READ   =1,
} app_iic_rw_mode_t;


/// IIC clock speed selection.
typedef enum
{
    /// SCL clock is 100kHz
    IIC_STANDARD_MODE   = 1,
    /// SCL clock is 400kHz
    IIC_FAST_MODE       = 2,
    /// SCL clock is 1000kHz.                                                           <p>
    /// High speed has a master code before main data send                              <p>
    /// IC_HS_MADDR regisitor can change the master code , but cannot be removed .
    IIC_HIGH_SPEED_MODE = 3,
} app_iic_speed_mode_t;

/// IIC device address bit number selection.
typedef enum
{
    /// target devices address is 7 bit
    IIC_7BIT_ADDRESS  = 0,
    /// target devices address is 10 bit
    IIC_10BIT_ADDRESS = 1,
} app_iic_address_bit_num_t;

/// IIC memory address bit number selection.
typedef enum
{
    /// Simple IIC transmit without memory write.
    IIC_NO_MEMORY_ADDRESS    = 0,
    /// memory address is 8bit , range from 0x00 to 0xFF
    IIC_8BIT_MEMORY_ADDRESS  = 1,
    /// memory address is 16bit , range from 0x0000 to 0xFFFF
    IIC_16BIT_MEMORY_ADDRESS = 2,
} app_iic_mem_addr_num_t;


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/// IIC TX RX Channel
struct iic_channel
{
    /// transmit size
    uint32_t  size;
    /// buffer pointer
    uint8_t  *bufptr;
    /// dma buffer
    uint32_t dma_buffer[APP_IIC_DMA_TXRX_BUFFER + 2];
    /// remain of transmie size
    uint8_t  remain;
    /// call back function pointer
    void (*callback) (void*,uint8_t);
    /// call back function parameter
    void *dummy;
};

/// IIC environment structure
struct iic_env_tag
{
    /// tx channel
    struct iic_channel tx;
    /// rx channel
    struct iic_channel rx;
    /// iic read/write mode select
    app_iic_rw_mode_t  rw_mode;
    /// error detect
    bool errordetect;
};

/**@brief IIC communication structure holding configuration settings for the peripheral. */
typedef struct
{
    app_iic_address_bit_num_t   dev_addr_bit_num;   /**< device address length. */
    app_iic_mem_addr_num_t      mem_addr_bit_num;   /**< memory address length. */
    app_iic_work_mode_t         work_mode;          /**< work mode is slave or master.  */
    app_iic_speed_mode_t        speed_mode;         /**< SCL clock speed switch.  */
    uint16_t                    slave_address;      /**< Slave Address only in SLAVE MODE  */

    uint8_t sda_pin;                                /**< SDA pin number, range from 2 to 23.  */
    uint8_t scl_pin;                                /**< SCL pin number, range from 2 to 23.  */
    uint8_t enable_pull_up;                         /**< if SDA and SCL enable internal pull-up resistor.  */
    uint8_t use_dma;                                /**< use DMA = 1 , donnot use DMA = 0 */
}app_iic_comm_params_t;

/**@brief IIC instance structure */
typedef struct
{
    //public:
    periph_inst_t           inst;       /**< Peripheral common instance for IIC.  */
    app_iic_comm_params_t   param;      /**< IIC initialization parameters.  */
    //private:
    struct iic_env_tag      env;        /**< IIC initialization parameters.  */
    reg_iic_t               *reg;       /**< IIC register base address.  */
    uint8_t                 idx;        /**< IIC instance index.  */
}app_iic_inst_t;



/*
 * MACROS
 ****************************************************************************************
 */
/**@brief macro for IIC instantiation. */
#define IIC_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_iic_init,},\
            .idx = (id),\
            .reg = (reg_iic_t *)REG_IIC##id##_BASE,\
        }



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


void app_iic_isr(app_iic_inst_t *inst);



/**
 * @brief Initialize APP_IIC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the IIC instance
 */
void app_iic_init  (periph_inst_handle_t hdl);


/**
 * @brief De initialize APP_IIC module
 * @param[in] hdl the peripheral common instance handler(pointer) for the IIC instance
 */
void app_iic_uninit(periph_inst_handle_t hdl);


/**
 * @brief To read bytes from IIC
 * @param[in] hdl the peripheral common instance handler(pointer) for the IIC instance
 * @param[out] bufptr buffer where data received from IIC will be put
 * @param[in] size data length that the IIC will read
 * @param[in] device_address the address of hardware device
 * @param[in] mem_address the address of memory location to read
 * @param[in] callback function pointer that will be called when all data has been received
 * @param[in] dummy parameter that will be pass to callback function
 */
void app_iic_read     (periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, uint16_t device_address, uint16_t mem_address , void (*callback) (void*,uint8_t),void* dummy);


/**
 * @brief To write bytes to IIC
 * @param[in] hdl the peripheral common instance handler(pointer) for the IIC instance
 * @param[in] bufptr buffer where data sent to IIC is located
 * @param[in] size data length that the IIC will write
 * @param[in] device_address the address of hardware device
 * @param[in] mem_address the address of memory location to write
 * @param[in] callback function pointer that will be called when all data has been sent
 * @param[in] dummy parameter that will be pass to callback function
 */
void app_iic_write   (periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, uint16_t device_address, uint16_t mem_address , void (*callback) (void*,uint8_t),void* dummy);



/** @}*/
#endif
