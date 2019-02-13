/*********************************************************************************************************
 ** file:	    fsm.c
 ** description:	协议内容实现
 ** author:      li
 ** date :       2017/05/16
 ***********************************************************************************************************/
#include "fsm.h"
#include "m_uart.h"
#include "m_timer.h"
#include <string.h>
#include <mafp_sensor.h>
#include <system.h>
#include <stdio.h>
#include <stdarg.h>
#include "m_flash.h"
#include "sensor_operation_afs121n.h"
#include "stdlib.h"
#include "synoc.h"




typedef enum
{
	STATE_IDLE,                        //空闲状态
	STATE_UPLOAD,                      //上传数据状态
	STATE_DOWNLOAD,                    //下载数据状态
	STATE_ENROLL,                      //注册状态
	STATE_MATCH,                       //匹配状态
	STATE_ENROLL_NOCHECK_FINGER
}_FsmState;  //命令状态机的状态



typedef enum
{
	ENROLL_STATE_IDLE,
	ENROLL_STATE_CAPTURE,
	ENROLL_STATE_COMPUTE,
	ENROLL_STATE_LEAVE,
	ENROLL_STATE_TIMEOUT
}_EnrollState; //注册状态机的状态

typedef enum
{
	MATCH_STATE_IDLE,
	MATCH_STATE_CAPTURE,
	MATCH_STATE_COMPUTE,
	MATCH_STATE_TIMEOUT,
}_MatchState; //匹配状态机的状态


#define ENROLL_PRESS_TIMES_DEFAULT      DEFAULT_ENROLL_TIMES   //默认注册按压次数
#define ENROLL_TIMEOUT_DEFAULT          (30 * 1000 * 1000)
#define MATCH_TIMEOUT_DEFAULT           (3 * 1000 * 1000)
#define ENROLL_PRESS_TIMES_MAX          (6)


//函数申明
static void maFsmTask(S32 ch);
static S32 fsmGetFrame(S32 ch,U8 err_flag);
static U16 frameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff);
static void fsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf);
static void fsmSendFrameForPostEnroll(U8 _ack,U16 _data_len,U8 *_data_buf,S32 (*mafp_post_enroll)(void));
static void fsmSendFrameTmp(U8 _ack,U16 _data_len,U8 *_data_buf,U32 addr);                  // for setaddr
static U16 frameSetPackTmp(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff,U32 addr);  //for setaddr
static void recvTimeoutProcess(void);

//下载数据函数
static void initDownLoadTag(void);
static void fsmDownLoadTask(void);


//上传数据函数
static void fsmUpLoadTask(void);
static void fsmUploadImage(void);
static void resetUpLoadTag(void);
static void initUploadTag(void);
static void setUpLoadTaskDoneFlag(void);


//指令函数
static void fsmDownImage(void);
static void fsmVfyPwd(void);
static void fsmSetPwd(void);
static void fsmSetAddr(void);
static void fsmSetSysPara(void);
static void fsmGetSysPara(void);
static void fsmGetIdList(void);
static void fsmGetIdNum(void);
static void fsmDeleteID(void);
static void fsmEmptyId(void);
static void fsmEnrollTask(void);
static void fsmEnroll(void);
static void fsmMatch(void);
static void fsmMatchTask(void);
static void fsmInitSystem(void);
static void fsmInitSensor(void);
static void fsmHandshake(void);
static void fsmEnDebugPrint(void);
static void fsmDebugGetPara(void);
static void fsmDebugUpdate(void);
static void fsmDebugEnrollAfterDownLoad(void);
static void fsmDebugMatchAfterDownLoad(void);
static void fsmDebugResetTemplateStorage(void);
static void fsmDebugReadAllFlash(void);
static void fsmSendDataTask(U8 flag);
static void fsmSwitchProtocol(void);
static void initEnrollTag(void);
static void initMatchTag(void);
static void fsmDeleteMultiId(void);
static void fsmConfigEnroll(void);
static void fsmConfigMatch(void);
static void matchTimerRestart(void);
static U32 matchTimerElapsedMs(void);
static void fsmBurnCodeTime(void);
static void fsmCheckDetectMode(void);
static void fsmIsFingerLeave(void);

typedef enum
{
	idle,
	busy,
	done,
	running,
	null
}_Status;


//上传Tag
typedef struct
{
	U8 * upload_buff;
	S32 upload_len;
	U32 upload_data_offset;
	U32 nums;
	U32 timeout;
	U16 pos;
	U16 off;
	U16 len;
	U16 resend_now_times;
	U16 resend_max_times;
	_Status status;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 tmp_buff[256];
}_UploadTag;


typedef struct
{
	U8 *buff;
	U32 len;
	U8 tmp_buff[256];
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U32 timeout;
	U8 resend_times;
}_SendDataTag;



//下载Tag
typedef struct
{
	U8* download_buff;                          //下载数据指针
	U32 download_len;                           //下载数据的长度
	U32 download_data_offset;                   //下载数据的偏移量
	U32 nums;
	U32 last_nums;
	U8  is_download_image_value;
}_DownLoadTag;



typedef struct
{
	U8 enroll_id;
	U8 enroll_press_times;
	U32 enroll_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 enroll_cmd;
}_EnrollTag;




typedef struct
{
	U8 match_id;
	U32 match_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U16 match_time;
	U8 isShow_timeoutInfo;
	U8 match_cmd;
	void (*matchTimerRestart)(void);
	U32 (*matchTimerElapsedMs)(void);
	U32 matchTimeStart;
	U32 mathcTimeEnd;
}_MatchTag;


//全局变量
static U8  Last_Data_Flag=0;                        //下载数据时，若为最后一帧置1，否则清0
static _UploadTag UpLoad_Tag={0};
static _SendDataTag SendData_Tag={0};
static _DownLoadTag DownLoad_Tag={0};
static _EnrollTag Enroll_Tag={0};
static _MatchTag  Match_Tag={0};


#define RS_BUFF_LEN    512          //接收发送缓冲buff长度
static U8 g_recv_buff[RS_BUFF_LEN];         //接收帧缓冲
static U8 g_send_buff[RS_BUFF_LEN];         //发送帧缓冲
static _FsmState g_fsm_state;
static _EnrollState g_enroll_state;
static _MatchState g_match_state;
static void (*g_frame_func[FRAME_MAX])(void); //命令处理函数表
static U8 Burn_Code_Time_Buff[128+2];

#define NREC_TIMEOUT (1*1000*1000)

//#define NREC_TIMEOUT (2) /* 2 ticks,40ms */


static U32 recv_len = 0;
static	U16 nold_RecNum=0;
volatile static  U32 nrec_TimeOut=0;


/*********************************************************************************************************
 ** 函数名称:	fsmInit
 ** 功能描述:	初始化状态机和初始化命令处理函数表
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void fsmInit(void)
{
	//初始化命令处理函数表
	memset(g_frame_func, 0, sizeof(g_frame_func));
	g_frame_func[ENROLL] = fsmEnroll;
	g_frame_func[SERACH] = fsmMatch;
	g_frame_func[Delete_ID] = fsmDeleteID;
	g_frame_func[EMPTY_ID] = fsmEmptyId;
	g_frame_func[SET_SYS_PARA] = fsmSetSysPara;
	g_frame_func[GET_SYS_PARA] = fsmGetSysPara;
	g_frame_func[SET_PWD] = fsmSetPwd;
	g_frame_func[VFY_PWD] = fsmVfyPwd;
	g_frame_func[GET_RANDOM] = NULL;
	g_frame_func[SET_ADDR] = fsmSetAddr;
	g_frame_func[GET_ID_NUM] = fsmGetIdNum;
	g_frame_func[GET_ID_LIST] = fsmGetIdList;
	g_frame_func[DOWN_IMAGE] = fsmDownImage;
	g_frame_func[UPLOAD_IMAGE] = fsmUploadImage;


	g_frame_func[INIT_SYSTEM] =fsmInitSystem;
	g_frame_func[DEVICE_READY]=deviceReady;
	g_frame_func[INIT_SENSOR] =fsmInitSensor;
	g_frame_func[HAND_SHAKE]=fsmHandshake;
	g_frame_func[CONFIG_ENROLL]=fsmConfigEnroll;
	g_frame_func[CONFIG_MATCH]=fsmConfigMatch;

	g_frame_func[SWITCH_PROTOCOL]=fsmSwitchProtocol;

	g_frame_func[DEBUG_PRINT_EN]=fsmEnDebugPrint;

	g_frame_func[DEBUG_GET_PARA]=fsmDebugGetPara;
	g_frame_func[DEBUG_UPDATE]=fsmDebugUpdate;

	g_frame_func[DEBUG_ENROLL_AFTER_DOWNLOAD]=fsmDebugEnrollAfterDownLoad;

	g_frame_func[DEBUG_MATCH_AFTER_DOWNLOAD]= fsmDebugMatchAfterDownLoad;

	g_frame_func[DEBUG_RESET_TEMPLATE_STORAGE]=fsmDebugResetTemplateStorage;

	g_frame_func[DEBUG_READ_ALL_FLASH]=fsmDebugReadAllFlash;

	g_frame_func[Delete_MULTI_ID]=fsmDeleteMultiId;

	g_frame_func[DEBUG_READ_BURN_CODE_TIME]=fsmBurnCodeTime;

	g_frame_func[DEBUG_CHECK_DETECT_MODE]=fsmCheckDetectMode;

	g_frame_func[DEBUG_IS_FINGER_LEAVE]=fsmIsFingerLeave;

	//初始化状态机的状态
	g_fsm_state = STATE_IDLE;

	g_enroll_state = ENROLL_STATE_IDLE;

	g_match_state = MATCH_STATE_IDLE;

	initUploadTag();
	initDownLoadTag();
	initEnrollTag();
	initMatchTag();
}



static void maCompatibleFunc(S32 ch)
{
	U8 command;
	if(ch>=0)
	{
		if (fsmGetFrame(ch,0))
		{
			if(PID_COMMAND ==g_recv_buff[FRAME_PID])
			{
				command = g_recv_buff[9];      //得到命令
				if( (command>0) && (command<FRAME_MAX) &&(g_frame_func[command])&&
				    (IS_MICROARRAY_COMPATIBLE_SYNOCHIP_VAILD_FUNC(command)) )
				{
					g_frame_func[command]();
				}
			}
		}
	}
}


/*********************************************************************************************************
 ** 函数名称:	fsmTask
 ** 功能描述:	状态机主函数
 **
 ** input: ch-->串口buff中取出的字节
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void fsmTask(S32 ch)
{

	switch(Protocol)
	{
	case PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP:
		maCompatibleFunc(ch);
		synocFsmTask(ch);
		break;

	case PROTOCOL_MICROARRAY_COMPATIBLE_YALE:
		maCompatibleFunc(ch);
		//yaleFsmTask(ch);
		break;

	default:
		maFsmTask(ch);
		break;
	}
}

static void maFsmTask(S32 ch)
{
	U8 command;
	U8 tmp_buf[2]={0};
	recvTimeoutProcess();
	if (ch >= 0)
	{
		if (fsmGetFrame(ch,0))
		{
			if(Sys_ParaTag.en_passwd)
			{
			    
				if( (g_recv_buff[9]==VFY_PWD) && (PID_COMMAND ==g_recv_buff[FRAME_PID]))
				{
					fsmVfyPwd();
					return;
				}

                

				if((!Gl_Password)&&(g_recv_buff[9]!=INIT_SYSTEM))
				{
					tmp_buf[0]=VFY_PWD;//指令
					tmp_buf[1]=PS_MUST_VERIFY_PWD;
					fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
					return ;
				}
			}
			if(PID_COMMAND ==g_recv_buff[FRAME_PID])
			{
			    
				command = g_recv_buff[9];      //得到命令
				if( (command>0) && (command<FRAME_MAX) &&(g_frame_func[command]) )
				{
					g_frame_func[command]();
				}
				else
				{
					tmp_buf[0]=0;//指令
					tmp_buf[1]=PS_COMMAND_ERR ;
					fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
				}
			}
			else if( (PID_DATA  ==g_recv_buff[FRAME_PID] ) || (PID_LAST_DATA  == g_recv_buff[FRAME_PID]) )
			{
			    
				fsmDownLoadTask();
			}
			else if(PID_DATA_ACK  ==g_recv_buff[FRAME_PID])
			{
			    
				//数据包应答
				setUpLoadTaskDoneFlag();
			}
		}
	}
	else if(g_fsm_state == STATE_UPLOAD)
	{
	    
		fsmUpLoadTask();
	}
	else if (g_fsm_state == STATE_ENROLL)
	{
	    
		fsmEnrollTask();
	}
	else if (g_fsm_state == STATE_MATCH)
	{
	    
		fsmMatchTask();
	}
        

}

/*********************************************************************************************************
 ** 函数名称:	fsmCRC
 ** 功能描述:	计算CRC校验
 **
 ** input: dat-->数据buf,len-->数据长度
 ** return:返回CRC校验值
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
U8  fsmCRC(U8 *dat, U32 len)
{
	U8 crc8 = 0;
	while(len--){
		crc8 = CRC8Table[crc8 ^ *dat];
		dat++;
	}
	return crc8;
}


static void recvTimeoutProcess(void)
{
	nrec_TimeOut++;
	/*判断超时工作*/
	if(recv_len==0)
	{
		nrec_TimeOut=0;
		nold_RecNum=0;
	}

	if(nold_RecNum!=recv_len)
	{
		nrec_TimeOut=0;
		nold_RecNum=recv_len;
	}
	else//判断超时
	{
		nrec_TimeOut++;
		if(nrec_TimeOut>NREC_TIMEOUT)
		{
			recv_len=0;
            
		}
	}
}


/*********************************************************************************************************
 ** 函数名称:	fsmGetFrame
 ** 功能描述:	取帧
 **
 ** input: ch-->字节
 ** return:0-->没有取到完整帧    >0-->取到完整的帧
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static S32 fsmGetFrame(S32 ch,U8 err_flag)
{
	U32 device_addr;
	U16 frame_len=0;
	U8 pid=0;
	U8 tmp[2]={0};
	tmp[0]=0;
	tmp[1]=PS_OK;
	if (recv_len == 0)
	{
		if ( ch == (FRAME_START>>8) )
		{
			g_recv_buff[recv_len++] = ch;
		}
	}
	else if(recv_len == 1)
	{
		if ( ch == (FRAME_START&0x00ff) )
		{
			g_recv_buff[recv_len++] = ch;
		}
		else
		{
			recv_len=0;
			return 0;
		}
	}
	else
	{
		g_recv_buff[recv_len++] = ch;
		if(recv_len == 6)
		{
			device_addr=( (g_recv_buff[2]<<8)+g_recv_buff[3]);
			device_addr=( ( device_addr<<16)+(g_recv_buff[4]<<8)+g_recv_buff[5]);
			//debugPrint("%x ",device_addr);
			// 模块地址在flash中存储
			if( (device_addr!=Sys_ParaTag.device_add) && (device_addr!=0))
			{
				recv_len=0;
				tmp[1]=PS_DEVICE_ADDR_ERR;
				if(err_flag)
					fsmSendFrameTmp(PID_ACK ,0x02,&tmp[0],0);
				return 0;
			}

		}
		else if(recv_len >= FRAME_STA_LEN)
		{

			frame_len=(g_recv_buff[FRAME_LEN_HIGH ]);
			frame_len<<=8;
			frame_len|=(g_recv_buff[FRAME_LEN_LOW ]);

			if(frame_len>FRAME_MOST_LEN)
			{
				recv_len=0;
				tmp[1]=PS_RECV_ERR;
				if(err_flag)
					fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
				return 0;
			}

			frame_len+=FRAME_STA_LEN;

			if(recv_len >=frame_len)
			{
				recv_len=0;
				//检测是否校验正确
				if( fsmCRC(g_recv_buff,frame_len-1) ==  g_recv_buff[frame_len-1]  )
				{
					//无校验错误
					pid=g_recv_buff[FRAME_PID];   //标识
					if( (pid==PID_COMMAND) ||  (pid == PID_DATA )|| (pid==PID_DATA_ACK ))
					{
						Last_Data_Flag=0;
					}
					else if(pid == PID_LAST_DATA)
					{
						Last_Data_Flag=1;     //表示是最后一帧数据
					}
					else
					{
						tmp[1]=PS_PID_ERR ;
						if(err_flag)
							fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
						return 0;
					}
#if DEBUG_PRINT
					debugPrint(" frame_len == %d\n\r",frame_len);
#endif
					return frame_len;
				}
				else
				{
					//校验错误
					tmp[1]=PS_COMM_ERR;
					if(err_flag)
						fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
					return 0;

				}
			}
		}
	}
	return 0;
}


/*********************************************************************************************************
 ** 函数名称:	frameSetPack
 ** 功能描述:	封装发送帧
 **
 ** input:  _data_len-->内容长度,_data_buf-->内容buff,type-->标识,_send_buff 发送buff
 ** return: 0-->失败, >0-->一帧命令或者数据的总长度
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static U16 frameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff)
{

	U8 nwCHK = 0;
	if(_send_buff&&_data_buf)   //参数检测
	{
		/*make packet head*/
		_send_buff[0] = (FRAME_START>>8);
		_send_buff[1] = FRAME_START &0x00ff;
		_send_buff[2] = (Sys_ParaTag.device_add>>24)&0xff;
		_send_buff[3] = (Sys_ParaTag.device_add>>16)&0xff;
		_send_buff[4] = (Sys_ParaTag.device_add>>8)&0xff;
		_send_buff[5] = Sys_ParaTag.device_add&0xff;
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+FRAME_CRC) & 0xff;

		memcpy(&g_send_buff[FRAME_STA_LEN],_data_buf,_data_len);
		nwCHK=fsmCRC(g_send_buff,_data_len+FRAME_STA_LEN);

		g_send_buff[_data_len+FRAME_STA_LEN]=nwCHK;

		return (_data_len+FRAME_CRC+FRAME_STA_LEN);
	}
	else
	{
		return 0;
	}

}


/*********************************************************************************************************
 ** 函数名称:	fsmSendFrame
 ** 功能描述:	发送一帧命令或者数据
 **
 ** input: _data_len-->内容长度,_data_buf-->内容buff,_ack-->标识
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf)
{
	U16 len;
	if(_data_buf)
	{
		len=frameSetPack(_data_len,_data_buf,_ack,g_send_buff);
		uartSendBuff(g_send_buff,len);
	}
}


static void fsmSendFrameForPostEnroll(U8 _ack,U16 _data_len,U8 *_data_buf,S32 (*mafp_post_enroll)(void))
{
	U16 len;
	if(_data_buf)
	{
		len=frameSetPack(_data_len,_data_buf,_ack,g_send_buff);
		uartSendBuff(g_send_buff,6);
		mafp_post_enroll();
		uartSendBuff(g_send_buff+6,len-6);
	}
}


/*********************************************************************************************************
 ** 函数名称:	resetDownLoadTag
 ** 功能描述:	复位 下载Tag
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void initDownLoadTag(void)
{
	DownLoad_Tag.download_buff=NULL;
	DownLoad_Tag.download_data_offset=0;
	DownLoad_Tag.download_len=0;
	DownLoad_Tag.nums=0;
	DownLoad_Tag.last_nums=0;
	DownLoad_Tag.is_download_image_value=0;
}


/*********************************************************************************************************
 ** 函数名称:	 fsmDownImage
 ** 功能描述:  下载图片
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmDownImage(void)
{
	U8 tmp_buf[2]={0}, *dst=NULL;
	U32 len=0, buflen;
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;                        //
	initDownLoadTag();
	g_fsm_state = STATE_IDLE;

	len=(g_recv_buff[10] << 16) | (g_recv_buff[11] << 8) | g_recv_buff[12];  //得到下载数据的总长度

	extern int32_t sensor_get_download_buf(uint8_t *name, uint8_t **dst_addr, uint32_t size);
	buflen = sensor_get_download_buf((uint8_t *)"image.bmp", &dst, len);
	if( (len>0) && (buflen>0) && dst)
	{
		//符合下载图像的条件
		g_fsm_state = STATE_DOWNLOAD;
		DownLoad_Tag.download_buff=dst;
		DownLoad_Tag.download_len=len;
	}
	else
	{
		tmp_buf[1]=PS_PARAM_ERROR;
		g_fsm_state = STATE_IDLE;
	}
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);

}

/*********************************************************************************************************
 ** 函数名称:	  fsmDownLoadTask
 ** 功能描述:  下载状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmDownLoadTask(void)
{
	U32 nums=0,data_len=0;
	U8 tmp[4];

	if(g_fsm_state == STATE_DOWNLOAD)
	{
		tmp[0]=g_recv_buff[FRAME_STA_LEN ];
		tmp[1]=g_recv_buff[FRAME_STA_LEN +1];
		tmp[2]=PS_DOWN_IMG_ERR;

		if( DownLoad_Tag.download_buff != NULL)
		{
			data_len = ( ( (g_recv_buff[FRAME_LEN_HIGH] << 8) | g_recv_buff[FRAME_LEN_HIGH+1])-3);     //得到一桢的数据长度
			nums = ((g_recv_buff[9] << 8) | g_recv_buff[10]);                                          //得到本次帧号

			if( (DownLoad_Tag.download_data_offset+data_len) <= DownLoad_Tag.download_len)
			{
				if( nums == DownLoad_Tag.nums)
				{
					memcpy(DownLoad_Tag.download_buff+DownLoad_Tag.download_data_offset,&g_recv_buff[FRAME_STA_LEN+2],data_len);
					DownLoad_Tag.download_data_offset+=data_len;
					DownLoad_Tag.nums++;
					tmp[2]=PS_OK;
				}
				else if(nums == DownLoad_Tag.last_nums)
				{
					tmp[2]=PS_OK;
				}
				DownLoad_Tag.last_nums=nums;
			}
		}

		if( Last_Data_Flag)
		{
			g_fsm_state=STATE_IDLE;
			Last_Data_Flag=0;
			if(DownLoad_Tag.download_data_offset == DownLoad_Tag.download_len)
			{
				tmp[2]=PS_OK;
				DownLoad_Tag.is_download_image_value=1;
			}
		}
		fsmSendFrame(PID_DATA_ACK ,0x03,&tmp[0]);
	}
}


/*********************************************************************************************************
 ** 函数名称:	ResetUpLoadTag
 ** 功能描述:	复位 上传Tag
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void resetUpLoadTag(void)
{
	UpLoad_Tag.status=idle;
	UpLoad_Tag.nums=0;
	UpLoad_Tag.resend_now_times=0;
	UpLoad_Tag.upload_data_offset=0;
	UpLoad_Tag.upload_buff=NULL;
	UpLoad_Tag.upload_len=0;
}


static void initUploadTag(void)
{
	UpLoad_Tag.upload_buff=NULL;
	UpLoad_Tag.upload_len=0;
	UpLoad_Tag.upload_data_offset=0;
	UpLoad_Tag.nums=0;
	UpLoad_Tag.timeout=3*1000*1000;
	UpLoad_Tag.off=0;
	UpLoad_Tag.pos=0;
	UpLoad_Tag.resend_max_times=5;
	UpLoad_Tag.resend_now_times=0;
	UpLoad_Tag.timerElapsedUs=timerElapsedUs;
	UpLoad_Tag.timerRestart=timerRestart;
	memset(UpLoad_Tag.tmp_buff,0,sizeof(UpLoad_Tag.tmp_buff));
	UpLoad_Tag.status=idle;
}

/*********************************************************************************************************
 ** 函数名称:	 fsmUploadImage
 ** 功能描述:  上传图片
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmUploadImage(void)
{

	U8 tmp[6]={0},type=0;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];      //指令
	tmp[1]=PS_OK;
	type=g_recv_buff[FRAME_STA_LEN+1];
	resetUpLoadTag();
	g_fsm_state = STATE_IDLE;

	extern int32_t sensor_get_upload_image_buf(uint32_t idx, uint8_t **dst_addr);
	
	switch (type)
	{
	case 1://上传当前采图

		/*
		  采一帧图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(2, &UpLoad_Tag.upload_buff);
//		UpLoad_Tag.upload_buff=buff;
//		UpLoad_Tag.upload_len=256;
//		debugPrint("UpLoad_Tag.upload_len == %x\r\n",UpLoad_Tag.upload_len );
//		debugPrint("UpLoad_Tag.buff == %x\r\n",UpLoad_Tag.upload_buff );
		break;

	case 2://上传背景图
		/*
		  准备好背景图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(0, &UpLoad_Tag.upload_buff);
		break;

	case 3://上传去背景图
		/*
		  准备好背景图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(1, &UpLoad_Tag.upload_buff);
		break;

	case 4://上传按压注册图
		/*
		  准备好背景图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(4, &UpLoad_Tag.upload_buff);
		break;

	case 5://上传按压匹配图
		/*
		  准备好背景图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(5, &UpLoad_Tag.upload_buff);
		break;
		
	case 6://上传按压测试图
		/*
		  准备好背景图
		  准备好上传的图像的buff和字节数
		*/
		UpLoad_Tag.upload_len = sensor_get_upload_image_buf(6, &UpLoad_Tag.upload_buff);
		break;

	default:
		tmp[1]=PS_PARAM_ERROR;
		break;
	}

	if(UpLoad_Tag.upload_buff && (UpLoad_Tag.upload_len>0) )
	{
		g_fsm_state = STATE_UPLOAD;
		tmp[2]=(UpLoad_Tag.upload_len>>16)&0xff;
		tmp[3]=(UpLoad_Tag.upload_len>>8)&0xff;
		tmp[4]=(UpLoad_Tag.upload_len)&0xff;
		UpLoad_Tag.len=(Frame_Data_Size-3);
		UpLoad_Tag.pos=UpLoad_Tag.upload_len/UpLoad_Tag.len;
		UpLoad_Tag.off=UpLoad_Tag.upload_len%UpLoad_Tag.len;
		UpLoad_Tag.status=running;
	}
	else
	{
		tmp[1]= PS_UP_IMG_ERR;
	}
	fsmSendFrame(PID_ACK ,0x05,&tmp[0]);
	timerDelayMs(5);

}


static void setUpLoadTaskDoneFlag(void)
{
	if(g_fsm_state == STATE_UPLOAD)
	{
		UpLoad_Tag.status=done;
	}
}



/*********************************************************************************************************
 ** 函数名称:	  fsmUpLoadTask
 ** 功能描述:  上传状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmUpLoadTask(void)
{

	U8 res;
	U16 nums;
	U8 tmp[6]={0};
	if(g_fsm_state == STATE_UPLOAD)
	{
		switch(UpLoad_Tag.status)
		{
		case idle:
			g_fsm_state = STATE_IDLE;             //退出upload状态
			break;
		case running:
			if((UpLoad_Tag.upload_buff)&&(UpLoad_Tag.upload_data_offset<=UpLoad_Tag.upload_len)
			   &&(UpLoad_Tag.resend_now_times<UpLoad_Tag.resend_max_times))
			{
				UpLoad_Tag.status=busy;
				UpLoad_Tag.timerRestart();
				UpLoad_Tag.tmp_buff[0]=UpLoad_Tag.nums>>8;
				UpLoad_Tag.tmp_buff[1]=UpLoad_Tag.nums&0xff;

				if(UpLoad_Tag.nums<UpLoad_Tag.pos)
				{
					memcpy(&UpLoad_Tag.tmp_buff[2],UpLoad_Tag.upload_buff+UpLoad_Tag.nums*UpLoad_Tag.len,UpLoad_Tag.len);
					UpLoad_Tag.upload_data_offset=UpLoad_Tag.nums*UpLoad_Tag.len+UpLoad_Tag.len;
					if( (UpLoad_Tag.nums==(UpLoad_Tag.pos-1) ) && (UpLoad_Tag.off==0))
					{
						fsmSendFrame(PID_LAST_DATA,UpLoad_Tag.len+2,&UpLoad_Tag.tmp_buff[0]);
					}
					else
					{
						fsmSendFrame(PID_DATA,UpLoad_Tag.len+2,&UpLoad_Tag.tmp_buff[0]);
					}

				}
				else if(UpLoad_Tag.nums==UpLoad_Tag.pos)
				{
					if(UpLoad_Tag.off>0)
					{
						memcpy(&UpLoad_Tag.tmp_buff[2],UpLoad_Tag.upload_buff+UpLoad_Tag.nums*UpLoad_Tag.len,UpLoad_Tag.off);
						UpLoad_Tag.upload_data_offset=UpLoad_Tag.nums*UpLoad_Tag.len+UpLoad_Tag.off;
						fsmSendFrame(PID_LAST_DATA,UpLoad_Tag.off+2,&UpLoad_Tag.tmp_buff[0]);
					}
				}
			}
			break;
		case busy:
			if( UpLoad_Tag.timerElapsedUs()> UpLoad_Tag.timeout)
			{
				UpLoad_Tag.status=running;
				UpLoad_Tag.resend_now_times++;
			}

			if(UpLoad_Tag.resend_now_times>=UpLoad_Tag.resend_max_times)
			{
				g_fsm_state = STATE_IDLE;             //超时退出upload状态
				tmp[0]=UPLOAD_IMAGE;
				tmp[1]=PS_TIMEOUT;
				fsmSendFrame(PID_ACK ,0x05,&tmp[0]);
			}
			break;

		case done:
			nums=g_recv_buff[FRAME_STA_LEN ];
			nums=(nums<<8)+g_recv_buff[FRAME_STA_LEN +1];   //帧号
			res=g_recv_buff[FRAME_STA_LEN+2];               //结果码
			if(res==PS_OK )
			{
				UpLoad_Tag.status=running;
				UpLoad_Tag.resend_now_times=0;
				if(UpLoad_Tag.nums == nums)
				{
					UpLoad_Tag.nums++;
					if(UpLoad_Tag.upload_data_offset == UpLoad_Tag.upload_len )
						g_fsm_state = STATE_IDLE;
				}
			}
			else if(res == PS_UP_IMG_ERR)
			{
				g_fsm_state = STATE_IDLE;
				tmp[0]=UPLOAD_IMAGE;
				tmp[1]=PS_UP_IMG_ERR;
				fsmSendFrame(PID_ACK ,0x05,&tmp[0]);
			}
			else
				UpLoad_Tag.status=busy;
			break;
		default:
			g_fsm_state = STATE_IDLE;
			break;
		}
	}
}




/*********************************************************************************************************
 ** 函数名称:	fsmVfyPwd
 ** 功能描述: 验证口令
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmVfyPwd(void)
{
	U32 passwd;
	U8 tmp_buf[2]={0};

	ReadParaTab(&Sys_ParaTag,sizeof(_SysParaTag) );                           //读取参数

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];                                   //指令
	tmp_buf[1]=PS_OK;
	passwd=g_recv_buff[FRAME_STA_LEN+1];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+2];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+3];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+4];

	if(passwd!=Sys_ParaTag.pwd)
	{
		tmp_buf[1]=PS_INVALID_PASSWORD;
		Gl_Password=0;
	}
	else
	{
		Gl_Password=1;
	}
#if DEBUG_PRINT
	debugPrint("passwd==%x \r\n",passwd);
#endif
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}



/*********************************************************************************************************
 ** 函数名称:	fsmSetPwd
 ** 功能描述: 设置口令
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmSetPwd(void)
{
	U32 passwd;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	passwd=g_recv_buff[FRAME_STA_LEN+1];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+2];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+3];
	passwd=passwd<<8|g_recv_buff[FRAME_STA_LEN+4];

	Sys_ParaTag.en_passwd=1;
	Sys_ParaTag.pwd=passwd;
	Gl_Password=0;

	WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
#if DEBUG_PRINT
	debugPrint("passwd==%x \r\n",passwd);
	debugPrint("Sys_ParaTag.pwd==%x \r\n",Sys_ParaTag.pwd);
#endif
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}






static U16 frameSetPackTmp(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff,U32 addr)
{

	U8 nwCHK = 0;
	if(_send_buff&&_data_buf)   //参数检测
	{
		/*make packet head*/
		_send_buff[0] = (FRAME_START>>8);
		_send_buff[1] = FRAME_START &0x00ff;
		_send_buff[2] = (addr>>24)&0xff;
		_send_buff[3] = (addr>>16)&0xff;
		_send_buff[4] = (addr>>8)&0xff;
		_send_buff[5] = addr&0xff;
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+FRAME_CRC) & 0xff;

		memcpy(&g_send_buff[FRAME_STA_LEN],_data_buf,_data_len);
		nwCHK=fsmCRC(g_send_buff,_data_len+FRAME_STA_LEN);

		g_send_buff[_data_len+FRAME_STA_LEN]=nwCHK;

		return (_data_len+FRAME_CRC+FRAME_STA_LEN);
	}
	else
	{
		return 0;
	}

}

static void fsmSendFrameTmp(U8 _ack,U16 _data_len,U8 *_data_buf,U32 addr)
{
	U16 len;
	if(_data_buf)
	{
		len=frameSetPackTmp(_data_len,_data_buf,_ack,g_send_buff,addr);
		uartSendBuff(g_send_buff,len);
	}
}


/*********************************************************************************************************
 ** 函数名称:	fsmSetAddr
 ** 功能描述: 设置器件地址
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmSetAddr(void)
{
	U32 addr,addr_tmp;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	addr=g_recv_buff[FRAME_STA_LEN+1];
	addr=addr<<8|g_recv_buff[FRAME_STA_LEN+2];
	addr=addr<<8|g_recv_buff[FRAME_STA_LEN+3];
	addr=addr<<8|g_recv_buff[FRAME_STA_LEN+4];

	addr_tmp=Sys_ParaTag.device_add;
	Sys_ParaTag.device_add=addr;
	WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));

	g_fsm_state = STATE_IDLE;
#if DEBUG_PRINT
	debugPrint("Sys_ParaTag.device_add==%x \r\n",Sys_ParaTag.device_add);
#endif

	fsmSendFrameTmp(PID_ACK ,0x02,&tmp_buf[0],addr_tmp);
}



/*********************************************************************************************************
 ** 函数名称:	fsmSetSysPara
 ** 功能描述: 设置系统参数
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmSetSysPara(void)
{
	U8 type,para,flag=0;
	U8 tmp_buf[2]={0};
	U32 timeout=0xffffff;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];     //指令
	tmp_buf[1]=PS_OK;

	type=g_recv_buff[FRAME_STA_LEN+1];
	para=g_recv_buff[FRAME_STA_LEN+2];


	switch(type)
	{
	case SYS_PARA_BAUDRATE:
		if(para>=1)
		{
			flag|=0x01;
			Baud_Rate=para;
			Sys_ParaTag.baudrate=Baud_Rate;
			WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));

#if PROTOCOL ==  PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP
			Synoc_Sys_ParaTag.cfg_baudrate=Baud_Rate;
			writeSynocParaTab(&Synoc_Sys_ParaTag,sizeof(_SynocSysParaTag));
#endif
		}
		else
		{
			tmp_buf[1]=PS_PARAM_ERROR;
		}
		break;
	case SYS_PARA_SECLVL:
		break;
	case SYS_PARA_PACKLEN:
		if( /*(para>=0) && */(para<=3))
		{
			Sys_ParaTag.pkt_size=para;
			WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
			switch(para)
			{
			case 0: Frame_Data_Size=32; break;
			case 1: Frame_Data_Size=64; break;
			case 2: Frame_Data_Size=128; break;
			case 3: Frame_Data_Size=256; break;
			default:Frame_Data_Size=256;break;
			}

		}
		else
		{
			tmp_buf[1]=PS_PARAM_ERROR;
		}
		break;
	default:
		tmp_buf[1]=PS_INVALID_REG;
		break;
	}

	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
	if(flag&0x01)
	{
		timerDelayMs(20);
		while((!uartSendBuffEmpty())&&(timeout--));
		uartInit(Baud_Rate*9600);
		timerDelayMs(2);

	}
}

/*********************************************************************************************************
 ** 函数名称:	fsmReadSysPara
 ** 功能描述: 读系统参数
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetSysPara(void)
{
	U8 tmp_buf[19]={0};

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	//版本
	tmp_buf[2]=Sys_ParaTag.version[0];
	tmp_buf[3]=Sys_ParaTag.version[1];
	tmp_buf[4]=Sys_ParaTag.version[2];
	tmp_buf[5]=Sys_ParaTag.version[3];
	tmp_buf[6]=Sys_ParaTag.version[4];
	tmp_buf[7]=Sys_ParaTag.version[5];
	tmp_buf[8]=Sys_ParaTag.version[6];
	tmp_buf[9]=Sys_ParaTag.version[7];

	//指纹库大小
	tmp_buf[10]=(Sys_ParaTag.index>>8)&0xff;
	tmp_buf[11]=Sys_ParaTag.index&0xff;

	tmp_buf[12]=Sys_ParaTag.device_add>>24;
	tmp_buf[13]=Sys_ParaTag.device_add>>16;
	tmp_buf[14]=Sys_ParaTag.device_add>>8;
	tmp_buf[15]=Sys_ParaTag.device_add;

	tmp_buf[16]=Sys_ParaTag.pkt_size;
	tmp_buf[17]=Baud_Rate;
	tmp_buf[18]=Sys_ParaTag.protocol;

	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x13,&tmp_buf[0]);
}



/*********************************************************************************************************
 ** 函数名称:	fsmReadConList
 ** 功能描述: 读取指纹ID索引列表
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetIdList(void)
{
	U8 tmp_buf[34]={0};
	U8 page=g_recv_buff[FRAME_STA_LEN+1];
	U16 i;
	memset(Index_Tab,0,sizeof(Index_Tab));
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	if( /*(page>=0) && */(page<4))
	{
		for(i=0;i<MAX_ID;i++)
		{
			if(mafp_get_enrollid_status(i))
			{
				Index_Tab[i/PAGE_SIZE][i/PAGE_TAP]|=1<<(i%PAGE_TAP);
			}
		}
	}
	else
	{
		page=0;
		tmp_buf[1]=PS_PARAM_ERROR;
	}

	memcpy(&tmp_buf[2],Index_Tab[page]+0,sizeof(Index_Tab)/4);
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x22,&tmp_buf[0]);
}



/*********************************************************************************************************
 ** 函数名称:	fsmTemplateNum
 ** 功能描述: 读取录入的指纹数量
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmGetIdNum(void)
{
	U16 num;
	U8 tmp_buf[4]={0};

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	num=mafp_get_used_enrollids();
	tmp_buf[2]=num>>8;
	tmp_buf[3]=num;
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x4,&tmp_buf[0]);
}




/*********************************************************************************************************
 ** 函数名称:	fsmDeleteID
 ** 功能描述: 删除指纹ID
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmDeleteID(void)
{
	U16 id;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];
	tmp_buf[1]=PS_DEL_TEMP_ERR;
	id=g_recv_buff[FRAME_STA_LEN+1]<<8|g_recv_buff[FRAME_STA_LEN+2];
	if (/*(id >=0) && */(id < MAX_ID))
	{
		if (mafp_remove_enrollid(id) == 0)
		{
			tmp_buf[1]= PS_OK ;
		}
	}
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


/*********************************************************************************************************
 ** 函数名称:	fsmEmpty
 ** 功能描述: 清空指纹库
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEmptyId(void)
{
	U8 tmp_buf[2]={0};
	g_fsm_state = STATE_IDLE;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	if(mafp_clear_all_enrollids() == ERROR_CLEAR_FIDS)
		tmp_buf[1]=PS_CLEAR_TEMP_ERR ;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}



static void initEnrollTag(void)
{
	Enroll_Tag.enroll_press_times=ENROLL_PRESS_TIMES_DEFAULT;
	Enroll_Tag.enroll_timeout=ENROLL_TIMEOUT_DEFAULT;
	Enroll_Tag.timerRestart=timerRestart;
	Enroll_Tag.timerElapsedUs=timerElapsedUs;
	Enroll_Tag.enroll_id=0;
	Enroll_Tag.enroll_cmd=ENROLL;
}




/*********************************************************************************************************
 ** 函数名称:	fsmEnroll
 ** 功能描述: 注册指纹
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEnroll(void)
{
	U8 tmp[2]={0};
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	g_fsm_state = STATE_IDLE;

	tmp[1]=PS_RESPONSE;
	fsmSendFrame(PID_ACK ,0x02,&tmp[0]);        //发送应答包

	mafp_cancel_enroll();
	Enroll_Tag.enroll_press_times=ENROLL_PRESS_TIMES_DEFAULT;
	Enroll_Tag.enroll_timeout=ENROLL_TIMEOUT_DEFAULT;
	Enroll_Tag.enroll_id = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];

	if ( (Enroll_Tag.enroll_id >= MAX_ID))
	{
	    timerDelayMs(5);
		tmp[1]=PS_ADDRESS_OVER;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else if( (mafp_get_enrollid_status(Enroll_Tag.enroll_id))== 1)
	{
	    
	    timerDelayMs(5);
		tmp[1]=PS_DUL_ENROLL_ERROR;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else
	{
		g_fsm_state = STATE_ENROLL;
		g_enroll_state = ENROLL_STATE_CAPTURE;
		Enroll_Tag.timerRestart();
		Enroll_Tag.enroll_cmd=ENROLL;
	}
}


static void fsmConfigEnroll(void)
{
	U8 tmp[2]={0},timeout=0;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	g_fsm_state = STATE_IDLE;

	tmp[1]=PS_RESPONSE;
	fsmSendFrame(PID_ACK ,0x02,&tmp[0]);        //发送应答包

	mafp_cancel_enroll();

	Enroll_Tag.enroll_timeout=ENROLL_TIMEOUT_DEFAULT;
	Enroll_Tag.enroll_id = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];
	Enroll_Tag.enroll_press_times=g_recv_buff[FRAME_STA_LEN+3];
	timeout=g_recv_buff[FRAME_STA_LEN+4];

	if( (Enroll_Tag.enroll_press_times == 0) || (Enroll_Tag.enroll_press_times > ENROLL_PRESS_TIMES_MAX) )
		Enroll_Tag.enroll_press_times=ENROLL_PRESS_TIMES_DEFAULT;

	if(timeout <= 60)
		Enroll_Tag.enroll_timeout=timeout*1000*1000;



	if ( (Enroll_Tag.enroll_id >= MAX_ID))
	{
		tmp[1]=PS_ADDRESS_OVER;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else if( (mafp_get_enrollid_status(Enroll_Tag.enroll_id))== 1)
	{
		tmp[1]=PS_DUL_ENROLL_ERROR;
		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	}
	else
	{
		g_fsm_state = STATE_ENROLL;
		g_enroll_state = ENROLL_STATE_CAPTURE;
		Enroll_Tag.timerRestart();
		Enroll_Tag.enroll_cmd=CONFIG_ENROLL;
	}
}





/*********************************************************************************************************
 ** 函数名称:	fsmEnrollTask
 ** 功能描述: 注册状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmEnrollTask(void)
{
	U8 tmp[2];
	S32 ret, r;

	tmp[0]=Enroll_Tag.enroll_cmd;
	if (g_fsm_state != STATE_ENROLL)
	{
		g_enroll_state = ENROLL_STATE_IDLE;
	}

	switch(g_enroll_state)
	{
	case ENROLL_STATE_IDLE:
		break;
	case ENROLL_STATE_CAPTURE:

		if((Enroll_Tag.enroll_timeout>0) && (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout))
		{
			mafp_cancel_enroll();
			g_enroll_state = ENROLL_STATE_TIMEOUT;
			break;
		}

		r = mafp_pre_enroll();
		/*
		 * ERROR_ENROLL_NO_TOUCH
		 * ERROR_BAD_IMAGE
		 * ERROR_GET_IMAGE
		 * 0-->ok
		 * */
		if (r == 0)
		{
			//ok
			g_enroll_state = ENROLL_STATE_COMPUTE;
		}
		else if(r == ERROR_ENROLL_NO_TOUCH)
		{
			g_enroll_state = ENROLL_STATE_CAPTURE;
		}
		else
		{
			g_enroll_state = ENROLL_STATE_LEAVE;
			tmp[1]=PS_GET_IMG_ERR;
			fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
		}

		break;

	case ENROLL_STATE_COMPUTE:
		ret = mafp_enroll(Enroll_Tag.enroll_id);
		/*
		 * ERROR_ENROLL_NO_MOVE
		 * ERROR_BAD_PARAMETERS
		 * ERROR_ENROLL_LAST_INCOMPLETE
		 * ERROR_ENROLL_EXCEED_MAX_TIMES
		 * ERROR_ENROLL_ALGO_FAIL
		 * enroll times -->OK
		 * */
		g_enroll_state = ENROLL_STATE_LEAVE;
		if( ret>0 )
		{
			//valid times
			if(ret < Enroll_Tag.enroll_press_times)
			{
				tmp[1] = PS_NO_ERROR;
			}
			else
			{
				tmp[1] =PS_OK;
				g_fsm_state = STATE_IDLE;
				g_enroll_state = ENROLL_STATE_IDLE;
				fsmSendFrameForPostEnroll(PID_ACK ,0x02,&tmp[0],mafp_post_enroll);
				break;
			}
		}
		else
		{
			tmp[1] = PS_ENROLL_ERR;
			g_enroll_state = ENROLL_STATE_IDLE;
			g_fsm_state = STATE_IDLE;
		}

		fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
		Enroll_Tag.timerRestart();
		break;
	case ENROLL_STATE_LEAVE:
		if (mafp_finger_leave() == 1)
		{
			g_enroll_state = ENROLL_STATE_CAPTURE;
		}
		else if((Enroll_Tag.enroll_timeout>0) && (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout))
		{
			g_enroll_state = ENROLL_STATE_TIMEOUT;
		}
		break;
	case ENROLL_STATE_TIMEOUT:
		tmp[1]=PS_TIMEOUT;                          //暂定错误码
		g_enroll_state = ENROLL_STATE_IDLE;
		g_fsm_state = STATE_IDLE;                   //退出注册状态机
		mafp_cancel_enroll();
		if(Enroll_Tag.enroll_cmd== CONFIG_ENROLL)
			fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
		break;
	}
}



static void matchTimerRestart(void)
{
	Match_Tag.matchTimeStart=systemTick();
}


static U32 matchTimerElapsedMs(void)
{
	Match_Tag.mathcTimeEnd=systemTick();
	return ( (Match_Tag.mathcTimeEnd-Match_Tag.matchTimeStart));
}


static void initMatchTag(void)
{
	Match_Tag.match_id=0xff;
	Match_Tag.match_timeout=MATCH_TIMEOUT_DEFAULT;
	Match_Tag.timerElapsedUs=timerElapsedUs;
	Match_Tag.timerRestart=timerRestart;
	Match_Tag.match_time=0;
	Match_Tag.isShow_timeoutInfo=1;
	Match_Tag.match_cmd=SERACH;
	Match_Tag.matchTimerRestart= matchTimerRestart;
	Match_Tag.matchTimerElapsedMs = matchTimerElapsedMs;
}


/*********************************************************************************************************
 ** 函数名称:	fsmMatch
 ** 功能描述: 匹配指纹
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmMatch(void)
{
	//暂时不判断参数
	//U16 start,nums;
	U8 tmp[4]={0};
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	tmp[1]=PS_RESPONSE;
	fsmSendFrame(PID_ACK ,0x04,&tmp[0]);    //发送应答包

	Match_Tag.match_id=0xff;
	Match_Tag.match_timeout=MATCH_TIMEOUT_DEFAULT;
	Match_Tag.match_time=0;
	Match_Tag.isShow_timeoutInfo=1;
	Match_Tag.match_cmd=SERACH;

	g_fsm_state = STATE_MATCH;
	g_match_state = MATCH_STATE_CAPTURE;
	//start = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];
	//nums  = (g_recv_buff[FRAME_STA_LEN+3]<<8)|g_recv_buff[FRAME_STA_LEN+4];
	//if( (start>=MAX_ID)|| (start+nums))
	Match_Tag.timerRestart();
}


static void fsmConfigMatch(void)
{
	//暂时不判断参数
	//U16 start,nums;
	U8 tmp[6]={0};
	U16 timeout=0;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];
	tmp[1]=PS_RESPONSE;
	fsmSendFrame(PID_ACK ,0x06,&tmp[0]);    //发送应答包

	Match_Tag.match_id=0xff;
	Match_Tag.match_timeout=MATCH_TIMEOUT_DEFAULT;
	Match_Tag.match_time=0;
	Match_Tag.match_cmd=CONFIG_MATCH;

	timeout=g_recv_buff[FRAME_STA_LEN+5];
	Match_Tag.isShow_timeoutInfo=g_recv_buff[FRAME_STA_LEN+6];

	if(timeout<=60)
		Match_Tag.match_timeout=timeout*1000*1000;



	g_fsm_state = STATE_MATCH;
	g_match_state = MATCH_STATE_CAPTURE;
	//start = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];
	//nums  = (g_recv_buff[FRAME_STA_LEN+3]<<8)|g_recv_buff[FRAME_STA_LEN+4];
	//if( (start>=MAX_ID)|| (start+nums))
	Match_Tag.timerRestart();
}


/*********************************************************************************************************
 ** 函数名称:	fsmMatchTask
 ** 功能描述: 匹配状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmMatchTask(void)
{
	U8 tmp[6]={0},para_len=0;
	U32 match_time;
	int ret;

    
	tmp[0]=Match_Tag.match_cmd;

	if(Match_Tag.match_cmd == SERACH)
		para_len=4;
	else if(Match_Tag.match_cmd == CONFIG_MATCH)
		para_len=6;

	if (g_fsm_state != STATE_MATCH){
		g_match_state = MATCH_STATE_IDLE;
	}

	switch(g_match_state)
	{
	case MATCH_STATE_IDLE:
		break;
	case MATCH_STATE_CAPTURE:
        

		if ((Match_Tag.match_timeout>0)&&(Match_Tag.timerElapsedUs() > Match_Tag.match_timeout))
		{
			g_match_state = MATCH_STATE_TIMEOUT;
			break;
		}



		ret=mafp_pre_authenticate();

        
        
        
		/*
		 * ERROR_AUTH_NO_TOUCH
		 * ERROR_BAD_IMAGE
		 * ERROR_GET_IMAGE
		 * 0-->OK
		 * */
		if (ret == 0)
		{
			g_match_state = MATCH_STATE_COMPUTE;
		}
		else if(ret == ERROR_AUTH_NO_TOUCH)
		{
			g_match_state = MATCH_STATE_CAPTURE;
		}
		else
		{
			g_fsm_state = STATE_IDLE;
			g_match_state = MATCH_STATE_IDLE;
			tmp[1]=PS_GET_IMG_ERR;

            
			fsmSendFrame(PID_ACK ,para_len,&tmp[0]);

            
		}
		break;
	case MATCH_STATE_COMPUTE:
        
		Match_Tag.matchTimerRestart();
        
        
		ret = mafp_authenticate(Match_Tag.match_id);
        
        
		match_time=Match_Tag.matchTimerElapsedMs();
        

		tmp[4]=(match_time>>8)&0xff;
		tmp[5]=(match_time&0xff);

        

		/*
		 * ERROR_AUTH_NO_MOVE
		 * ERROR_AUTH_NO_MATCH
		 * ERROR_AUTH_ALGO_FAIL
		 * match_id
		 *
		 * */
		if(ret >= 0 )
		{
			tmp[1]=PS_OK;
			tmp[2]=(ret>>8)&0xff;
			tmp[3]=ret&0xff;
		}
		else
		{
			tmp[1]=PS_NOT_SEARCHED;
		}



		mafp_post_authenticate();

        
		g_match_state = MATCH_STATE_IDLE;
		g_fsm_state = STATE_IDLE;
        
		fsmSendFrame(PID_ACK ,para_len,&tmp[0]);

        
        
		break;
	case MATCH_STATE_TIMEOUT:
        
		tmp[1]=PS_TIMEOUT;
		g_match_state = MATCH_STATE_IDLE;
		g_fsm_state = STATE_IDLE;
		if(Match_Tag.isShow_timeoutInfo)
			fsmSendFrame(PID_ACK ,para_len,&tmp[0]);
		break;
//	case MATCH_STATE_AFTER_COMPUTE:
//
//		mafp_post_authenticate();
//		g_match_state = MATCH_STATE_IDLE;
//		g_fsm_state = STATE_IDLE;
//
//		break;
	}
}




/*********************************************************************************************************
 ** 函数名称:	fsmResetFactory
 ** 功能描述: 恢复出厂设置
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmInitSystem(void)
{
	U32 addr;
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

    
	ReadParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	addr=Sys_ParaTag.device_add;

	initDeviceErase();
	hardwareInit();
    
	if(mafp_clear_all_enrollids())
		tmp_buf[1]=PS_INITSYSTEM_ERR;
	initDeviceInfo();
	fsmSendFrameTmp(PID_ACK ,0x02,&tmp_buf[0],addr);
	timerDelayMs(20);
	while(!uartSendBuffEmpty());
	//uartInit(Baud_Rate*9600);
	timerDelayMs(2);
}



/*********************************************************************************************************
 ** 函数名称:	fsmResetFactory
 ** 功能描述: 恢复出厂设置
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmInitSensor(void)
{
	U8 tmp_buf[2]={0};
	
	g_fsm_state = STATE_IDLE;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];
	if(mafp_sensor_parameters_reinit())
		tmp_buf[1]=PS_INITSEBSOR_ERR;
	else
		tmp_buf[1]=PS_OK;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}




/*********************************************************************************************************
 ** 函数名称:	fsmHandle
 ** 功能描述: 握手指令
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void fsmHandshake(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}




//表示 所有接口初始化完成
void deviceReady(void)
{
	S32 res,bad_blocks;
	U8 tmp_buf[4]={0};
	tmp_buf[0]=DEVICE_READY;   //指令

	res=mafp_init(&bad_blocks);

	if(res == ERROR_LOAD_PROPERTIES)
	{
		Device_Ready_Flag|=DEVICE_READY_SENSOR_CRC_FAILE;
	}
	else if( (res!=0))
	{
		Device_Ready_Flag|=DEVICE_READY_SENSOR_BREAKDOWN;
	}
	else
	{
		Device_Ready_Flag&=~(DEVICE_READY_SENSOR_BREAKDOWN|DEVICE_READY_SENSOR_CRC_FAILE);
	}
	tmp_buf[1]=Device_Ready_Flag;
	tmp_buf[2]=(bad_blocks>>8)&0xff;
	tmp_buf[3]=(bad_blocks&0xff);

	if(Protocol == PROTOCOL_MICROARRAY)
	{
		fsmSendFrame(PID_ACK ,0x04,&tmp_buf[0]);
	}
	else  if(Protocol == PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP)
	{
		U8 synoc_start_flag=0x55;
		uartSendBuff(&synoc_start_flag,1);
	}

}



//用于调试，必须在 uartInit(Sys_ParaTag.baudrate*9600) 后使用
void fsmDebugPrint(const char *fmt, ...)
{
	if(Sys_ParaTag.en_debug_print)
	{
		timerDelayMs(3);
		//S32 ret;
		U8 tmp_buf[256];
		va_list args;
		va_start(args, fmt);
		/*ret = */vsprintf((char *)&tmp_buf[1], fmt, args);
		va_end(args);
		tmp_buf[0]= DEBUG_PRINT_MODE;
		fsmSendFrame(PID_DEBUG_PRINT ,sizeof(tmp_buf)-2,&tmp_buf[0]);
		timerDelayMs(30);
	}
	return ;
}


//用于调试
static void fsmEnDebugPrint(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},flag=0;
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;
	flag=g_recv_buff[FRAME_STA_LEN+1];
	if(flag)
	{
		Sys_ParaTag.en_debug_print=1;
	}
	else
	{
		Sys_ParaTag.en_debug_print=0;
	}

	WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


//用于调试
static void fsmDebugGetPara(void)
{
	U16 sensor_id = 0, qflash_id = 0;
	S32 ret=1;
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[32]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	//获取sensor 参数  for cui
	sensor_id=mafp_get_chipid();
	if(ret > 0)
	{
		tmp_buf[2]=0;
		tmp_buf[3]=sensor_id&0xff;
	}
	else
	{
		tmp_buf[2]=0;
		tmp_buf[3]=0;
	}
	//获取外挂flash 参数 for cui
	qflash_id= 0xffff;
	tmp_buf[4]=qflash_id>>8;
	tmp_buf[5]=qflash_id&0xff;
	fsmSendFrame(PID_ACK ,0x20,&tmp_buf[0]);
}





/*********************************************************************************************************
 ** 函数名称:fsmTaskForUpload
 ** 功能描述:	状态机主函数用于upload
 **
 ** input: ch-->串口buff中取出的字节
 ** return:void
 ** output:0--> 收到数据应答包   1-->收到其他命令包则打断上传   2-->等待收到完整一帧
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static U8 fsmTaskForSendData(S32 ch)
{
	if (ch >= 0)
	{
		if (fsmGetFrame(ch,1))
		{
			if(PID_DATA_ACK  ==g_recv_buff[FRAME_PID])
			{
				return 0;
			}
			else if(PID_COMMAND  ==g_recv_buff[FRAME_PID])
			{
				return 1;
			}
		}
	}
	return 2;
}



/*********************************************************************************************************
 ** 函数名称:	  fsmUpLoadTask
 ** 功能描述:  上传状态机
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
// flag = 0 send image
// flag = 1 send eflash data
static void fsmSendDataTask(U8 flag)
{
	S32 ch;
	U32 len=(Frame_Data_Size-3);
	U32 pos=SendData_Tag.len/len;
	U32 off=SendData_Tag.len%len;
	U16 i=0,frameNum=0;
	U8 ret=1,res;
	U8 end=0;

	SendData_Tag.timerElapsedUs=timerElapsedUs;
	SendData_Tag.timerRestart=timerRestart;
	SendData_Tag.timeout=4*1000*1000;
	SendData_Tag.resend_times=10;

#if DEBUG_PRINT
	debugPrint("pos=%d\r\n",pos);
	debugPrint("off=%d\r\n",off);
#endif
	while(1)
	{
#if DEBUG_PRINT
		debugPrint("i=%d ",i);
#endif
//	    timerDelayMs(1);
		SendData_Tag.timerRestart();
		SendData_Tag.tmp_buff[0]=i>>8;
		SendData_Tag.tmp_buff[1]=i&0xff;
		if(i<pos)
		{
			if( (i==(pos-1) ) && (off==0))
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,len);
				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),len);

				fsmSendFrame(PID_LAST_DATA,len+2,&SendData_Tag.tmp_buff[0]);
				end=1;
			}
			else
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,len);
				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),len);

				fsmSendFrame(PID_DATA,len+2,&SendData_Tag.tmp_buff[0]);
			}
		}
		if(i==pos)
		{
			if(off>0)
			{
				if(flag == FSM_SEND_DATA_IMAGE)
					memcpy(&SendData_Tag.tmp_buff[2],SendData_Tag.buff+i*len,off);
				else if(flag == FSM_SEND_DATA_EFLASH1_DATA)
					memcpy(&SendData_Tag.tmp_buff[2],(U8 *)(FSM_EFLASH1_READ_START_ADDR+i*len),off);

				fsmSendFrame(PID_LAST_DATA,off+2,&SendData_Tag.tmp_buff[0]);
				end=1;
			}
			else
			{
				break;
			}
		}
		while( SendData_Tag.timerElapsedUs()<SendData_Tag.timeout)
		{
			uartReadByte(&ch);
			ret=fsmTaskForSendData(ch);

			if(ret==0)
			{
				res=g_recv_buff[FRAME_STA_LEN+2];        //获取结果码s
				if(res==PS_OK)
				{
					i++;
				}
				else if(res==PS_END_UPLOAD)
				{
					end=1;
				}
				else
				{
					frameNum=g_recv_buff[FRAME_STA_LEN ];
					frameNum=frameNum<<8;
					frameNum=frameNum|g_recv_buff[FRAME_STA_LEN +1];
					if(frameNum>pos)
					{
						i=0;
					}
					else
					{
						i=frameNum;
					}
					end=0;
				}
				break;
			}
			else if(ret==1)
			{
				end=1;
				break;
			}
		}
		if(SendData_Tag.timerElapsedUs()>=SendData_Tag.timeout)
		{

#if DEBUG_PRINT
			debugPrint("timeout resend_times\r\n",SendData_Tag.resend_times);
#endif
			SendData_Tag.resend_times--;
			if(SendData_Tag.resend_times==0)
				break;
			else
				continue;
		}

		if( end ||(SendData_Tag.resend_times==0))
			break;
	}
}


//成功返回0，失败返回1
U8 SendData(char *name,U8 *buf,U32 len)
{

	timerDelayMs(3);
	U8 tmp[256]={0},flag=0;

	if((len>0)&&(buf)&&(name))
	{
		strcpy((char *)&tmp[4],(char *)name);
		tmp[0]=DEBUG_UPLOAD;
		tmp[1]=(len>>16)&0xff;
		tmp[2]=(len>>8)&0xff;
		tmp[3]=(len)&0xff;
		fsmSendFrame(PID_COMMAND ,0xff,&tmp[0]);

		timerDelayMs(3);
		SendData_Tag.len=len;
		SendData_Tag.buff=buf;
		fsmSendDataTask(FSM_SEND_DATA_IMAGE);
	}
	else
	{
		flag=1;
	}
	timerDelayMs(3);
	return flag;
}



static void fsmDebugUpdate(void)
{
	U8 tmp_buf[2]={0};
	
	g_fsm_state = STATE_IDLE;

	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_UPDATE_FAIL;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
	timerDelayMs(3);
	while(!uartSendBuffEmpty());
}


static void fsmDebugEnrollAfterDownLoad(void)
{
	S32 ret;
	U16 enroll_id;
	U8 tmp[2]={0},post_enroll_flag=0;

	tmp[0]=g_recv_buff[FRAME_STA_LEN];

	g_fsm_state = STATE_IDLE;

	enroll_id = (g_recv_buff[FRAME_STA_LEN+1]<<8)|g_recv_buff[FRAME_STA_LEN+2];
	post_enroll_flag=g_recv_buff[FRAME_STA_LEN+3];

	if ( (enroll_id >= MAX_ID))
	{
		tmp[1]=PS_ADDRESS_OVER;
	}
	else if( (mafp_get_enrollid_status(enroll_id))== 1)
	{
		tmp[1]=PS_DUL_ENROLL_ERROR;
	}
	else
	{
		if(post_enroll_flag)
		{
			ret=mafp_post_enroll();
			if(!ret)
				tmp[1]=PS_OK;
			else
				tmp[1]=PS_ENROLL_ERR;
		}
		else if(DownLoad_Tag.is_download_image_value)
		{
			DownLoad_Tag.is_download_image_value=0;
			sensor_save_download_buf();
			/*
			 * ERROR_ENROLL_LAST_INCOMPLETE
			 * ERROR_ENROLL_EXCEED_MAX_TIMES
			 * ERROR_BAD_IMAGE
			 * ERROR_ENROLL_ALGO_FAIL
			 * */
			ret=sensor_enroll_image_testing(enroll_id);

			if(ret>0)
				tmp[1]=PS_NO_ERROR;
			else
				tmp[1]=PS_GET_IMG_ERR;
		}
		else
		{
			tmp[1]=PS_GET_IMG_ERR;
		}
	}
	fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
}



static void fsmDebugMatchAfterDownLoad(void)
{
	S32 ret;
	U8 tmp[4]={0};

	tmp[0]=g_recv_buff[FRAME_STA_LEN];

	g_fsm_state = STATE_IDLE;

	if(DownLoad_Tag.is_download_image_value)
	{
		DownLoad_Tag.is_download_image_value=0;
		sensor_save_download_buf();
		/*
		 * ERROR_BAD_IMAGE
		 * ERROR_AUTH_NO_MATCH
		 * */
		ret=sensor_authenticate_image_testing();

		if( ret == ERROR_BAD_IMAGE )
			tmp[1]=PS_GET_IMG_ERR;
		else if(ret == ERROR_AUTH_NO_MATCH)
			tmp[1]=PS_NOT_SEARCHED ;
		else
		{
			tmp[2]=(ret>>8)&0xff;
			tmp[3]=ret&0xff;
			tmp[1]=PS_OK;
		}
	}
	else
	{
		tmp[1]=PS_GET_IMG_ERR;
	}

	fsmSendFrame(PID_ACK ,0x04,&tmp[0]);
}



static void fsmDebugResetTemplateStorage(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_OK;

	if(mafp_reset_templates_storage())
		tmp_buf[1]=PS_CLEAR_BADBLOCKS_ERR;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


static void fsmDebugReadAllFlash(void)
{
	U8 tmp[256]={0};

	g_fsm_state = STATE_IDLE;
	tmp[0]=g_recv_buff[FRAME_STA_LEN];         //指令
	tmp[1]=PS_OK;
	fsmSendFrame(PID_ACK ,0x02,&tmp[0]);
	timerDelayMs(5);

	strcpy((char *)&tmp[4],"EFLASH1_DATA.hex");
	tmp[0]=DEBUG_UPLOAD;
	tmp[1]=(FSM_EFLASH1_READ_SIZE>>16)&0xff;
	tmp[2]=(FSM_EFLASH1_READ_SIZE>>8)&0xff;
	tmp[3]=(FSM_EFLASH1_READ_SIZE)&0xff;
	fsmSendFrame(PID_COMMAND ,0xff,&tmp[0]);

	timerDelayMs(5);
	SendData_Tag.len=FSM_EFLASH1_READ_SIZE;
	SendData_Tag.buff=NULL;
	fsmSendDataTask(FSM_SEND_DATA_EFLASH1_DATA);
}


static void fsmSwitchProtocol(void)
{
	g_fsm_state = STATE_IDLE;
	U8 tmp_buf[2]={0},protocol;
	protocol=g_recv_buff[FRAME_STA_LEN+1];
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_SWITCH_PROTOCOL_FAIL;

	if(protocol == PROTOCOL_MICROARRAY)
	{
		//microarray proticol
		Sys_ParaTag.protocol=PROTOCOL_MICROARRAY;
		Protocol=PROTOCOL_MICROARRAY;
		WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
		tmp_buf[1]=PS_OK;

	}
#if PROTOCOL ==  PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP
	else if(protocol ==PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP)
	{
		//compatible synoc protocol
		Sys_ParaTag.protocol=PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP;
		Protocol=PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP;
		WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
		tmp_buf[1]=PS_OK;
	}

#endif
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}


static void fsmDeleteMultiId(void)
{
	U16 start,nums,i;
	U8 tmp_buf[2]={0};
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];
	tmp_buf[1]=PS_DEL_TEMP_ERR;

	start=g_recv_buff[FRAME_STA_LEN+1]<<8|g_recv_buff[FRAME_STA_LEN+2];
	nums=g_recv_buff[FRAME_STA_LEN+3]<<8|g_recv_buff[FRAME_STA_LEN+4];

	if((start+nums) <= MAX_ID)
	{
		for(i=0;i<nums;i++)
		{
			if (mafp_remove_enrollid(start+i) == 0)
				tmp_buf[1]= PS_OK ;
			else
			{
				tmp_buf[1]=PS_DEL_TEMP_ERR;
				break;
			}
		}
	}
	g_fsm_state = STATE_IDLE;
	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}

static void fsmBurnCodeTime(void)
{
	g_fsm_state = STATE_IDLE;
	Burn_Code_Time_Buff[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	Burn_Code_Time_Buff[1]=PS_OK;
	readBurnCodeTime(Burn_Code_Time_Buff+2,sizeof(Burn_Code_Time_Buff)-2);
	fsmSendFrame(PID_ACK ,sizeof(Burn_Code_Time_Buff),&Burn_Code_Time_Buff[0]);
}



static void fsmIsFingerLeave(void)
{
	U8 tmp_buf[2]={0};
	g_fsm_state = STATE_IDLE;
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_FINGER_EXIST;

	if (mafp_finger_leave() == 1)
		tmp_buf[1]=PS_OK;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}



static void fsmCheckDetectMode(void)
{
	U8 tmp_buf[2]={0};
	g_fsm_state = STATE_IDLE;
	tmp_buf[0]=g_recv_buff[FRAME_STA_LEN];   //指令
	tmp_buf[1]=PS_DETECT_MODE_FAIL;
	if (mafp_detect_mode()==0)
		tmp_buf[1]=PS_OK;

	fsmSendFrame(PID_ACK ,0x02,&tmp_buf[0]);
}

