/** @addtogroup SPI
 *    @ingroup PERIPHERAL
 *  @{
 */

#ifndef APP_SPIM_H_
#define APP_SPIM_H_
#include <stdint.h>
#include <stdbool.h>
#include "periph_common.h"
#include "reg_ssi.h"
#include "_reg_base_addr.h"
#include "spi_integration.h"
#include "dw_apb_ssi_typedef.h"
enum spim_sys_stat_config
{
    SPI_INIT,
    SPI_UNINIT,
    SPI_TX_START,
    SPI_TX_DONE,
    SPI_RX_START,
    SPI_RX_DONE,
    SPI_TXRX_START,
    SPI_TXRX_DONE,
};

enum spim_pin_role_config
{
    SPIS_CLK_PIN,
    SPIM_CLK_PIN,
    MOSI_PIN,
    MISO_PIN,
    SPIS_CS_PIN,
    SPIM_CS0_PIN,
    SPIM_CS1_PIN,
    SPIM_CS2_PIN,
};
    
/**@brief SPI initialization parameters structure */
typedef struct
{

    uint16_t dfs_32:5, /**< Data Frame Size.  */
            cpol:1,         /**< Serial Clock Polarity.  */
            cph:1,          /**< Serial Clock Phase.  */
            rx_dma:1,   /**< Use DMA for RX.  */
            tx_dma:1;    /**< Use DMA for TX.  */
    uint8_t clk_pin_no; /**< CLK pin number. */
    uint8_t mosi_pin_no; /**< MOSI pin number. */
    uint8_t miso_pin_no; /**< MISO pin number. */
    union
    {
        struct
        {
            uint16_t clk_div; /**< Serial Clock Divisor. */
            uint8_t rxsample_dly; /**< RX Sample Delay. */
            uint8_t cs_pin_no[SPIM_CS_NUM]; /**< CS pins number */
            uint8_t hw_cs_mask; /**< Valid Hardware CS pin mask. */
        }master;                            /**< SPI Master Parameters. */
        struct
        {
            uint8_t cs_pin_no;     /**< CS pin number. */
        }slave;                               /**< SPI Slave Parameters. */
    }u;
}app_spi_init_param_t;


typedef struct
{
    uint8_t *tx_data;
    uint8_t *rx_data;
    uint16_t tx_size;
    uint16_t rx_size;
    void (*callback)(void *);
    void *callback_param;
    uint8_t cs_sel_mask;
}spi_transfer_env_t;

/**@brief SPI instance structure */
typedef struct
{
    periph_inst_t inst; /**< Peripheral common instance for SPI.  */
    app_spi_init_param_t param; /**< SPI initialization parameters.  */
    spi_transfer_env_t env; /**< SPI operating environment.  */
    reg_ssi_t *reg; /**< SPI register base address.  */
    uint8_t idx; /**< SPI instance index.  */
    bool slave; /**< if an instance for SPI Slave.  */
}app_spi_inst_t;

/**@brief macro for SPI Master instantiation. */
#define SPIM_INSTANCE(id)  \
        {\
            .inst = {.init_func = app_spi_init,},\
            .idx = (id),\
            .reg = (reg_ssi_t *)REG_SPIM_##id##_BASE,\
            .slave = false,\
        }

/**@brief macro for SPI Slave instantiation. */
#define SPIS_INSTANCE(id) \
        {\
            .inst = {.init_func = app_spi_init,},\
            .reg = (reg_ssi_t *)REG_SPIS##id##_BASE,\
            .slave = true,\
        }

/**
 * @brief Initialize APP_SPI module
 * @param[in] hdl the peripheral common instance handler(pointer) for the SPI instance
 */
void app_spi_init(periph_inst_handle_t hdl);

/**
 * @brief De initialize APP_SPI module
 * @param[in] hdl the peripheral common instance handler(pointer) for the SPI instance
 */
void app_spi_uninit(periph_inst_handle_t hdl);

/**
 * @brief SPI TX
 * @param[in] hdl the peripheral common instance handler(pointer) for the SPI instance
 * @param[in] cs_sel_mask Hardware CS pins select mask for the transfer (invalid for SPI Slave or Software CS)
 * @param[in] data buffer where data sent to SPI is located
 * @param[in] size data length that the SPI will send
 * @param[in] callback function pointer that will be called when all data has been sent
 * @param[in] cb_param parameter that will be pass to callback function
 */
void app_spi_transmit(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *data,uint16_t size,void (*cb)(void *),void *cb_param);

/**
 * @brief SPI RX
 * @param[in] hdl the peripheral common instance handler(pointer) for the SPI instance
 * @param[in] cs_sel_mask Hardware CS pins select mask for the transfer (invalid for SPI Slave or Software CS)
 * @param[out] data buffer where data received from SPI will be put
 * @param[in] size data length that the SPI will receive
 * @param[in] callback function pointer that will be called when all data has been received
 * @param[in] cb_param parameter that will be pass to callback function
 */
void app_spi_receive(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *data,uint16_t size,void (*cb)(void *),void *cb_param);

/**
 * @brief SPI TX_and_RX
 * @param[in] hdl the peripheral common instance handler(pointer) for the SPI instance
 * @param[in] cs_sel_mask Hardware CS pins select mask for the transfer (invalid for SPI Slave or Software CS)
 * @param[in] tx_data buffer where data sent to SPI is located
 * @param[out] rx_data buffer where data received from SPI will be put
 * @param[in] size data length that the SPI will send and receive
 * @param[in] callback function pointer that will be called when all data has been sent and received
 * @param[in] cb_param parameter that will be pass to callback function
 */
void app_spi_transmit_receive(periph_inst_handle_t hdl,uint8_t cs_sel_mask,void *tx_data,void *rx_data,uint16_t size,void (*cb)(void *),void *cb_param);

void app_spi_isr(app_spi_inst_t * inst);
#endif
/** @}*/

