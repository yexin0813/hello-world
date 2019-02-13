/*
 * yale.c
 *
 *  Created on: 2018Äê5ÔÂ11ÈÕ
 *      Author: li
 */




#include"yale.h"
#include "bsp/Uart.h"
#include <mafp_sensor.h>
#include "bsp/Debug.h"
#include "bsp/Timer.h"
#include"system.h"




#define MODULE_EVENT (0xa0)
#define MODULE_ACK   (0xa1)
#define MODULE_NACK  (0xa2)

#define LOCK_EVENT (0xb0)
#define LOCK_ACK   (0xb1)
#define LOCK_NACK  (0xb2)



#define REQUESR_TO_START_MODULE_DEVICE_INFOMATION_EXCHANGE   (0x44)
#define REQUESR_MODULE_DEVICE_INFOMATION   (0x44)
#define ENCRYPTED_MESSAGE                  (0xe1)
#define ENCRYPTED_KEY_EXCHANGE             (0xe2)
#define NON_ENCRYPTED_MESSAGE              (0xe3)


#define RC_STATUS_OK       (0x00)
#define RC_PACKET_ERR_     (0x01)
#define RC_CHECKSUM_ERR    (0x02)
#define RC_RANGE_ERR       (0x14)
#define RC_SLOT_NOT_EMPTY  (0x11)
#define RC_BUSY            (0x1e)
#define RC_FUNCTION_FAILED (0x1f)

#define NONE_RF_MODULE (0x70)
#define LOCK_MICOM     (0x30)


#define YALE_FRAME_EVENT_OFF         (0x00)
#define YALE_FRAME_SOURCE_OFF        (0x01)
#define YALE_FRAME_COUNT_OFF         (0x02)
#define YALE_FRAME_LENGTHOFVALID_OFF (0x03)
#define YALE_FRAME_DATA_OFF          (0x04)
#define YALE_FRAME_CHECK_OFF         (0x14)
#define YALE_FRAME_LEN               (0x15)



#define RC_STATUS_IDLE (0x00)
#define RC_STATUS_BUSY (0x20)
#define RC_STATUS_PROCESS (0x40)


#define MSG_NO_FOR_FINGER  (0x00)
#define MSG_FIRST_FINGER   (0x01)
#define MSG_SECOND_FINGER  (0x02)
#define MSG_THRID_FINGER   (0x03)
#define MSG_FOURTH_FINGER  (0x04)
#define MSG_FIFTH_FINGER   (0x05)
#define MSG_SIXTH_FINGER   (0x06)
#define MSG_SEVENTH_FINGER (0x07)
#define MSG_IMAGE_ERR      (0x08)



#define NO_RESULT             (0x00)
#define ENROLL_RESULT_SUCCESS (0x01)
#define VERIFY_RESULT_SUCCESS (0x02)
#define DELETE_RESULT_SUCCESS (0x03)
#define ENROLL_RESULT_FAILED  (0x81)
#define VERIFY_RESULT_FAILED  (0x82)
#define DELETE_RESULT_FAILED  (0x83)


#define IS_YALE_FRAME_EVENT(THIS) (((THIS) == REQUESR_MODULE_DEVICE_INFOMATION)  || \
				   ((THIS) == ENCRYPTED_MESSAGE)                  || \
				   ((THIS) == ENCRYPTED_KEY_EXCHANGE)             || \
				   ((THIS) == NON_ENCRYPTED_MESSAGE)              )


#define IS_YALE_FRAME_SOURCE(THIS) (((THIS) == LOCK_EVENT )   ||	\
				    ((THIS) == LOCK_ACK )      ||	\
				    ((THIS) == LOCK_NACK)      )



struct Result
{
	U8 busy_status;
	U8 result_data;
}YaleResult;


typedef enum
{
	STATE_IDLE=0x00,
	STATE_ENROLL,
	STATE_MATCH,
	STATE_DELETE,
	STATE_LISTALL,
	STATE_SETSECURITY_LEVEL=0x06,
	STATE_GET_STATUS,
	STATE_GET_RESULT,
	STATE_SCAN_CANCLE,
	STATE_GET_VERSION,
	STATE_PRODUCT_SN
}_YaleFsmState;


typedef struct
{
	U8 match_id;
	U32 match_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 press_times;
	U8 result;
}_MatchTag;


typedef struct
{
	U8 enroll_id;
	U8 press_times;
	U8 press_max_times;
	U32 enroll_timeout;
	U32 (*timerElapsedUs)(void);
	void (*timerRestart)(void);
	U8 result;
}_EnrollTag;


#pragma pack(1)
typedef struct
{
	U8 event;
	U8 source;
	U8 count;
	U8 lengthOfValid;
	U8 data[16];
	U8 check;
}YaleFrameStruct;
#pragma pack()


typedef struct
{

	_YaleFsmState state,old_state;  //ÃüÁî×´Ì¬»úµÄ×´Ì¬

	enum
	{
		MATCH_STATE_IDLE,
		MATCH_STATE_CAPTURE,
		MATCH_STATE_COMPUTE,
		MATCH_STATE_TIMEOUT,
	}match_state; //Æ¥Åä×´Ì¬»úµÄ×´Ì¬

	enum
	{
		ENROLL_STATE_IDLE,
		ENROLL_STATE_CAPTURE,
		ENROLL_STATE_COMPUTE,
		ENROLL_STATE_LEAVE,
		ENROLL_STATE_TIMEOUT
	}enroll_state; //×¢²á×´Ì¬»úµÄ×´Ì¬

	enum
	{
		ACK=MODULE_ACK,
		NACK=MODULE_EVENT
	}is_ack;

}_YaleFsmTag;

#define YALE_MATCH_TIMEOUT_DEFAULT           (4 * 1000 * 1000)
#define YALE_ENROLL_PRESS_TIMES_DEFAULT      8  //Ä¬ÈÏ×¢²á°´Ñ¹´ÎÊý
#define YALE_ENROLL_TIMEOUT_DEFAULT          (4 * 1000 * 1000)


static U8 Yale_Recv_Buff[64]={0};
static YaleFrameStruct Recv_Frame={0};
static _YaleFsmTag Fsm_Tag={0};
U8 EncryptionKeyDefault[16]={'A','S','S','A','A','B','L','O','Y','K','R','I','r','e','v','o'};
U8 EncryptionKey[16]={0};
static _MatchTag Match_Tag={0};
static _EnrollTag Enroll_Tag={0};
#define NREC_TIMEOUT (1*1000*1000)
static U32 recv_len = 0;
static	U16 nold_RecNum=0;
volatile static  U32 nrec_TimeOut=0;



static S32 yaleFsmGetFrame(S32 ch,U8 err_flag);
static void yaleFsmPackFrame(U8 event,U8 source,const U8 * const data,U8 data_len,YaleFrameStruct * const frame);
static void yaleFsmSendFrame(U8 event,U8 source,const U8 * const data,U8 data_len);
static U8 getYaleStatus(void);
static void setYaleFsmState(_YaleFsmState state);
static void setYaleResult(U8 busy_status,U8 result_data);
static _YaleFsmState getYaleFsmState(void);
static void yaleFsmConnectLockAndModule(void);
static void yaleFsmEncryptedKey(void);
static void yaleFsmListAllFinger(void);
static void yaleFsmListAllFingerTask(void);
static void yaleFsmSetSecurityLevel(void);
static void yaleFsmSetSecurityLevelTask(void);
static void yaleFsmGetStatus(void);
static void yaleFsmGetResult(void);
static void initMatchTag(void);
static void yaleFsmMatch(void);
static void sendMatchFrame(void);
static void yaleFsmMatchTask(void);
static void yaleFsmExcDataFunc(U8 cmd);
static void yaleFsmExcDataTask(void);
static void yaleFsmExcEventFunc( YaleFrameStruct const *const frame );
static void yaleFsmDelete(void);
static void yaleFsmDeleteTask(void);
static void yaleFsmEnroll(void);
static void yaleFsmEnrollTask(void);
static void recvTimeoutProcess(void);

static U8 chk(U8 *buff,S32 buff_len)
{
	S32 i;
	U8 check=0;
	for(i=0;i<buff_len;i++)
		check^=buff[i];
	return check;
}

void yaleFsmPackFrame(U8 event,U8 source,const U8 * const data,U8 data_len,YaleFrameStruct *const  frame)
{
	U8 i;
	if( (data==NULL) || (data_len> 16) ||(data_len ==0)||(frame==NULL) )
		return ;

	memcpy(frame->data,data,data_len);
	frame->event=event;
	frame->source=source;
	frame->lengthOfValid=data_len;
	frame->count=Recv_Frame.count;

	if(event == ENCRYPTED_MESSAGE)
	{
		for(i=0;i<16;i++)
			frame->data[i]=(frame->data[i]^EncryptionKey[i])^frame->count;
	}

	frame->check=chk((U8*)frame,YALE_FRAME_LEN-1);
}


static void yaleFsmSendFrame(U8 event,U8 source,const U8 * const data,U8 data_len)
{
	YaleFrameStruct frame={0};
	yaleFsmPackFrame(event,source,data,data_len,&frame);
	uartSendBuff( (U8*)&frame,sizeof(frame) );
	Fsm_Tag.is_ack=NACK;
}


static void recvTimeoutProcess(void)
{
	nrec_TimeOut++;
	/*ÅÐ¶Ï³¬Ê±¹¤×÷*/
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
	else//ÅÐ¶Ï³¬Ê±
	{
		nrec_TimeOut++;
		if(nrec_TimeOut>NREC_TIMEOUT)
		{
			recv_len=0;
		}
	}
}


static S32 yaleFsmGetFrame(S32 ch,U8 err_flag)
{

	S32 i=0;
	if (recv_len == 0)
	{
		if ( IS_YALE_FRAME_EVENT(ch) )
		{
			Yale_Recv_Buff[recv_len++] = ch;
		}
	}
	else if(recv_len == 1)
	{
		if (IS_YALE_FRAME_SOURCE(ch))
		{
			Yale_Recv_Buff[recv_len++] = ch;
		}
		else
		{
			recv_len=0;
			return 0;
		}
	}
	else
	{
		Yale_Recv_Buff[recv_len++] = ch;
		if(recv_len >= YALE_FRAME_LEN)
		{

			recv_len=0;
			if( chk((U8*)&Yale_Recv_Buff,YALE_FRAME_LEN-1) ==  Yale_Recv_Buff[YALE_FRAME_LEN-1]  )
			{
			
				Recv_Frame.event=Yale_Recv_Buff[YALE_FRAME_EVENT_OFF];
				Recv_Frame.source=Yale_Recv_Buff[YALE_FRAME_SOURCE_OFF];
				Recv_Frame.count=Yale_Recv_Buff[YALE_FRAME_COUNT_OFF];
				Recv_Frame.lengthOfValid=Yale_Recv_Buff[YALE_FRAME_LENGTHOFVALID_OFF];
				memcpy(Recv_Frame.data,&Yale_Recv_Buff[YALE_FRAME_DATA_OFF],sizeof(EncryptionKey));
				
				if(Recv_Frame.event == ENCRYPTED_MESSAGE)
				{
					for(i=0;i<16;i++)
					{
						Recv_Frame.data[i]=(Yale_Recv_Buff[i+YALE_FRAME_DATA_OFF]^Recv_Frame.count)^EncryptionKey[i];
					}
				}
				
				Recv_Frame.check=Yale_Recv_Buff[YALE_FRAME_CHECK_OFF];

				return YALE_FRAME_LEN;
			}
			else
				return 0;

		}

	}
	return 0;
}

static U8 getYaleStatus(void)
{
	U8 status;
	if(Fsm_Tag.state)
		status= RC_STATUS_PROCESS;
	else
		status= RC_STATUS_IDLE;
	return status;
}


static void setYaleFsmState(_YaleFsmState state)
{
	Fsm_Tag.state=state;
	Fsm_Tag.old_state=Fsm_Tag.state;
	if(Fsm_Tag.state == STATE_MATCH)
		Fsm_Tag.match_state=MATCH_STATE_CAPTURE;
}



static void setYaleResult(U8 busy_status,U8 result_data)
{
	YaleResult.busy_status=busy_status;
	YaleResult.result_data=result_data;
}

static _YaleFsmState getYaleFsmState(void)
{
	return Fsm_Tag.state;
}


static void yaleFsmConnectLockAndModule(void)
{
	U8 data[3]={0};
	if( (Recv_Frame.event!=REQUESR_TO_START_MODULE_DEVICE_INFOMATION_EXCHANGE)||(Recv_Frame.source!= LOCK_EVENT ))
		return;

	data[0]=NONE_RF_MODULE;
	data[1]=LOCK_MICOM;
	data[2]=0x01;

	yaleFsmSendFrame(Recv_Frame.event,MODULE_ACK,data,sizeof(data));

}

static void yaleFsmEncryptedKey(void)
{
	int i=0;
	if( (Recv_Frame.event!=ENCRYPTED_KEY_EXCHANGE)||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	
	for(i=0;i<16;i++)
	{
		Recv_Frame.data[i]=(Yale_Recv_Buff[i+YALE_FRAME_DATA_OFF]^Recv_Frame.count)^EncryptionKeyDefault[i];
	}
	memcpy(EncryptionKey,Recv_Frame.data,sizeof(EncryptionKey));
	writeYaleEncryptionKey(EncryptionKey,sizeof(EncryptionKey));
	yaleFsmSendFrame(Recv_Frame.event,MODULE_ACK,EncryptionKey,sizeof(EncryptionKey));
}



static void yaleFsmListAllFinger(void)
{
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	setYaleFsmState(STATE_LISTALL);
}


static void yaleFsmListAllFingerTask(void)
{
	U8 data[3]={0};
	if(getYaleFsmState()!=STATE_LISTALL)
		return;
	data[0]=0x84;
	data[1]=RC_STATUS_OK;
	data[2]=(U8)mafp_get_used_enrollids();
	setYaleFsmState(STATE_IDLE);
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
}



static void yaleFsmSetSecurityLevel(void)
{
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	setYaleFsmState(STATE_SETSECURITY_LEVEL);
}


static void yaleFsmSetSecurityLevelTask(void)
{
	U8 data[3]={0};
	if(getYaleFsmState()!=STATE_SETSECURITY_LEVEL)
		return;
	data[0]=0x86;
	data[1]=RC_STATUS_OK;
	data[2]=Recv_Frame.data[1];
	setYaleFsmState(STATE_IDLE);
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
}


static void yaleFsmGetStatus(void)
{
	U8 data[3]={0},state;
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	data[0]=0x87;
	data[1]=(RC_STATUS_OK|getYaleStatus());
	state=getYaleFsmState();
	data[2]=(state<<4)&0xf0;
	if(state == STATE_MATCH)
		data[2]|=(Match_Tag.press_times&0x0f);
	else if(state == STATE_ENROLL)
		data[2]|=(Enroll_Tag.press_times&0x0f);
	else
		data[2]|=MSG_NO_FOR_FINGER;
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
}




static void yaleFsmGetResult(void)
{
	U8 data[4]={0};
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	data[0]=0x88;
	data[1]=RC_STATUS_OK;
	data[2]=YaleResult.busy_status;
	data[3]=YaleResult.result_data;
	setYaleFsmState(STATE_IDLE);
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
}


static void yaleFsmScanCancle(void)
{
	U8 data[2];
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	data[0]=0x89;
	data[1]=RC_STATUS_OK;
	setYaleFsmState(STATE_IDLE);
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
}

static void yaleFsmDelete(void)
{
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;
	setYaleFsmState(STATE_DELETE);
}


static void yaleFsmDeleteTask(void)
{
	U8 data[2]={0},start,end,i,result;
	if(getYaleFsmState()!=STATE_DELETE)
		return;
	start=Recv_Frame.data[1];
	end=Recv_Frame.data[2];
	result=DELETE_RESULT_FAILED;
	if(start)
	{
		if(end <= MAX_ID)
		{
			for(i=start;i<end;i++)
			{
				if (mafp_remove_enrollid(i) == 0)
					result=DELETE_RESULT_SUCCESS;
				else
				{
					result=DELETE_RESULT_FAILED;
					break;
				}
			}
		}
		if(result == DELETE_RESULT_SUCCESS)
			setYaleResult(DELETE_RESULT_SUCCESS ,STATE_DELETE);
		else
			setYaleResult(DELETE_RESULT_FAILED ,0xff);
	}
	else
	{
		if(mafp_clear_all_enrollids() == 0)
			setYaleResult(DELETE_RESULT_SUCCESS ,STATE_DELETE);
	}

	data[0]=0x83;
	data[1]=RC_STATUS_OK;
	setYaleFsmState(STATE_IDLE);
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
	yaleFsmGetResult();
}


static void initMatchTag(void)
{
	Match_Tag.match_id=0xff;
	Match_Tag.match_timeout=YALE_MATCH_TIMEOUT_DEFAULT;
	Match_Tag.timerElapsedUs=timerElapsedUs;
	Match_Tag.timerRestart=timerRestart;
	Match_Tag.press_times=0;
}


static void yaleFsmMatch(void)
{
	U8 data[2]={0};
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;

	initMatchTag();

	data[0]=0x82;
	data[1]=RC_STATUS_OK;
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));
	setYaleFsmState(STATE_MATCH);
	Match_Tag.timerRestart();
}

static void sendMatchFrame(void)
{
	yaleFsmGetStatus();
	yaleFsmGetResult();
}

static void yaleFsmMatchTask(void)
{
	S32 ret;
	if (getYaleFsmState() != STATE_MATCH)
		return ;

	switch(Fsm_Tag.match_state)
	{
	case MATCH_STATE_IDLE:
		break;
	case MATCH_STATE_CAPTURE:

		if ((Match_Tag.match_timeout>0)&&(Match_Tag.timerElapsedUs() > Match_Tag.match_timeout))
		{
			Fsm_Tag.match_state = MATCH_STATE_TIMEOUT;
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
			Fsm_Tag.match_state = MATCH_STATE_COMPUTE;
		}
		else if(ret == ERROR_AUTH_NO_TOUCH)
		{
			Fsm_Tag.match_state = MATCH_STATE_CAPTURE;
		}
		else
		{
			Match_Tag.press_times=1;
			setYaleResult(VERIFY_RESULT_FAILED,0xff);
			sendMatchFrame();
			setYaleFsmState(STATE_IDLE);
			Fsm_Tag.match_state = MATCH_STATE_IDLE;
		}
		break;
	case MATCH_STATE_COMPUTE:
		Match_Tag.press_times=1;
		ret = mafp_authenticate(Match_Tag.match_id);

		/*
		 * ERROR_AUTH_NO_MOVE
		 * ERROR_AUTH_NO_MATCH
		 * ERROR_AUTH_ALGO_FAIL
		 * match_id
		 *
		 * */
		if(ret >= 0 )
			setYaleResult(VERIFY_RESULT_SUCCESS,(ret&0xff));
		else
			setYaleResult(VERIFY_RESULT_FAILED,0xff);
		mafp_post_authenticate();
		sendMatchFrame();
		setYaleFsmState(STATE_IDLE);
		Fsm_Tag.match_state = MATCH_STATE_IDLE;
		break;
	case MATCH_STATE_TIMEOUT:
		Match_Tag.press_times=1;
		Match_Tag.timerRestart();
		yaleFsmGetStatus();
		Fsm_Tag.match_state = MATCH_STATE_CAPTURE;
		break;
	}
}







static void initEnrollTag(void)
{
	Enroll_Tag.press_max_times=YALE_ENROLL_PRESS_TIMES_DEFAULT;
	Enroll_Tag.enroll_timeout=0;
	Enroll_Tag.timerRestart=timerRestart;
	Enroll_Tag.timerElapsedUs=timerElapsedUs;
	Enroll_Tag.enroll_id=0;
	Enroll_Tag.press_times=0;
}


static void yaleFsmEnroll(void)
{
	U8 data[2]={0},result=0,id;
	S32 ret;
	if( (Recv_Frame.event!=ENCRYPTED_MESSAGE )||(Recv_Frame.source!= LOCK_EVENT ))
		return;

	initEnrollTag();

	id=Recv_Frame.data[1];
	if(!id)
		id=mafp_get_used_enrollids();
	else
		id-=1;

	ret=mafp_get_enrollid_status(id);
	if( ret== 1)
		result=RC_SLOT_NOT_EMPTY;
	else if(ret == ERROR_BAD_PARAMETERS)
		result= RC_FUNCTION_FAILED;
	else
	{
		Enroll_Tag.enroll_id=id;
		setYaleFsmState(STATE_ENROLL);
		Enroll_Tag.timerRestart();
		Fsm_Tag.enroll_state=ENROLL_STATE_CAPTURE;
	}
	data[0]=0x81;
	data[1]=result;
	yaleFsmSendFrame(ENCRYPTED_MESSAGE,Fsm_Tag.is_ack,data,sizeof(data));

}


static void yaleFsmEnrollTask(void)
{
	S32 ret;
	switch(Fsm_Tag.enroll_state)
	{
	case ENROLL_STATE_IDLE:
		break;
	case ENROLL_STATE_CAPTURE:

		if((Enroll_Tag.enroll_timeout>0) && (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout))
		{
			Fsm_Tag.enroll_state = ENROLL_STATE_TIMEOUT;
			break;
		}

		ret = mafp_pre_enroll();
		/*
		 * ERROR_ENROLL_NO_TOUCH
		 * ERROR_BAD_IMAGE
		 * ERROR_GET_IMAGE
		 * 0-->ok
		 * */
		if (ret == 0)
		{
			//ok
			Fsm_Tag.enroll_state = ENROLL_STATE_COMPUTE;
		}
		else
		{
			Fsm_Tag.enroll_state = ENROLL_STATE_CAPTURE;
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
		Fsm_Tag.enroll_state = ENROLL_STATE_LEAVE;
		if( ret>0 )
		{
			Enroll_Tag.press_times=ret;
			yaleFsmGetStatus();
			//valid times
			if(ret == Enroll_Tag.press_max_times)
			{
				setYaleFsmState(STATE_IDLE);
				Fsm_Tag.enroll_state = ENROLL_STATE_IDLE;
				mafp_post_enroll();
				setYaleResult(ENROLL_RESULT_SUCCESS,Enroll_Tag.enroll_id+1);
				yaleFsmGetResult();
			}
		}
		else
		{
			Fsm_Tag.enroll_state = ENROLL_STATE_CAPTURE;
		}
		Enroll_Tag.timerRestart();
		break;
	case ENROLL_STATE_LEAVE:
		if (mafp_finger_leave() == 1)
		{
			Fsm_Tag.enroll_state = ENROLL_STATE_CAPTURE;
		}
		else if((Enroll_Tag.enroll_timeout>0) && (Enroll_Tag.timerElapsedUs() > Enroll_Tag.enroll_timeout))
		{
			Fsm_Tag.enroll_state = ENROLL_STATE_TIMEOUT;
		}
		break;
	case ENROLL_STATE_TIMEOUT:
		Enroll_Tag.timerRestart();
		Fsm_Tag.enroll_state = ENROLL_STATE_CAPTURE;
		break;
	}
}




static void yaleFsmExcDataFunc(U8 cmd)
{
	Fsm_Tag.is_ack=ACK;
	switch(cmd)
	{
	case STATE_ENROLL:
		yaleFsmEnroll();
		break;
		
	case STATE_MATCH:
		yaleFsmMatch();
		break;
		
	case STATE_DELETE:
		yaleFsmDelete();
		break;
		
	case STATE_LISTALL:
		yaleFsmListAllFinger();
		break;
		
	case STATE_SETSECURITY_LEVEL:
		yaleFsmSetSecurityLevel();
		break;
		
	case STATE_GET_STATUS:
		yaleFsmGetStatus();
		break;

	case STATE_GET_RESULT:
		yaleFsmGetResult();
		break;

	case STATE_SCAN_CANCLE:
		yaleFsmScanCancle();
		break;

	case STATE_GET_VERSION:
		break;

	case STATE_PRODUCT_SN:
		break;

	default:
		Fsm_Tag.is_ack=NACK;
		break;
	}
}



static void yaleFsmExcDataTask(void)
{
	switch(Fsm_Tag.state)
	{
	case STATE_IDLE:
	    	break;
	case STATE_ENROLL:
	    	yaleFsmEnrollTask();
	    	break;
	case STATE_MATCH:
	    	yaleFsmMatchTask();
	    	break;
	case STATE_DELETE:
	    	yaleFsmDeleteTask();
	    	break;
	case STATE_LISTALL:
	    	yaleFsmListAllFingerTask();
	    	break;
	case STATE_SETSECURITY_LEVEL:
	    	yaleFsmSetSecurityLevelTask();
	    	break;
	case STATE_GET_VERSION:
	    	break;
	default:break;
	}
}

static void yaleFsmExcEventFunc( YaleFrameStruct const *const frame )
{
	switch(frame->event)
	{
	case REQUESR_TO_START_MODULE_DEVICE_INFOMATION_EXCHANGE:
		yaleFsmConnectLockAndModule();
		break;

	case ENCRYPTED_KEY_EXCHANGE:
		yaleFsmEncryptedKey();
		break;

	case ENCRYPTED_MESSAGE:
		yaleFsmExcDataFunc(frame->data[0]);
		break;
	
	default:
		break;
	}
}


static void yaleFsmTaskInit(void)
{
	readYaleEncryptionKey(EncryptionKey,sizeof(EncryptionKey));
}


void yaleFsmTask(S32 ch)
{
	static U8 isFsmTaskInit=0;
	recvTimeoutProcess();
	if(!isFsmTaskInit)
	{
		yaleFsmTaskInit();
		isFsmTaskInit=1;
	}
	if(ch>=0)
	{
		if(yaleFsmGetFrame(ch,0))
			yaleFsmExcEventFunc(&Recv_Frame);
	}
	else
	{
		yaleFsmExcDataTask();

	}
}






