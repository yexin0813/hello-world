//
//	gaborFilter.h
//
//	compressed Gabor filter blank : s8 gaborFilterBlankS8[ 16K ]
//	related Gabor filter function : gaborFilterALUS32 / gaborFilterSoftS32
//	if filterID is different : isDifFilterID
//
//	by LYY 2016-05-03
//

#ifndef GABORFILTER_H_LYY_
#define GABORFILTER_H_LYY_

#include "algType.h"


#ifdef __cplusplus
extern "C" {
#endif

#define EXP_W 32
void gaborFilterALUS32( u8* subImgHead, u8 ori, u8 freq, s32* imagVal, s32* realVal);
void gaborFilterSoftS32( u8* subImgHead, u8 ori, u8 freq, s32* imagVal, s32* realVal );

#if PLATFORM_TYPE == PLATFORM_ALU

#define gaborFilterFuncS32 gaborFilterALUS32

#elif PLATFORM_TYPE == PLATFORM_SOFT

#define gaborFilterFuncS32 gaborFilterSoftS32
//	Gabor filter function
//		subImgHead, leftUp pixel pointer, use imgRowLen to step to next row;

#endif

// if filterID is different
u32 isDifFilterID(u8 oldOri, u8 oldFreq, u8 newOri, u8 newFreq);

#ifdef __cplusplus
}
#endif

#endif
