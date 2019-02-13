/*
 * synoc.c
 *
 *  Created on: 2017年10月23日
 *      Author: li
 */

#include"synoc.h"





#include "m_uart.h"
#include "m_timer.h"
#include "system.h"
#include<string.h>
#include <mafp_sensor.h>

static S32 synocFsmGetFrame(S32 ch,U8 err_flag);
static void synocFsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf);
static U16 synocFrameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff);
static void synocFsmVfyPwd(void);
static void synocFsmGetImage(void);
static void synocFsmGenChar(void);
static void synocFsmRegModel(void);
static void synocFsmStoreChar(void);
static void synocFsmDeleteChar(void);
static void synocFsmEmpty(void);
static void synocFsmSearch(void);
static void synocFsmGetImageForOptical(void);
static void synocFsmLoadChar(void);
static void synocFsmSleep(void);
static void synocFsmGetEnrollImage(void);
static void recvTimeoutProcess(void);

static U8 Synoc_Send_Buff[512];
static U8 Synoc_Recv_Buff[512];
#define NREC_TIMEOUT (1*1000*1000)
static U32 recv_len = 0;
static	U16 nold_RecNum=0;
volatile static  U32 nrec_TimeOut=0;





//0 --> ok    !0-->fail
static U8 chkRec(U8 *buff)
{

	U16 rcCHK=0,nwCHK=0,len,i;

	len = buff[SYNOC_FRAME_LEN_HIGH] * 256 + buff[SYNOC_FRAME_LEN_LOW];
	for(i = 6; i < 7+len;i++)
	{
		nwCHK += (buff[i]&0xff);
	}

	rcCHK = buff[SYNOC_FRAME_LEN_HIGH+len]*256+ buff[SYNOC_FRAME_LEN_LOW+len];

	if(nwCHK != rcCHK)
		return 1;
	else
		return 0;
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


static S32 synocFsmGetFrame(S32 ch,U8 err_flag)
{
	U32 device_addr,fdevice_addr;
	U16 frame_len=0;
	U8 pid=0;
	U8 flag;
	if (recv_len == 0)
	{
		if ( ch == (SYNOC_FRAME_START>>8) )
		{
			Synoc_Recv_Buff[recv_len++] = ch;
		}
	}
	else if(recv_len == 1)
	{
		if ( ch == (SYNOC_FRAME_START&0x00ff) )
		{
			Synoc_Recv_Buff[recv_len++] = ch;
		}
		else
		{
			recv_len=0;
			return 0;
		}
	}
	else
	{
		Synoc_Recv_Buff[recv_len++] = ch;
		if(recv_len == 6)
		{
			device_addr=( ( Synoc_Recv_Buff[2]<<8)+ Synoc_Recv_Buff[3]);
			device_addr=( ( device_addr<<16)+( Synoc_Recv_Buff[4]<<8)+ Synoc_Recv_Buff[5]);

			fdevice_addr = ((U32)(Synoc_Sys_ParaTag.device_addr[0] & 0xffff) << 16) + (Synoc_Sys_ParaTag.device_addr[1] & 0xffff) ;


			// 模块地址在flash中存储
			if( device_addr!=fdevice_addr)
			{
				recv_len=0;
				flag=SYNOC_PS_DEVICE_ADDR_ERR;
				if(err_flag)
					synocFsmSendFrame(SYNOC_PID_ACK ,0x01,&flag);
				return 0;
			}

		}
		else if(recv_len >= SYNOC_FRAME_STA_LEN)
		{

			frame_len=( Synoc_Recv_Buff[SYNOC_FRAME_LEN_HIGH ]);
			frame_len<<=8;
			frame_len|=( Synoc_Recv_Buff[SYNOC_FRAME_LEN_LOW ]);

			if(frame_len>SYNOC_FRAME_MOST_LEN)
			{
				recv_len=0;
				flag=SYNOC_PS_RECV_ERR;
				if(err_flag)
					synocFsmSendFrame(SYNOC_PID_ACK ,0x01,&flag);
				return 0;
			}

			frame_len+=SYNOC_FRAME_STA_LEN;

			if(recv_len >=frame_len)
			{
				recv_len=0;
				if(!chkRec(Synoc_Recv_Buff))
				{
					//无校验错误
					pid=Synoc_Recv_Buff[FRAME_PID];   //标识
					if( (pid==SYNOC_PID_COMMAND) ||  (pid == SYNOC_PID_DATA )|| (pid==SYNOC_PID_LAST_DATA ))
					{

						//uartDisableReceive();
						return frame_len;
					}
					else
					{
						flag=SYNOC_PS_RECV_ERR;
						if(err_flag)
							synocFsmSendFrame(SYNOC_PID_ACK ,0x01,&flag);
						return 0;
					}
				}
				else
				{
					//校验错误
					flag = SYNOC_PS_COMM_ERR;
					if(err_flag)
						synocFsmSendFrame(SYNOC_PID_ACK ,0x01,&flag);
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
static U16 synocFrameSetPack(U16 _data_len,U8* _data_buf,U8 _type,U8 * _send_buff)
{

	U16 nwCHK = 0,i;
	if(_send_buff&&_data_buf)   //参数检测
	{
		/*make packet head*/
		_send_buff[0] = (SYNOC_FRAME_START>>8);
		_send_buff[1] = (SYNOC_FRAME_START &0x00ff);
		_send_buff[2] = (Synoc_Sys_ParaTag.device_addr[0]>>8)&0xff;
		_send_buff[3] =  Synoc_Sys_ParaTag.device_addr[0]&0xff;
		_send_buff[4] = (Synoc_Sys_ParaTag.device_addr[1]>>8)&0xff;
		_send_buff[5] =  Synoc_Sys_ParaTag.device_addr[1]&0xff;
		_send_buff[6] = _type;
		_send_buff[7] = ((_data_len+SYNOC_FRAME_CRC) >> 8) & 0xff;
		_send_buff[8] = (_data_len+SYNOC_FRAME_CRC) & 0xff;

		memcpy(&Synoc_Send_Buff[SYNOC_FRAME_STA_LEN],_data_buf,_data_len);


		for(i = 6; i < 7+(_data_len+SYNOC_FRAME_CRC);i++)
		{
			nwCHK += (_send_buff[i]&0xff);
		}

		_send_buff[7+(_data_len+SYNOC_FRAME_CRC)] = nwCHK/256;
		_send_buff[8+(_data_len+SYNOC_FRAME_CRC)] = nwCHK%256;


		return (_data_len+SYNOC_FRAME_CRC+SYNOC_FRAME_STA_LEN);
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
static void synocFsmSendFrame(U8 _ack,U16 _data_len,U8 *_data_buf)
{
	U16 len;
	if(_data_buf)
	{
		len=synocFrameSetPack(_data_len,_data_buf,_ack,Synoc_Send_Buff);
		uartSendBuff(Synoc_Send_Buff,len);
	}
}




static void synocFsmVfyPwd(void)
{
	U8 tmp_buf[1]={0};
	synocFsmSendFrame(PID_ACK ,0x01,&tmp_buf[0]);
}

static void synocFsmGetEnrollImage(void)
{
	synocFsmGetImage();
}

static void synocFsmGetImage(void)
{
	S32 ret;
	U8 flag=SYNOC_PS_NO_FINGER;
	ret=mafp_capture_image();
	if(ret==0)
		flag=SYNOC_PS_OK;
	else if(ret == ERROR_GET_IMAGE )
		flag= SYNOC_PS_GET_IMG_ERR;
	else
		flag= SYNOC_PS_NO_FINGER;

	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}

static void synocFsmGenChar(void)
{
	U8 buffer_id,flag;
	S32 ret;
	buffer_id=Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+1];
	ret=mafp_extract_characteristic(buffer_id-1);
	/*
	 * ERROR_IVALID_IMAGE
	 * ERROR_BAD_IMAGE
	 * 0-->OK
	 *
	 **/
	if(ret == 0)
		flag=SYNOC_PS_OK;
	else if(ret == ERROR_BAD_IMAGE)
		flag=SYNOC_PS_LITTLE_FEATURE;
	else
		flag= SYNOC_PS_INVALID_IMAGE;

	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}



static void synocFsmRegModel(void)
{
	U8 flag=SYNOC_PS_OK;
	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}

static void synocFsmStoreChar(void)
{
	U16 enroll_id=Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+2]*256+Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+3];
	U8 flag=SYNOC_PS_FLASH_ERR;
	S32 ret;
	if(enroll_id>=MAX_ID)
	{
		flag=SYNOC_PS_ADDRESS_OVER;
	}
	else if((mafp_get_enrollid_status(enroll_id))== 1)
	{
		flag=SYNOC_PS_ID_EXIST;
	}
	else
	{
		ret=mafp_enroll_save_template(enroll_id);
		/*
		 * ERROR_BAD_PARAMETERS
		 * ERROR_ENROLL_SAVE_TEMPLATE
		 * 0-->OK
		 * */
		if(ret == 0)
			flag=SYNOC_PS_OK;
		else
			flag=SYNOC_PS_FLASH_ERR;
	}
	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}


static void synocFsmDeleteChar(void)
{
	U16 start=Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+1]*256+Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+2];
	U16 nums =Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+3]*256+Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+4];
	U16 i;
	U8 flag=SYNOC_PS_OK;
	if((start+nums) <= MAX_ID)
	{
		for(i=0;i<nums;i++)
		{
			if (mafp_remove_enrollid(start+i) == 0)
				flag=SYNOC_PS_OK;
			else
			{
				flag=SYNOC_PS_DEL_TEMP_ERR;
				break;
			}
		}
	}

	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}



static void synocFsmEmpty(void)
{
	U8 flag=SYNOC_PS_OK;
	S32 ret=mafp_clear_all_enrollids();
	if(ret)
		flag = SYNOC_PS_CLEAR_TEMP_ERR;
	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}


static void synocFsmSearch(void)
{
	U8 buffer_id ,flag,tmp[5]={0};
	U16 start_page,nums;
	S32 ret;

	flag =  Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+1];
	start_page = (Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+2] & 0xff) ;
	start_page = (start_page << 8) | (Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+3] & 0xff);
	nums = Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+4] & 0xff;
	nums = (nums << 8) | (Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+5] & 0xff);

	tmp[0]=SYNOC_PS_NOT_SEARCHED;
	if(flag==0x01)
		buffer_id=0;
	else
		buffer_id=1;

	ret=mafp_match_range(buffer_id,start_page,start_page+nums-1);
	/*
	 *
	 * ERROR_BAD_PARAMETER
	 * ERROR_AUTH_ALGO_FAIL
	 * ERROR_AUTH_NO_MATCH
	 * match_fid
	 * */

	if(ret>=0)
	{
		tmp[0]=SYNOC_PS_OK;
		tmp[1]=(ret>>8)&0xff;
		tmp[2]=ret&0xff;
		tmp[3]=0x00;
		tmp[4]=0xfa;
	}

	synocFsmSendFrame(PID_ACK ,0x05,&tmp[0]);

}

static void synocFsmValidTempltenum(void)
{
	U8 tmp[3]={0};
	S32 num;
	tmp[0]=SYNOC_PS_OK;

	num=mafp_get_used_enrollids();
	tmp[1]=num>>8;
	tmp[2]=num;
	synocFsmSendFrame(PID_ACK ,0x03,&tmp[0]);

}





//
////CMD_READ_INDEX_TABLE
static void SynocFsmReadIndexTable(void)
{
	U8 tmp_buf[34]={0};
	U16 i;
	U8 page= Synoc_Recv_Buff[FRAME_STA_LEN+1];
	memset(Index_Tab,0,sizeof(Index_Tab));
	tmp_buf[0]=PS_OK;

	if( /*(page>=0) &&*/ (page<4))
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

	memcpy(&tmp_buf[1],Index_Tab[page]+0,sizeof(Index_Tab)/4);
	synocFsmSendFrame(PID_ACK ,0x21,&tmp_buf[0]);
}


static void synocFsmGetImageForOptical(void)
{
	S32 ret;
	U8 flag;
	ret=mafp_capture_image();
	if(ret==0)
		flag=SYNOC_PS_OK;
	else if(ret == ERROR_GET_IMAGE )
		flag= SYNOC_PS_GET_IMG_ERR;
	else
		flag= SYNOC_PS_NO_FINGER;

	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}



static void synocFsmOnLight(void)
{
	U8 flag=SYNOC_PS_OK;
	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}


static void synocFsmOffLight(void)
{
	synocFsmOnLight();
}


static void synocFsmSleep(void)
{
	U8 flag=SYNOC_PS_OK;
	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}



static void synocFsmLoadChar(void)
{
	U16 id;
	U8 flag=SYNOC_PS_OK;
	S32 ret;
	id = (Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+2] & 0xff) ;
	id = (id << 8) | (Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN+3] & 0xff);

	ret=mafp_get_enrollid_status(id);

	if(ret == 0)
	{
		flag =SYNOC_PS_READ_ERR;
	}
	else if(ret ==1)
	{
		flag =SYNOC_PS_OK ;
	}
	else
	{
		flag=SYNOC_PS_ADDRESS_OVER;
	}

	synocFsmSendFrame(PID_ACK ,0x01,&flag);
}


static void synocExcFunc(_SynocCmd cmd)
{
	switch(cmd)
	{
	case CMD_VFY_PWD:
		synocFsmVfyPwd();
		break;
	case CMD_GET_ENROLL_IMAGE:
		synocFsmGetEnrollImage();
		break;
	case CMD_GEN_CHAR:
		synocFsmGenChar();
		break;
	case CMD_REG_MODEL:
		synocFsmRegModel();
		break;
	case CMD_STORE_CHAR:
		synocFsmStoreChar();
		break;
	case CMD_DELETE_CHAR:
		synocFsmDeleteChar();
		break;
	case CMD_EMPTY:
		synocFsmEmpty();
		break;
	case CMD_GET_IMAGE:
		synocFsmGetImage();
		break;
	case CMD_SEARCH:
		synocFsmSearch();
		break;
	case CMD_LOAD_CHAR:
		synocFsmLoadChar();
		break;
	case CMD_VALID_TEMPLTENUM:
		synocFsmValidTempltenum();
		break;
	case CMD_READ_INDEX_TABLE:
		SynocFsmReadIndexTable();
		break;
	case CMD_GET_IMAGE_FOR_OPTICAL:
		synocFsmGetImageForOptical();
		break;

	case CMD_GET_ON_LIGHT:
		synocFsmOnLight();
		break;
	case CMD_GET_OFF_LIGHT:
		synocFsmOffLight();
		break;

	case CMD_SLEEP:
		synocFsmSleep();
		break;
	default:
		break;
	}

}





void synocFsmTask(S32 ch)
{
	U8 command;
	recvTimeoutProcess();
	if (ch >= 0)
	{
		if (synocFsmGetFrame(ch,0))
		{
			if(SYNOC_PID_COMMAND ==Synoc_Recv_Buff[SYNOC_FRAME_PID])
			{
				command = Synoc_Recv_Buff[SYNOC_FRAME_STA_LEN];      //得到命令
				uartDisableReceive();
				synocExcFunc((_SynocCmd)command);
				uartEnableReceive();
			}
		}
	}
}




