//
//	algApi.h
//
//	
//	by LYY 2017-04-20
//

#ifndef ALGAPI_H_LYY_
#define ALGAPI_H_LYY_

#include "sysCfg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int imgExp[ EXP_SZ / 4 ];
	int setBuf[ SET_SZ ];
	int blockBuf[ BLOCK_SZ ];
	int adaptProcBuf[ SET_SZ * 2 ];
} extractBuf;

// extractBuf = imgExp + setBuf + blockBuf + adaptProcBuf
// EXP_SZ / 4 + SET_SZ + BLOCK_SZ + SET_SZ * 2
int algo_extract( int *imgSrc, int *temp, extractBuf *procBuf );

typedef struct {
	int matchTransBuf[ TRANS_FINE_MAX_CNT * 4 ];
	int matchProcBuf[ UNWRAP_MAP_SZ + 512 ]; // 3600 - 45
} matchBuf;

int fastMatch( int* tplMatch, int* tplEnroll, int *procBuf );

int accuracyMatch( int* tplMatch, int* tplEnroll, int transCnt, int* transOut, int *procBuf, int lvl );

int matchTpl( int* tplMatch, int* tplEnroll, int* transVec, int *procBuf );

#ifdef __cplusplus
}
#endif

#endif
