//=====================================================================
//    file mane:
//        common_lock_uart.c
//    description:
//        uart function head file.
//    date:
//        2018-5-9
//    author:
//        ian.lee
//    version:
//        v0.1        init version.
//    others:
//        uart module name must be: UART_1.
//=====================================================================



#include "common_lock_sensor3403.h"
#include "m_uart.h"
#include "Hardware.h"

#define   SEBSOE_TOUCH  15

//cmd state
#define CMD_BUSY                     1
#define CMD_FREE                     0


#define COMMON_LOCK_RX_BUFFER_SZIE  256

struct Timeout
{
	uint32_t start_time;
  uint32_t end_time;
	uint32_t time_over;
	uint8_t  time_over_flag;
};

struct Timeout gtTimeout;

static   uint8_t  uartTxBuff[COMMON_LOCK_RX_BUFFER_SZIE];
static   uint8_t  uartFrameBuff[100];
static   uint8_t  TempuartFrameBuff[100];
static   volatile int32_t  frameDone = 0;
static   uint8_t  cmdbusyflag = CMD_FREE;
static   uint8_t  fignum,currentcmd = 0;

void Init_SensorTouch(void);


const uint8_t CRCTable[]=
{
	0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
};


static uint8_t paraGenCRC(uint8_t *dat,uint8_t len)
{
	uint8_t crc8 = 0;
	while(len--)
	{
		crc8 = CRCTable[crc8 ^ *dat];
		dat++;
	}

	return crc8;
}

/*********************************************************************/
//    function name:
//        Com_Lock_Init_M4_Uart
//    description:
//        init uart prot for debug.
//    others:
//         
/*********************************************************************/
void Com_Lock_Init_Sensor(void)
{
	/*
		uint8_t dataconfig[13];
	  dataconfig[0] = 0xef;
		dataconfig[1] = 0x01;
		dataconfig[2] = 0xff;
		dataconfig[3] = 0xff;
		dataconfig[4] = 0xff;
		dataconfig[5] = 0xff;
		dataconfig[6] = 0x01;
		dataconfig[7] = 0x00;
		dataconfig[8] = 0x04;
		dataconfig[9] = 0x27;
		dataconfig[10] = 0x01;
	  dataconfig[11] = 0x00;
	  dataconfig[12] = paraGenCRC(dataconfig,12);
	  uartSendBuff((uint8_t *)&dataconfig,13);*/
	  //gtTimeout.time_over_flag = 0;
	  Init_SensorTouch();
	
}




static   uint32_t     frmeLen = 0;

/*********************************************************************/
//    function name:
//        Com_Lock_Pause_Uart_Frame()
//    description:
//        pause one command frame.(byte 7 = byte0~byte6 xor)
/*********************************************************************/
void Com_Lock_Pause_Uart_Frame(void)
{
    uint8_t bitChk  = 0;
	  int32_t i_temp_data;
	  uint8_t uc_temp_data; 
	  if(uartReadByte(&i_temp_data))
		{
			   uc_temp_data = i_temp_data &0xff;
				 if((frmeLen == 0) && (uc_temp_data == COMMON_LOCK_CMD_FRAME_HEAD))          // start.
				 {
						uartFrameBuff[frmeLen++] = uc_temp_data;
				 }
				 else if(frmeLen == 1)
				 {
					 if(uc_temp_data == 0x01)
					 {
						 uartFrameBuff[frmeLen++] = uc_temp_data;
					 }
					 else
					 {
						 frmeLen = 0;
					 }
				 }
				 else if((frmeLen > 1) && (frmeLen < COMMON_LOCK_CMD_FRAME_LENGTH))        // 0-9 
         {
            uartFrameBuff[frmeLen++] = uc_temp_data;
         }
				 else if((frmeLen >= COMMON_LOCK_CMD_FRAME_LENGTH) && (frmeLen < COMMON_LOCK_CMD_FRAME_LENGTH + uartFrameBuff[COMMON_LOCK_CMD_FRAME_LENGTH - 1] - 1))        // 0-9 
         {
            uartFrameBuff[frmeLen++] = uc_temp_data;
         }
				 else if(frmeLen == (COMMON_LOCK_CMD_FRAME_LENGTH + uartFrameBuff[COMMON_LOCK_CMD_FRAME_LENGTH - 1] - 1))    // one frame done.
         {
            // checke frame.
            uartFrameBuff[frmeLen++] = uc_temp_data;
            bitChk = paraGenCRC(uartFrameBuff,frmeLen -1);

            if(uartFrameBuff[frmeLen -1] == bitChk)
            {
                frameDone = 1;
                frmeLen   = 0;
            }
						else
						{
								frmeLen   = 0;
						}
        }
		}
}



static uint16_t frameSetPack(uint16_t _data_len,uint8_t* _data_buf,uint8_t _type,uint8_t * _send_buff,uint8_t *pcmd)
{

	uint8_t nwCHK = 0;
	if(_send_buff&&_data_buf)
	{
		/*make packet head*/
		_send_buff[0] = (FRAME_START>>8);
		_send_buff[1] = FRAME_START &0x00ff;
		_send_buff[2] = pcmd[0];
		_send_buff[3] = pcmd[1];
		_send_buff[4] = pcmd[2];
		_send_buff[5] = pcmd[3];
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+FRAME_CRC) & 0xff;

		memcpy(&_send_buff[FRAME_STA_LEN],_data_buf,_data_len);
		nwCHK=paraGenCRC(_send_buff,_data_len+FRAME_STA_LEN);

		_send_buff[_data_len+FRAME_STA_LEN]=nwCHK;

		return (_data_len+FRAME_CRC+FRAME_STA_LEN);
	}
	else
	{
		return 0;
	}

}

void Send_Get_Num_Cmd(void)  
{
	uint8_t  tempbuff[5];
	uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	uint16_t  sendlen;
	tempbuff[0] = COMMON_LOCK_GET_NUM;
	sendlen = frameSetPack(0x01,tempbuff,0x01,uartTxBuff,pCmd);
  uartSendBuff(uartTxBuff, sendlen);
	
	
}

void Send_Get_List_Cmd(void)
{
	uint8_t  tempbuff[5];
	uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	uint16_t  sendlen;
	tempbuff[0] = COMMON_LOCK_CMD_READ_ID_LIST;
	tempbuff[1] = 0x00;
	sendlen = frameSetPack(0x02,tempbuff,0x01,uartTxBuff,pCmd);
  uartSendBuff(uartTxBuff, sendlen);
	
}



void Send_Enroll_Cmd(uint8_t num)
{
	  uint8_t  tempbuff[5];
	  uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	  uint16_t  sendlen;
	  tempbuff[0] = COMMON_LOCK_CMD_ID_ENROLL;
	  tempbuff[1] = 0x00;
	  tempbuff[2] = num;
	  sendlen = frameSetPack(0x03,tempbuff,0x01,uartTxBuff,pCmd);
    uartSendBuff(uartTxBuff, sendlen);
	
}

void Send_Match_Cmd(void)
{
	uint8_t  tempbuff[5];
	uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	uint16_t  sendlen;
	tempbuff[0] = COMMON_LOCK_MATCH;
	tempbuff[1] = 0x00;
	tempbuff[2] = 0x00;
	tempbuff[3] = 0x00;
	tempbuff[4] = 0xff;
	sendlen = frameSetPack(0x05,tempbuff,0x01,uartTxBuff,pCmd);
  uartSendBuff(uartTxBuff, sendlen);
	
}


void Send_Delete_Cmd(void)
{
	uint8_t  tempbuff[5];
	uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	uint16_t  sendlen;
	tempbuff[0] = COMMON_LOCK_CMD_ALL_DELETE;
	sendlen = frameSetPack(0x01,tempbuff,0x01,uartTxBuff,pCmd);
  uartSendBuff(uartTxBuff, sendlen);
	
}

void Send_Init_Cmd(void)
{
	uint8_t  tempbuff[5];
	uint8_t  pCmd[4] = {0xff,0xff,0xff,0xff};
	uint16_t  sendlen;
	tempbuff[0] = COMMON_LOCK_INIT;
	sendlen = frameSetPack(0x01,tempbuff,0x01,uartTxBuff,pCmd);
  uartSendBuff(uartTxBuff, sendlen);
	
}


void GetStartTime(uint32_t timeover)
{
		gtTimeout.start_time =  systemTick();
	  gtTimeout.time_over  = timeover;
	  gtTimeout.time_over_flag = 0;
}

uint8_t GetCurrentTimeCnt(void)
{
	  uint32_t temptime;
		gtTimeout.end_time = systemTick();
	  if(gtTimeout.end_time >= gtTimeout.start_time)
		{
			 temptime =  gtTimeout.end_time - gtTimeout.start_time;
		}
		else
		{
			 temptime = 0xFFFFFFFF - gtTimeout.start_time + gtTimeout.end_time;
		}
		if(temptime >= gtTimeout.time_over)
		{
			 gtTimeout.time_over_flag = 1;
		}
		return gtTimeout.time_over_flag;
}


   
/*********************************************************************/
//    function name:
//        Com_Lock_Pause_Cmd()
//    description:
//        pause and execute command.
/*********************************************************************/
void Com_Lock_Sensor_Cmd(uint8_t *pCmd)
{        
	 switch(pCmd[9])
	 {
		 case COMMON_LOCK_CMD_TEST:
			 debugPrint("test return\n");
		   cmdbusyflag = CMD_FREE;
			 break;
		 case COMMON_LOCK_GET_NUM:
			 fignum = pCmd[12];
			 debugPrint("get num = %d \n",fignum);
		   if(currentcmd == COMMON_LOCK_CMD_ID_ENROLL)
			 {
				  currentcmd = COMMON_LOCK_CMD_ID_ENROLL_STEP2;
			 }
			 else
			 {
					cmdbusyflag = CMD_FREE;
			 }
			 break;
		 case COMMON_LOCK_MATCH:
			 if(pCmd[10] == 0x33)
			 {
				   debugPrint("3403 receive match cmd\n");
			 }
			 else if(pCmd[10] == 0x00)
			 {
				    debugPrint(" match success\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 else if(pCmd[10] == 0x09)
			 {
				    debugPrint(" match fail\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 else if(pCmd[10] == 0x26)
			 {
				    debugPrint(" match timeout\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 else if(pCmd[10] == 0x03)
			 {
				    debugPrint(" get image error\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 break;
		 case COMMON_LOCK_INIT:
			 debugPrint(" init sensor\n");
		   cmdbusyflag = CMD_FREE;
			 break;
		 case COMMON_LOCK_CMD_ALL_DELETE:
			  if(pCmd[10] == 0x00)
				{
					debugPrint(" delete success\n");
				}
				else
				{
					debugPrint(" delete fail\n");
				}
				cmdbusyflag = CMD_FREE;
			 break;
		 case COMMON_LOCK_CMD_ID_ENROLL:
			 if(pCmd[10] == 0x33)
			 {
				   debugPrint("3403 receive enroll cmd\n");
			 }
			 else if(pCmd[10] == 0x00)
			 {
				    debugPrint(" enroll success\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 else if(pCmd[10] == 0x25)
			 {
				    debugPrint(" single get success\n");
			 }
			 else if(pCmd[10] == 0x03)
			 {
				    debugPrint(" single get fail\n");
			 }
			 else if(pCmd[10] == 0x26)
			 {
				    debugPrint(" enroll timeout\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 else
			 {
				    debugPrint(" enroll error\n");
				    cmdbusyflag = CMD_FREE;
			 }
			 break;
		 case COMMON_LOCK_CMD_READ_ID_LIST:
			 uart0SendBuff(pCmd, 44);
			 cmdbusyflag = CMD_FREE;
		 default:
			 break;
	 }
}
void Com_lock_Wait_Cmd(void)
{
	  int32_t i_temp_data;
	  uint8_t uc_temp_data; 
	  if(cmdbusyflag == CMD_FREE)
		{
				if(uart0ReadByte(&i_temp_data))
				{
					  cmdbusyflag = CMD_BUSY;
					  GetStartTime(1000);//1 s
						uc_temp_data = i_temp_data &0xff;
						switch(uc_temp_data)
						{
							case 0x55: //get fig nums
								Send_Get_Num_Cmd();
								break;
							case 0x5A: //get list
								Send_Get_List_Cmd();
								break;
							case 0xA5://enroll
								GetStartTime(4000);// 4 s
								currentcmd = COMMON_LOCK_CMD_ID_ENROLL;
								Send_Get_Num_Cmd();
								break;
							case 0xAA://match
								GetStartTime(2000);// 4 s
								Send_Match_Cmd();
								break;
							case 0x33://delete
								 Send_Delete_Cmd();
								break;
							case 0xCC://init sensor
								Send_Init_Cmd();
								break;
							case 0x3C:
								cmdbusyflag = CMD_FREE;
								break;
							default:
								cmdbusyflag = CMD_FREE;
								break;
							
						}
					}
	   }
		else
		{
				if(currentcmd == COMMON_LOCK_CMD_ID_ENROLL_STEP2)
				{
						Send_Enroll_Cmd(fignum);
					  currentcmd = 0;
				}
		}
	
	
	
}
/*********************************************************************/
//    function name:
//        Com_Lock_Uart_Task()
//    description:
//        uart command task.
/*********************************************************************/
void Com_Lock_Sensor_Task(void)
{
	  Com_lock_Wait_Cmd();
    Com_Lock_Pause_Uart_Frame();            // compose frame.
    if(frameDone == 0x01)                   // puse command.
    {
			  memcpy(TempuartFrameBuff,uartFrameBuff,100);
        Com_Lock_Sensor_Cmd(TempuartFrameBuff);
			  memset(uartFrameBuff, 0x00, sizeof(uartFrameBuff));
        frameDone = 0x00; 
    } 
		else
		{
			if(cmdbusyflag == CMD_BUSY)
			{
					if(GetCurrentTimeCnt())
					{
						cmdbusyflag = CMD_FREE;
						debugPrint(" test time out\n");
					}
			}
		}	
}

uint8_t Touchcnt = 0;
void Sensor_Int_Callback(void)
{
	 Touchcnt ++;
	 if(g_hardware_input.finger_touch)
	 {
		   io_ext_int_en(SEBSOE_TOUCH,0);
		   io_ext_int_cfg(SEBSOE_TOUCH,EXT_INT_TRIGGER_POS_EDGE,Sensor_Int_Callback);
	     io_ext_int_en(SEBSOE_TOUCH,1);
	     g_hardware_input.finger_touch = 0;
		   //debugPrint(" sensor touch release time %d \n",Touchcnt);
	 }
	 else
	 {
		  io_ext_int_en(SEBSOE_TOUCH,0);
			//debugPrint(" sensor touch press time %d \n",Touchcnt);
		  io_ext_int_cfg(SEBSOE_TOUCH,EXT_INT_TRIGGER_NEG_EDGE,Sensor_Int_Callback);
	    io_ext_int_en(SEBSOE_TOUCH,1);
	    g_hardware_input.finger_touch = 1;
	 }
}
void Init_SensorTouch(void)
{
		io_ext_int_cfg(SEBSOE_TOUCH,EXT_INT_TRIGGER_POS_EDGE,Sensor_Int_Callback);
	  io_ext_int_en(SEBSOE_TOUCH,1);
	  g_hardware_input.finger_touch = 0;
	  
}


//=====================================================================
//    no more.
//=====================================================================

