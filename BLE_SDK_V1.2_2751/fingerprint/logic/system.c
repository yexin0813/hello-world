/*
 * system.c
 *
 *  Created on: 2017年5月2日
 *      Author: li
 */
#include<system.h>
#include<string.h>
#include "m_flash.h"
#include"m_uart.h"
#include"logic/fsm.h"




static U8 paraGenCRC(U8 *dat,U32 len);
static void initParaTab(void);



//全局变量
U16 Frame_Data_Size;
U32 Gl_Password=0;
U8 Index_Tab[4][32];
U8 Baud_Rate=12;
U8 Device_Ready_Flag=0;
U32 Protocol;
_SysParaTag Sys_ParaTag;




//CRC8码表
const U8 CRC8Table[]=
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










/*********************************************************************************************************
 ** 函数名称:	ReadParaTab
 ** 功能描述:	从eflash中读取参数表
 **
 ** input: sys_tag-->参数表地址(传入传出参数),len-->长度
 ** return:0--> 成功    1-->失败
 ** output:void
 ** author:li
 ** data : 2017/05/04
 ***********************************************************************************************************/
U8 ReadParaTab(_SysParaTag * sys_tag,U32 len)
{

	if(sys_tag)
	{
		memcpy((U8*)sys_tag,(U8 *)(SYS_PARA_ADDR),len);
		return 0;
	}
	return  1;
}


/*********************************************************************************************************
 ** 函数名称:	ReadParaTab
 ** 功能描述:	将参数表写入到eflash中，然后再次更新到sys_tag开始的地址中
 **
 ** input: sys_tag-->参数表地址(传入传出参数),len-->长度
 ** return:0--> 成功    1-->失败
 ** output:void
 ** author:li
 ** data : 2017/05/04
 ***********************************************************************************************************/
U8 WriteParaTab(_SysParaTag * sys_tag,U32 len)
{
	if(sys_tag)
	{
		sys_tag->crc=paraGenCRC((U8*)sys_tag,PARA_CRC_LEN );
		
		eflash_write(SYS_PARA_ADDR, len, (U32 *)sys_tag);
		
		ReadParaTab(sys_tag,len);
		return 0;
	}
	return 1;
}


/*********************************************************************************************************
 ** 函数名称:	paraGenCRC
 ** 功能描述:	生成参数CRC校验值
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static U8 paraGenCRC(U8 *dat,U32 len)
{
	U8 crc8 = 0;
	while(len--){
		crc8 = CRC8Table[crc8 ^ *dat];
		dat++;
	}

	return crc8;
}




//填写版本信息
static U8 Version[8] = VERSION_NUM;


/*********************************************************************************************************
 ** 函数名称:InitParaTab
 ** 功能描述:	初始化参数表
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
static void initParaTab(void)
{
	memset(&Sys_ParaTag,0,sizeof(_SysParaTag));
	Sys_ParaTag.en_passwd=0;
	Sys_ParaTag.flag=SYS_CHANGE;
	Sys_ParaTag.pkt_size=3;
	Sys_ParaTag.baudrate=12;
	Sys_ParaTag.device_add=0xffffffff;
	Sys_ParaTag.en_dul_enroll=0;
	Sys_ParaTag.pwd=0;
	Sys_ParaTag.secur_level=1;
	memcpy(&Sys_ParaTag.version[0],&Version[0],sizeof(Version));
	Sys_ParaTag.index=MAX_ID;
	Sys_ParaTag.en_debug_print=0;
	Sys_ParaTag.protocol=PROTOCOL;
	Sys_ParaTag.crc=paraGenCRC((U8*)&Sys_ParaTag,PARA_CRC_LEN);
}



/*********************************************************************************************************
 ** 函数名称:InitDeviceInfo
 ** 功能描述:	初始化chip
 **
 ** input: void
 ** return:void
 ** output:void
 ** author:li
 ** date : 2017/05/04
 ***********************************************************************************************************/
void initDeviceInfo(void)
{
	U32 crc=0;
	ReadParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	if( Sys_ParaTag.flag!=SYS_CHANGE)
	{
		initParaTab();
		WriteParaTab(&Sys_ParaTag,sizeof(Sys_ParaTag));
	}


	Baud_Rate=Sys_ParaTag.baudrate;
	Protocol=Sys_ParaTag.protocol;
	crc=paraGenCRC((U8 *)&Sys_ParaTag,PARA_CRC_LEN);
	if(Sys_ParaTag.crc!=crc)
	{
		Device_Ready_Flag|=DEVICE_READY_SYS_PARA_CRC_FAILE;
		Baud_Rate=12;  //baudrate 115200
		Protocol=PROTOCOL_MICROARRAY ;
	}

	switch(Sys_ParaTag.pkt_size)
	{
		case 0: Frame_Data_Size=32; break;
		case 1: Frame_Data_Size=64; break;
		case 2: Frame_Data_Size=128; break;
		case 3: Frame_Data_Size=256; break;
		default:Frame_Data_Size=256;break;
	}

	if(Protocol==PROTOCOL_MICROARRAY_COMPATIBLE_SYNOCHIP)
	{
		initSynocDeviceInfo();
	}
	else if(Protocol==PROTOCOL_MICROARRAY_COMPATIBLE_YALE)
	{
		initYaleDeviceInfo();
	}

	return;
}


void initDeviceErase(void)
{
	eflash_page_erase(EFM0_MAIN_BASEADDR+CODE_SIZE+SYS_BRUN_CODE_TIME_SIZE);
}






//全局变量
_SynocSysParaTag Synoc_Sys_ParaTag;
U16 Synoc_Pkt_Size;


U8 readSynocParaTab(_SynocSysParaTag * sys_tag,U32 len)
{

	if(sys_tag)
	{
		memcpy((U8*)sys_tag,(U8 *)(SYNOC_PARA_ADDR),len);
		return 0;
	}
	return  1;
}


U8 writeSynocParaTab(_SynocSysParaTag * sys_tag,U32 len)
{
	if(sys_tag)
	{
		sys_tag->crc=paraGenCRC((U8*)sys_tag,SYNOC_PARA_CRC_LEN );
		eflash_write(SYNOC_PARA_ADDR, len, (U32*)sys_tag);
		readSynocParaTab(sys_tag,len);
		return 0;
	}
	return 1;
}


static void initSynocParaTab(void)
{
	memset((U8 *)&Synoc_Sys_ParaTag,0,sizeof(Synoc_Sys_ParaTag));
	Synoc_Sys_ParaTag.device_addr[0]=0xffff;
	Synoc_Sys_ParaTag.device_addr[1]=0xffff;
	Synoc_Sys_ParaTag.cfg_baudrate=6;
	Synoc_Sys_ParaTag.password[0]=0;
	Synoc_Sys_ParaTag.password[1]=0;
	Synoc_Sys_ParaTag.secur_level=2;
	Synoc_Sys_ParaTag.data_base_size=MAX_ID;
	Synoc_Sys_ParaTag.cfg_pkt_size=3;
	Synoc_Sys_ParaTag.ParaTabFlg=SYNOC_SYS_CHANGE;
	Synoc_Sys_ParaTag.crc=paraGenCRC((U8 *)&Synoc_Sys_ParaTag,SYNOC_PARA_CRC_LEN);
}


void initSynocDeviceInfo(void)
{
	U32 crc=0;
	readSynocParaTab(&Synoc_Sys_ParaTag,sizeof(_SynocSysParaTag));
	if( Synoc_Sys_ParaTag.ParaTabFlg!=SYNOC_SYS_CHANGE)
	{
		initSynocParaTab();
		writeSynocParaTab(&Synoc_Sys_ParaTag,sizeof(_SynocSysParaTag));
	}
	Baud_Rate=Synoc_Sys_ParaTag.cfg_baudrate;
	crc=paraGenCRC((U8 *)&Synoc_Sys_ParaTag,SYNOC_PARA_CRC_LEN);
	if(Synoc_Sys_ParaTag.crc!=crc)
	{
		Baud_Rate=6;  //baudrate 115200
	}

	switch(Synoc_Sys_ParaTag.cfg_pkt_size)
	{
		case 0: Synoc_Pkt_Size=32;  break;
		case 1: Synoc_Pkt_Size=64;  break;
		case 2: Synoc_Pkt_Size=128; break;
		case 3: Synoc_Pkt_Size=256; break;
		default:Synoc_Pkt_Size=256; break;
	}
	return;

}


void initYaleDeviceInfo(void)
{
	Baud_Rate=1;
}



U8 readYaleEncryptionKey(U8 * key,U8 len)
{

	if(key)
	{
		memcpy((U8*)key,(U8 *)(YALE_EKEY_ADDR),len);
		return 0;
	}
	return  1;
}


U8 writeYaleEncryptionKey(U8 * key,U8 len)
{
	if(key)
	{
		eflash_write(YALE_EKEY_ADDR, len, (U32 *)key);
		return 0;
	}
	return 1;
}



U8 readBurnCodeTime(U8 * data,U8 len)
{

	if(data)
	{
		memcpy((U8*)data,(U8 *)(BURN_CODE_TIME_ADDR),len);
		return 0;
	}
	return  1;
}

U8 writeBurnCodeTime(U8 * data,U8 len)
{

	if(data)
	{
		eflash_write(BURN_CODE_TIME_ADDR, len, (U32 *)data);
		return 0;
	}
	return  1;
}


