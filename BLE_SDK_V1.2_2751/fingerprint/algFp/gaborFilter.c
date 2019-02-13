//
//	gaborFilter.c
//
//	compressed Gabor filter blank : S8 gaborFilterBlankS8[ 16K ]
//	related Gabor filter function : gaborFilterALUS32 / gaborFilterSoftS32
//	if filterID is different : isDifFilterID
//
//	by LYY 2016-05-03
//

#include "gaborFilter.h"


const s8 gaborFilterBlankS8[ 16*1024 ] = { 0 };

// 128 Bytes
const u8 freqIDTable[] = {0, 1};

#if PLATFORM_TYPE == PLATFORM_ALU
void gaborFilterALUS32( u8* subImgHead, u8 ori, u8 freq, s32* imagVal, s32* realVal )
{
}

#elif PLATFORM_TYPE == PLATFORM_SOFT
void gaborFilterSoftS32(u8* subImgHead, u8 ori, u8 freq, s32* imagVal, s32* realVal )
{
}

#endif

u32 isDifFilterID(u8 oldOri, u8 oldFreq, u8 newOri, u8 newFreq)
{
		return 0;
}
