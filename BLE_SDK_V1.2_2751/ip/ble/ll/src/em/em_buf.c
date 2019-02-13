/**
 ****************************************************************************************
 *
 * @file em_buf.c
 *
 * @brief BLET EM buffers
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup EM_BUF
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // stack configuration

#if (BLE_EMB_PRESENT)

#include <stddef.h>          // standard definition
#include <stdint.h>          // standard integer definition
#include <stdbool.h>         // standard boolean definition
#include <string.h>          // string manipulation
#include "em_buf.h"          // ble buffer definition

#include "ke_mem.h"          // Kernel memory management

#include "em_map.h"

#include "reg_ble_em_tx_desc.h"         // EM TX_DESC BLE register
#include "reg_ble_em_rx_desc.h"         // EM RX_DESC BLE register
#include "reg_ble_em_tx_buffer_data.h"  // EM TX_BUFFER BLE register
#include "reg_ble_em_tx_buffer_cntl.h"  // EM TX_BUFFER BLE register
#include "reg_ble_em_rx_buffer.h"       // EM RX_BUFFER BLE register
#include "reg_common_em_et.h"
/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * STRUCTURE DEFINITION
 ****************************************************************************************
 */



/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// BLE EM buffer management environment
struct em_buf_env_tag em_buf_env;


/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void em_buf_init(void)
{
    int i;

    // Current RX buffer initialization
    em_buf_env.rx_current = 0;

    // Initialize the list of TX data buffers and descriptor
    co_list_init(&em_buf_env.tx_desc_free);
    co_list_init(&em_buf_env.tx_buff_free);

    // Save the address of the first TX descriptor
    em_buf_env.tx_desc = (struct em_buf_tx_desc *)(EM_BASE_ADDR +
                                                   REG_BLE_EM_TX_DESC_ADDR_GET(0));

    // Fill in the TX control and non connected descriptors lists
    for (i=0; i<(BLE_TX_DESC_CNTL+BLE_TX_DESC_ADV); i++)
    {
        // Store the index of the TX buffers
        em_buf_env.tx_desc_node[i].idx = i;
        // Initialize TX descriptor data pointer
        ble_txdataptr_set(i, REG_BLE_EM_TX_BUFFER_CNTL_ADDR_GET(i));
        ble_txdle_set(i, 0);
        //Set by default all the descriptor are acknowledged
        ble_txcntl_set(i, BLE_TXDONE_BIT);
    }

    // Fill in the TX descriptors data lists
    for (i=(BLE_TX_DESC_CNTL+BLE_TX_DESC_ADV); i<BLE_TX_DESC_CNT; i++)
    {
        // Store the index of the TX buffers
        em_buf_env.tx_desc_node[i].idx = i;
        ble_txdataptr_set((int)i, (uint16_t)0);
        ble_txdle_set(i, 0);
        //Set by default all the descriptor are acknowledged
        ble_txcntl_set(i, BLE_TXDONE_BIT);
        co_list_push_back(&em_buf_env.tx_desc_free, &em_buf_env.tx_desc_node[i].hdr);
    }

    #if (BLE_TX_BUFFER_CNT > 0)
    // Fill in the TX buffer data lists
    for (i=0; i<BLE_TX_BUFFER_CNT; i++)
    {
        // Store the index of the TX buffers
        em_buf_env.tx_buff_node[i].idx = i;
        em_buf_env.tx_buff_node[i].buf_ptr = REG_BLE_EM_TX_BUFFER_DATA_ADDR_GET(i);
        co_list_push_back(&em_buf_env.tx_buff_free, &em_buf_env.tx_buff_node[i].hdr);
    }
    #endif // (BLE_TX_BUFFER_CNT > 0)

    // Fill in the RX buffer lists
    for (i=0; i<BLE_RX_BUFFER_CNT; i++)
    {
        int j = em_buf_rx_next(i);

        // Initialize RX descriptor data pointer
        ble_rxdataptr_set(i, REG_BLE_EM_RX_BUFFER_ADDR_GET(i));

        // Link the RX descriptors together in exchange memory
        ble_rxcntl_set(i, REG_BLE_EM_RX_DESC_ADDR_GET(j));
    }
}

void em_buf_rx_free(uint8_t hdl)
{
    // Clear RX done bit from the cntl field
    ble_rxdone_setf(hdl, 0);
}

uint8_t *em_buf_rx_buff_addr_get(uint16_t rx_hdl)
{
    return (uint8_t *)(REG_BLE_EM_RX_BUFFER_BASE_ADDR + ble_rxdataptr_get(rx_hdl));
}

uint8_t *em_buf_tx_buff_addr_get(struct em_buf_tx_desc *tx_desc)
{
    return (uint8_t *)(REG_BLE_EM_TX_BUFFER_DATA_BASE_ADDR + tx_desc->txdataptr);
}

bool em_buf_tx_free(struct em_desc_node *desc_to_be_freed)
{
    uint16_t dle_field = ble_txdle_get(desc_to_be_freed->idx);
    bool buffer_flushed = false;
    GLOBAL_INT_DISABLE();

    // If the buffer can be freed
    if(dle_field & BLE_FREEBUFF_BIT)
    {
        buffer_flushed = true;
        #if (BLE_TX_BUFFER_CNT > 0)
        // Get the idex of the buffer in the descriptor and free the buffer
        em_buf_tx_buff_free(dle_field & BLE_BUFFIDX_MASK);
        #endif //(BLE_TX_BUFFER_CNT > 0)
    }

    // Free the descriptor
    em_buf_tx_desc_free(desc_to_be_freed);
    GLOBAL_INT_RESTORE();
    return (buffer_flushed);
}

#endif //BLE_EMB_PRESENT
/// @} EM_BUF
