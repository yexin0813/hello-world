/*
 * afslib.h
 *
 *  Created on: 2014-11-29
 *      Author: lyy
 */

#ifndef AFSLIB_H_
#define AFSLIB_H_

#include "mafp_defs.h"


extern int afs_extract( int* imgSrc, int *tplCur, int *procBuf );
extern int afs_enroll( int fid, int *tpls, int *procBuf );
extern int afs_match( int fid, int *tpls, int *procBuf, int updateFlag );
extern int afs_checkMove( int *tpls, int *procBuf );

#endif /* AFSLIB_H_ */

