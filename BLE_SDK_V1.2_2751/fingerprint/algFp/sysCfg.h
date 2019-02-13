//
//	sysCfg.h
//
//	SYMBOL
//	FLATFPORM
//	TYPE
//	SENSOR
///	MATCH
//	DEBUG CTL
//
//	by LYY 2016-10-20
//

#ifndef SYSCFG_H_LYY_
#define SYSCFG_H_LYY_

// -------------------- SYMBOL --------------------

#define SUCCESS			0
#define FAIL			-1
#define TRUE			1
#define FALSE			0

// -------------------- PLATFORM --------------------

#define PLATFORM_GENERAL			0
#define PLATFORM_SOFT				1
#define	PLATFORM_CCORE_ALU			2

#define	PLATFORM_TYPE	PLATFORM_SOFT

// -------------------- GENERAL --------------------

#define MAX(a,b)		( ((a)>(b)) ? (a) : (b) )
#define MIN(a,b)		( ((a)<(b)) ? (a) : (b) )
#define BOUND(x,a,b)	( ((x)<(a)) ? (a) : ( ((x)>(b)) ? (b) : (x) ) )
#define ABS(a)			( ((a)>0) ? (a) : -(a) )
#define ABS_DIF(a,b)	( ((a)>(b)) ? ((a)-(b)) : ((b)-(a)) )
#define UNWRAP(a,b)		( (s8)((a)-(b)) + (b) )

// -------------------- TYPE --------------------

// all platform
typedef unsigned int			u32;
typedef signed int				s32;
typedef unsigned short int		u16;
typedef signed short int		s16;
typedef unsigned char			u8;
typedef signed char				s8;

// windows
// typedef unsigned __int32		u32;
// typedef signed __int32		s32;
// typedef unsigned __int16		u16;
// typedef signed __int16		s16;
// typedef unsigned __int8		u8;
// typedef signed __int8		s8;

// -------------------- SENSOR --------------------
#define SENSOR_ID  121

#define EXTRACT_BLOCK_NUM 0

#if SENSOR_ID == 160
	#define IMG_H  160
	#define IMG_W  160
	#define UPDATE_DISTANCE		30
	#define MATCH_MINU_NUM		3
#elif SENSOR_ID == 120 || SENSOR_ID == 121
	#define IMG_H  120
	#define IMG_W  120
	#define UPDATE_DISTANCE		20
	#define MATCH_MINU_NUM		2
#elif SENSOR_ID == 80 || SENSOR_ID == 81 || SENSOR_ID == 82
	#define IMG_H  160
	#define IMG_W  80
	#define UPDATE_DISTANCE		20
	#define MATCH_MINU_NUM		2
#elif SENSOR_ID == 112 || SENSOR_ID == 96
	#define IMG_H  112
	#define IMG_W  96
	#define UPDATE_DISTANCE		20
	#define MATCH_MINU_NUM		2
#elif SENSOR_ID == 83
	#define IMG_H  128
	#define IMG_W  80
	#define UPDATE_DISTANCE		20
	#define MATCH_MINU_NUM		2
#endif

#define IMG_SZ						( IMG_H * IMG_W )

#define EXP_H						( IMG_H + 12 )
#define EXP_W						( IMG_W + 12 )
#define EXP_SZ						( EXP_H * EXP_W )

#define BLOCK_H						( IMG_H / 4 )
#define BLOCK_W						( IMG_W / 4 )
#define BLOCK_SZ					( BLOCK_H * BLOCK_W )

#define SET_H						( BLOCK_H - 1 )
#define SET_W						( BLOCK_W - 1 )
#define SET_SZ						( SET_H * SET_W )

// -------------------- MATCH --------------------

// for minutia extraction
#define MINU_MAX_CNT				30

// for template data structure
#define TEMPLATE_HEAD_LEN			16
#define TEMPLATE_MINU_LIST_LEN		( MINU_MAX_CNT * 8 )
#define TEMPLATE_BLOCK_MAP_H		BLOCK_H
#define TEMPLATE_BLOCK_MAP_W		BLOCK_W
#define TEMPLATE_BLOCK_MAP_SZ		( TEMPLATE_BLOCK_MAP_H * TEMPLATE_BLOCK_MAP_W )

#define TEMPLATE_SZ ( TEMPLATE_HEAD_LEN + TEMPLATE_MINU_LIST_LEN + TEMPLATE_BLOCK_MAP_SZ * 4 )

// for match
#define COUPLE_MAX_CNT				( MINU_MAX_CNT * (MINU_MAX_CNT-1) / 2 )

#define TRANS_RAW_MAX_CNT			64
#define TRANS_FINE_MAX_CNT			16

#define UNWRAP_MAP_H				( TEMPLATE_BLOCK_MAP_H + 2 )
#define UNWRAP_MAP_W				( TEMPLATE_BLOCK_MAP_W + 2 )
#define UNWRAP_MAP_SZ				( UNWRAP_MAP_H * UNWRAP_MAP_W )

// -------------------- DEBUG CTL --------------------

#define DBUG_EN						0

#if DBUG_EN==1
	#define TEST_EXTRACT				0
	#define TEST_LITERATION				0
	#define TEST_MATCH					1
	#include <stdio.h>
	#include <malloc.h>
	#include "visual.h"
	char fileNameTest[256];
#endif

#endif
