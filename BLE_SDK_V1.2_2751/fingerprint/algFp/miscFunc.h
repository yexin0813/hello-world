//
//	miscFunc.h
//
//	smoothOriFreq
//	
//	by LYY 2016-10-21
//

#ifndef MISCFUNC_H_LYY_
#define MISCFUNC_H_LYY_

#include "algType.h"
#include "geometry.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BKG_AMP_TH				8

typedef struct
{
	u8 pha;
	u8 amp;
	u8 ori;
	u8 freq;
} tensorBlock;

typedef struct
{
	u8 angle;
	s8 coordH;
	s8 coordW;
	u8 frequency;
	s8 difOriF2B;
	s8 difOriL2R;
	s8 phaseFL;
	s8 phaseFR;
} minuVector;

#define fixedGaborFilter		gaborFilterFuncS32
#define fixedSin				sinMacroU8
#define fixedCos				cosFuncU8
#define fixedAbsAtan2			absAtan2FuncU8
#define fixedAtan2				atan2FuncU8
#define fixedAbs				absFuncU8
#define absDifAngle				absDifAngleMacroU8
#define signedDifAngle			difAngleMacroS8

void smoothOriFreq( u8* listOri, u8* listFreq, u8* listWeight, int cnt, u8 *oriOut, u8 *freqOut );


void tensorInterpolationAlu( tensorBlock* tensorVec, u8 oriVal, int preH, int preW, int* imagVal, int* realVal );
void tensorInterpolationSoft( tensorBlock* tensorVec, u8 oriVal, int preH, int preW, int* imagVal, int* realVal );

#if PLATFORM_TYPE == PLATFORM_ALU

#define tensorInterpolation tensorInterpolationAlu

#elif PLATFORM_TYPE == PLATFORM_SOFT

#define tensorInterpolation tensorInterpolationSoft

#endif

#ifdef __cplusplus
}
#endif

#endif
