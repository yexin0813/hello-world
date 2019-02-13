#define __RAM_CODE__
#include "arch.h"

#include "Hbeacon.h"
#include "lld_evt.h"
#include "ea.h"
#include "llm.h"

#include "co_bt.h"

struct Hbeacon_data
{
    ///Hbeacon data length
    uint8_t        data_len;
    ///Hbeacon data - maximum 31 bytes
    uint8_t        data[ADV_DATA_LEN];
};

struct Hbeacon_param
{
	uint16_t adv_evt_counter;
	uint8_t  Hbeacon1_enable;
	uint8_t  Hbeacon2_enable;
	uint8_t  Hbeacon1_period;
	uint8_t  Hbeacon2_period;
	uint16_t adv_interval_backup;
	uint8_t adv_type_backup;
	struct Hbeacon_data adv_data_backup;
	struct Hbeacon_data Hbeacon1_data;
	uint8_t Hbeacon1_type;
	struct Hbeacon_data Hbeacon2_data;
	uint8_t Hbeacon2_type;
};

struct Hbeacon_param Hbeacon_env;
#define TEMP_BUFFER1 	   "\xDE\xAD\xBE\xEF\xDE\xAD\xBE\xEF"
#define TEMP_BUFFER1_LEN    8
#define TEMP_BUFFER2 	   "\xDE\xAD\xBE\xEF\xDE\xAD\xBE\xEF\xDE\xAD\xBE\xEF\xDE\xAD\xBE\xEF"
#define TEMP_BUFFER2_LEN    16

void Hbeacon_trap(uint32_t operation, void *p1,void *p2,void *p3)
{
	switch (operation)
    {
		case (HBEACON_START):
			//user configuration Hbeacon_period should more than 4 
			memset(&Hbeacon_env, 0, sizeof(struct Hbeacon_param));
			
			Hbeacon_env.Hbeacon1_enable = 1;
			Hbeacon_env.Hbeacon1_period = 4;
			Hbeacon_env.Hbeacon1_type = LL_ADV_NONCONN_UNDIR;
			Hbeacon_env.Hbeacon1_data.data_len= TEMP_BUFFER1_LEN;
			memcpy(&Hbeacon_env.Hbeacon1_data.data[0],TEMP_BUFFER1, TEMP_BUFFER1_LEN);

			Hbeacon_env.Hbeacon2_enable = 0;
			Hbeacon_env.Hbeacon2_period = 10;
			Hbeacon_env.Hbeacon2_type = LL_ADV_CONN_DIR;
			Hbeacon_env.Hbeacon2_data.data_len= TEMP_BUFFER2_LEN;
			memcpy(&Hbeacon_env.Hbeacon2_data.data[0],TEMP_BUFFER2, TEMP_BUFFER2_LEN);

			//user should not touch below
			struct advertising_pdu_params *adv_param = (struct advertising_pdu_params *)p1;
			if(Hbeacon_env.Hbeacon1_enable || Hbeacon_env.Hbeacon2_enable)
			{
				Hbeacon_env.adv_type_backup = ble_txtype_getf(LLM_LE_ADV_IDX);
				Hbeacon_env.adv_interval_backup = adv_param->intervalmax;
		break;
		
		case (HBEACON_STOP):
			if(Hbeacon_env.Hbeacon1_enable || Hbeacon_env.Hbeacon2_enable)
				memset(&Hbeacon_env, 0, sizeof(struct Hbeacon_param));
		break;
		
		case (HBEACON_DATA_BACKUP):
			struct hci_le_set_adv_data_cmd *param =  (struct hci_le_set_adv_data_cmd*)p1;
			if((Hbeacon_env.Hbeacon1_enable || Hbeacon_env.Hbeacon2_enable) && (param->adv_data_len > 0))
			{
				memcpy(&Hbeacon_env.adv_data_backup.data[0], &param->data.data[0], param->adv_data_len);
				Hbeacon_env.adv_data_backup.data_len = param->adv_data_len;
			}
		break;
		
		case (HBEACON_SCHED):
			struct lld_evt_tag *evt = LLD_EVT_ENV_ADDR_GET(((struct ea_elt_tag*)p1));
			Hbeacon_env.adv_evt_counter++;
			if((Hbeacon_env.Hbeacon1_enable!=0) && (Hbeacon_env.Hbeacon1_period!=0))
			{
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon1_period==(Hbeacon_env.Hbeacon1_period-1))
				{
					evt->interval = (evt->interval)/2;
				}
				//copy the beacon1 data
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon1_period==0)
				{
					em_wr((void *)&Hbeacon_env.Hbeacon1_data.data[0], ble_txdataptr_get(LLM_LE_ADV_IDX), Hbeacon_env.Hbeacon1_data.data_len);
					ble_txadvlen_setf(LLM_LE_ADV_IDX,(BD_ADDR_LEN + Hbeacon_env.Hbeacon1_data.data_len));  // Packet length
					ble_txtype_setf(LLM_LE_ADV_IDX, Hbeacon_env.Hbeacon1_type);
				}
				//copy back original adv data
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon1_period==1)
				{
					em_wr((void *)&Hbeacon_env.adv_data_backup.data[0], ble_txdataptr_get(LLM_LE_ADV_IDX), Hbeacon_env.adv_data_backup.data_len);
					ble_txadvlen_setf(LLM_LE_ADV_IDX,(BD_ADDR_LEN + Hbeacon_env.adv_data_backup.data_len));  // Packet length
					evt->interval = Hbeacon_env.adv_interval_backup;
					ble_txtype_setf(LLM_LE_ADV_IDX, Hbeacon_env.adv_type_backup);
				}
			}
					
			if((Hbeacon_env.Hbeacon2_enable!=0) && (Hbeacon_env.Hbeacon2_period!=0) &&
				(Hbeacon_env.Hbeacon2_period!=Hbeacon_env.Hbeacon1_period))
			{
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon2_period==(Hbeacon_env.Hbeacon2_period-1))
				{
					evt->interval = (evt->interval)/2;
				}
				//copy the beacon2 adv data
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon2_period==0)
				{
					em_wr((void *)&Hbeacon_env.Hbeacon2_data.data[0], ble_txdataptr_get(LLM_LE_ADV_IDX), Hbeacon_env.Hbeacon2_data.data_len);//copy the beacon2 adv data
					ble_txadvlen_setf(LLM_LE_ADV_IDX,(BD_ADDR_LEN + Hbeacon_env.Hbeacon2_data.data_len));  // Packet length
					ble_txtype_setf(LLM_LE_ADV_IDX, Hbeacon_env.Hbeacon2_type);
				}	
				//copy back original adv data
				if(Hbeacon_env.adv_evt_counter%Hbeacon_env.Hbeacon2_period==1)
				{
					em_wr((void *)&Hbeacon_env.adv_data_backup.data[0], ble_txdataptr_get(LLM_LE_ADV_IDX), Hbeacon_env.adv_data_backup.data_len);
					ble_txadvlen_setf(LLM_LE_ADV_IDX,(BD_ADDR_LEN + Hbeacon_env.adv_data_backup.data_len));  // Packet length
					evt->interval = Hbeacon_env.adv_interval_backup;
					ble_txtype_setf(LLM_LE_ADV_IDX, Hbeacon_env.adv_type_backup);
				}
			}
				break;
		}
	}
}


