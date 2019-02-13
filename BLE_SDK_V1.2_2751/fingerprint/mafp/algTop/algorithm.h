/*
 * algorithm.h
 *
 *  Created on: Oct 24, 2016
 *      Author: zhl
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include "malib.h"
#include "afs121.h"
#include "mafp_defs.h"

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned char uint8;

extern const int valList[];

void fp121_rmvBkg_Expand( u8* imgRawBuf, u8* imgBkgBuf, u8* imgExpBuf, s16* imgSrcBuf, int* procBuf );
void fp121_tell_bkg(uint8_t* img_buf, int *score_bkg);

int calCorrFull(uint8 *imgBkg, uint8 *imgRaw);
void calCorrList(uint8 *img1, uint8 *img2, int *corrList);

int getTouchArea(uint8 *imgBkg, uint8 *imgRaw, int *corrListBkg);
int checkPress(uint8 *imgBkg, uint8 *imgRaw, int corrFullBkg);

#endif /* ALGORITHM_H_ */
