//
//	miscFunc.h
//
//	smoothOriFreq
//	
//	by LYY 2016-10-24
//

#include "miscFunc.h"
#include "geometry.h"
#include "algApi.h"
#include <stdlib.h>
#include <string.h>

#define fixedSin				sinMacroU8
#define fixedCos				cosFuncU8
#define fixedAtan2				atan2FuncU8
#define fixedAbsAtan2			absAtan2FuncU8

void smoothOriFreq( u8* listOri, u8* listFreq, u8* listWeight, int cnt, u8 *oriOut, u8 *freqOut )
{
}

void tensorInterpolationSoft( tensorBlock* tensorVec, u8 oriVal, int preH, int preW, int *imagOut, int *realOut )
{
}

void getPhaseFromTensorMapSoft( tensorBlock* blockHead, int blockH, int blockW, u8 oriVal, int coordH, int coordW, u32 *ampOut, u8 *phaOut )
{
}

void tensorInterpolationAlu( tensorBlock* tensorVec, u8 oriVal, int preH, int preW, int *imagOut, int *realOut )
{
}

int algo_extract( int *imgSrc, int *temp, extractBuf *procBuf )
{
	unsigned char src[8] = {0x1, 0x2, 0x3, 0x4};

	memcpy(temp, src, 8);

	return (MATCH_MINU_NUM << 16) | 1;
}


int matchTpl( int* tplMatch, int* tplEnroll, int* transVec, int *procBuf )
{
	int i = rand() % 300 + 1;
	
	transVec[0] = rand();
	transVec[1] = rand();
	transVec[2] = rand();
	transVec[3] = rand();

	return rand() % 10000 / i;
}


int fastMatch( int* tplMatch, int* tplEnroll, int *procBuf )
{
	return 1;
}


int accuracyMatch( int* tplMatch, int* tplEnroll, int transCnt, int* transVec, int *procBuf, int lvl)
{
	int i = rand() % 30 + 1;
	
	transVec[0] = rand();
	transVec[1] = rand();
	transVec[2] = rand();
	transVec[3] = rand();

	return rand() % 10000 / i;
}
