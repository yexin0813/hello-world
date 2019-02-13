/** @addtogroup UART
 *    @ingroup PERIPHERAL
 *  @{
 */
#ifndef APP_UART_H__
#define APP_UART_H__
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "uart_integration.h"
#include "_reg_base_addr.h"
#include "reg_uart.h"
#include "periph_error.h"
#include "bx_config.h"



enum uart_divisor_settings
{
    UART_BAUDRATE_1200_DIVISOR   = 768,
    UART_BAUDRATE_2400_DIVISOR   = 384,
    UART_BAUDRATE_4800_DIVISOR   = 192,
    #if (defined(CFG_ON_CHIP)&&(CFG_ON_CHIP==1))
    UART_BAUDRATE_9600_DIVISOR   = 96,
    #else
    UART_BAUDRATE_9600_DIVISOR   = 208,
    #endif
    UART_BAUDRATE_14400_DIVISOR  = 64,
    UART_BAUDRATE_19200_DIVISOR  = 48,
    UART_BAUDRATE_28800_DIVISOR = 32,
    UART_BAUDRATE_38400_DIVISOR = 24,
    UART_BAUDRATE_57600_DIVISOR = 16,
    UART_BAUDRATE_76800_DIVISOR = 12,
    UART_BAUDRATE_115200_DIVISOR = 8,
    UART_BAUDRATE_230400_DIVISOR = 4,
    UART_BAUDRATE_250000_DIVISOR =  8 ,
    UART_BAUDRATE_460800_DIVISOR = 2,
    UART_BAUDRATE_921600_DIVISOR = 1,
    UART_BAUDRATE_1000000_DIVISOR = 2,
    UART_BAUDRATE_2000000_DIVISOR = 1,
};

#define UART_BUADRATE_ENUM_GEN(baud) (UART_BAUDRATE_##baud##_CLK_SRC<<10|UART_BAUDRATE_##baud##_DIVISOR)

/**@brief UART baud rate enum */
typedef enum
{
    UART_BAUDRATE_1200   = UART_BUADRATE_ENUM_GEN(1200),
    UART_BAUDRATE_2400   = UART_BUADRATE_ENUM_GEN(2400),
    UART_BAUDRATE_4800   = UART_BUADRATE_ENUM_GEN(4800),
    UART_BAUDRATE_9600   = UART_BUADRATE_ENUM_GEN(9600),
    UART_BAUDRATE_14400 = UART_BUADRATE_ENUM_GEN(14400),
    UART_BAUDRATE_19200 = UART_BUADRATE_ENUM_GEN(19200),
    UART_BAUDRATE_28800 = UART_BUADRATE_ENUM_GEN(28800),
    UART_BAUDRATE_38400  = UART_BUADRATE_ENUM_GEN(38400),
    UART_BAUDRATE_57600 = UART_BUADRATE_ENUM_GEN(57600),
    UART_BAUDRATE_76800  = UART_BUADRATE_ENUM_GEN(76800),
    UART_BAUDRATE_115200 = UART_BUADRATE_ENUM_GEN(115200),
    UART_BAUDRATE_230400 = UART_BUADRATE_ENUM_GEN(230400),
    UART_BAUDRATE_250000 = UART_BUADRATE_ENUM_GEN(250000),
    UART_BAUDRATE_460800 = UART_BUADRATE_ENUM_GEN(460800),
    UART_BAUDRATE_921600 = UART_BUADRATE_ENUM_GEN(921600),
    UART_BAUDRATE_1000000= UART_BUADRATE_ENUM_GEN(1000000),
    UART_BAUDRATE_2000000= UART_BUADRATE_ENUM_GEN(2000000),
}app_uart_baudrate_t;

enum uart_sys_stat_config
{
    UART_INIT,
    UART_UNINIT,
    UART_READ_START,
    UART_READ_CANCEL,
    UART_READ_DONE,
    UART_WRITE_START,
    UART_WRITE_DONE,
};

enum uart_pin_role_config
{
    UART_TXD_PIN,
    UART_RXD_PIN,
    UART_CTS_PIN,
    UART_RTS_PIN,
};


/// UART TX RX Channel
struct uart_txrxchannel
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
struct uart_env_tag
{
    /// tx channel
    struct uart_txrxchannel tx;
    /// rx channel
    struct uart_txrxchannel rx;
    /// dma channel for tx
    uint8_t dma_ch_tx:3,
                   dma_en_tx:1;
    /// dma channel for rx
    uint8_t dma_ch_rx:3,
                    dma_en_rx:1;
    /// error detect
    bool errordetect;
};

/**@brief UART initialization parameters structure */
typedef struct
{
    app_uart_baudrate_t baud_rate;    /**< Baud rate.  */
    uint8_t rx_pin_no;    /**< RX pin number. */
    uint8_t tx_pin_no;    /**< TX pin number. */
    uint8_t rts_pin_no;   /**< RTS pin number, only used if flow control is enabled. */
    uint8_t cts_pin_no;   /**< CTS pin number, only used if flow control is enabled. */
    uint8_t parity_en : 1, /**< Parity Enable. */
                even_parity : 1,/**< Even Parity if 1, else Odd Parity. */
                flow_control_en : 1, /**< Flow Control Enable. */
                auto_flow_control : 1, /**< Auto Flow Control if 1, else Manual Control. */
                rts_stat:1, /**< RTS Low if 1 after initialized, else High. */
                tx_dma: 1, /**< Default DMA Setting for TX. */
                rx_dma: 1; /**< Default DMA Setting for RX . */
}app_uart_comm_params_t;

/**@brief UART instance structure */
typedef struct
{
    //public:
    periph_inst_t inst; /**< Peripheral common instance for UART.  */
    app_uart_comm_params_t param; /**< UART initialization parameters.  */
    //private:
    struct uart_env_tag env; /**< UART operating environment.  */
    reg_uart_t *reg; /**< UART register base address.  */
    uint8_t idx; /**< UART instance index.  */
}app_uart_inst_t;

/**@brief macro for UART instantiation. */
#define UART_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_uart_init,},\
            .idx = (id),\
            .reg = (reg_uart_t *)REG_UART##id##_BASE,\
        }

/**
 * @brief Initialize APP_UART module
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 */
void app_uart_init(periph_inst_handle_t hdl);

/**
 * @brief De initialize APP_UART module
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 */
void app_uart_uninit(periph_inst_handle_t hdl);

/**
 * @brief To read bytes from UART
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 * @param[out] bufptr buffer where data received from UART will be put
 * @param[in] size data length that the UART will read
 * @param[in] callback function pointer that will be called when all data has been received
 * @param[in] dummy parameter that will be pass to callback function
 * @return error code 
 */
periph_err_t app_uart_read(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t),void* dummy);

/**
 * @brief Cancel uart read operation
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 * @param[out] remaining_length remaining length of data that hasn't been read from UART
 * @return error code
 */
periph_err_t app_uart_read_cancel(periph_inst_handle_t hdl,uint32_t *remaining_length);

/**
 * @brief To write bytes to UART
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 * @param[in] bufptr buffer where data sent to UART is located
 * @param[in] size data length that the UART will write
 * @param[in] callback function pointer that will be called when all data has been sent
 * @param[in] dummy parameter that will be pass to callback function
 * @return error code
 */
periph_err_t app_uart_write(periph_inst_handle_t hdl,uint8_t *bufptr, uint32_t size, void (*callback) (void*,uint8_t),void* dummy);

/**
 * @brief Manually assert RTS line
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 */
void app_uart_flow_on(periph_inst_handle_t hdl);

/**
 * @brief Manually de assert RTS line
 * @param[in] hdl the peripheral common instance handler(pointer) for the UART instance
 * @return true if RTS line is successfully de asserted else false
 */
bool app_uart_flow_off(periph_inst_handle_t hdl);


void app_uart_isr(app_uart_inst_t *inst);

#endif
/** @}*/


