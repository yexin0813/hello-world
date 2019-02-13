/**
 *
 * @file osapp_gble_itf.c
 *
 * @brief Gble write and read interface Customer of Gizwits write this file, Gizwits write osapp_gble.c.
 *
 * Copyright (C) Apollo 2015-2016
 *
 */

#include "osapp_config.h"

/**
 ****************************************************************************************
 * @addtogroup App Demo
 * @ingroup RTOS App
 * @brief sample interface for gble data read and write
 * @{
 ****************************************************************************************
 */

#ifdef OSAPP_GBLE
#include "osapp_gble_itf.h"
#include "dht11.h"
#include "motor.h"
#include "rgb_led.h"
#include "log.h"

/// send data to master through notify.
extern void osapp_gattc_send_evt_cmd(uint8_t *value, uint8_t len);


/**
 * @brief This function indicate gble has received a command.User should decode the data according to the protocol.
 * @param data: pointer to the received data.
 * @param length: length is the length of data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_receive_data_ind(uint8_t *data, uint8_t length)                // gble receive a command
{
    uint8_t i;
    for(i=0;i<length;i++)
        LOG(LOG_LVL_INFO,"%x,",data[i]);

    //decode data
    if(data[0] == 'T')                              //example: decode cmd and do something
        dht_trigger_read();





    return 1;
}

/**
 * @brief encode and send data to master, long data should be cut into pieces no longer than 20 here
 * @param data: pointer to the received data.
 * @param length: length is the length of data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_send_data_cmd(uint8_t *data, uint8_t length)           //encode and send command
{
    osapp_gattc_send_evt_cmd(data, length);

    return 1;
}

/**
 * @brief callback for dht_task. When temperature and humid data available, this function will be called and send data to master.
 * @param type: message type.
 * @param value: pointer to the received data.
 * @return return 1 when decode complete and everything is expected.
 */
int32_t gble_dht_data_avaliable(uint8_t type, uint8_t *value)
{   uint8_t temp,humid;
    uint8_t str[20];

    if(type == TEMPERATURE_AVALIABLE_IND)
    {
    	temp = value[0];
		humid = value[1];
		sprintf(str,"temp=%d,humid=%d",temp,humid);
		LOG(LOG_LVL_INFO,"temp=%d,humid=%d",temp,humid);
		gble_send_data_cmd(str,20);
		return 1;
    }
    else
    {
    	return 0;
    }

}

#endif

///@}


