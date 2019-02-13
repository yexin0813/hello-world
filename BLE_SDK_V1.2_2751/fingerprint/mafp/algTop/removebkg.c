/*
 * removebkg.c
 *
 *  Created on: Oct 24, 2016
 *      Author: zhl
 */


#include "algorithm.h"
#include <string.h>


void rmvBkg_AFS121( u8* rawHead, u8* bkgHead, s16* srcHead )
{
	int adr;
	u8 *pRaw, *pBkg;
	s16 *pSrc;

	// 从后向前填写src, raw和src可使用同一头地址, buf长度不同
	pRaw = rawHead + (14400-8);
	pBkg = bkgHead + (14400-8);
	pSrc = srcHead + (14400-8);
	for(adr=0;adr<14400;adr+=8,pRaw-=8,pBkg-=8,pSrc-=8)
	{
		pSrc[7] = valList[(int)pRaw[7]]-valList[(int)pBkg[7]];
		pSrc[6] = valList[(int)pRaw[6]]-valList[(int)pBkg[6]];
		pSrc[5] = valList[(int)pRaw[5]]-valList[(int)pBkg[5]];
		pSrc[4] = valList[(int)pRaw[4]]-valList[(int)pBkg[4]];
		pSrc[3] = valList[(int)pRaw[3]]-valList[(int)pBkg[3]];
		pSrc[2] = valList[(int)pRaw[2]]-valList[(int)pBkg[2]];
		pSrc[1] = valList[(int)pRaw[1]]-valList[(int)pBkg[1]];
		pSrc[0] = valList[(int)pRaw[0]]-valList[(int)pBkg[0]];
	}
}

// pHistVal is of 511 int
void expandGrey( s16* srcHead, u8* expHead, int* pHistVal )
{
	int i,j,adr,disH,disW;
	int histID, tmp;
	int sumLow, sumHigh, lowTH=0, highTH=0, expandRate;
	s16 *pSrc;
	u8 *pExp;

	memset(pHistVal, 0, 511*sizeof(int)); // all 511 num
	pSrc = srcHead + 1210;
	for(i=10;i<110;i++)
	{
		for(j=10;j<110;j++, pSrc++)
		{
			disH = (i<<1) - 119;
			disW = (j<<1) - 119;
			if( disH*disH + disW*disW <= 10000 )
			{
				histID = ( (int)(*pSrc) + 1024 ) >> 2; // 0-510,all 511
				pHistVal[histID]++;
			}
		}
		pSrc += 20;
	}
	sumLow = 0;
	for(i=0;i<511;i++)
	{
		sumLow += pHistVal[i];
		if(sumLow>=1200)
		{
			lowTH = ((i-256)<<2);
			break;
		}
	}
	sumHigh = 0;
	for(i=510;i>=0;i--)
	{
		sumHigh += pHistVal[i];
		if(sumHigh>=400)
		{
			highTH = ((i-256)<<2);
			break;
		}
	}

	if ( highTH <= lowTH )
	{
		highTH = lowTH + 4;
	}

	expandRate = (160<<13) / (highTH-lowTH);

	pExp=expHead + 14392;
	pSrc=srcHead + 14392;
	for(adr=0; adr<14400; adr+=8, pExp-=8, pSrc-=8)
	{
		tmp = ( ( ((int)pSrc[7]-lowTH)*expandRate ) >> 13 ) + 72; // (1016+64)*160*2^13 < 2^31-1
		pExp[7] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[6]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[6] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[5]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[5] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[4]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[4] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[3]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[3] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[2]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[2] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[1]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[1] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );

		tmp = ( ( ((int)pSrc[0]-lowTH)*expandRate ) >> 13 ) + 72;
		pExp[0] = (u8)( (tmp<0) ? 0 : ( (tmp>255) ? 255 : tmp ) );
	}
}

void fp121_rmvBkg_Expand( u8* imgRawBuf, u8* imgBkgBuf, u8* imgExpBuf, s16* imgSrcBuf, int* procBuf )
{
	rmvBkg_AFS121( imgRawBuf, imgBkgBuf, imgSrcBuf );
	expandGrey( imgSrcBuf, imgExpBuf, procBuf );
}
