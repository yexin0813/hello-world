#include <string.h>
#include "sensor_sal.h"
#include "algApi.h"
#include "afs121.h"
#include "policy.h"
#include "sensor_identify.h"
#include "m_timer.h"


#define ALGO_IMAGE_UPLOAD_EN	0
#define ALGO_TIMER_TEST_EN	1



#define fixedAbs	absFuncU8
extern u32 absFuncU8( s32 yVal, s32 xVal );


int afs_extract( int* imgSrc, int *tplCur, int *procBuf )
{
	int minuCnt, blockCnt, rlt;

#if ALGO_IMAGE_UPLOAD_EN
	save_as_img("imgSrc.bmp", (u8*)imgSrc, IMAGE_W, IMAGE_H);
#endif

#if ALGO_TIMER_TEST_EN
	timerRestart();
#endif

	rlt = algo_extract( imgSrc, tplCur, (extractBuf *)procBuf );
	minuCnt = rlt >> 16;
	blockCnt = rlt & 0xffff;

#if ALGO_TIMER_TEST_EN
	LOGE( "extract time cost = %d us, %d minutias", timerElapsedUs(), ((u8*)tplCur)[2] );
#endif

	if ( minuCnt < MATCH_MINU_NUM || blockCnt < EXTRACT_BLOCK_NUM )
	{
		LOGE("%d minutias, %d blocks, %d%% of area %d x %d", minuCnt, blockCnt, blockCnt*100/BLOCK_SZ, IMG_H, IMG_W );
		return -1;
	}
	
#if ALGO_IMAGE_UPLOAD_EN
	save_as_img("imgExp.bmp", (u8*)(((extractBuf *)procBuf)->imgExp), IMAGE_W + 12, IMAGE_H + 12);
	save_as_img("tpl.bmp", (u8*)tplCur, TEMPLATE_SZ, 1);
#endif

	return 0;
}


int afs_enroll( int fid, int *stpls, int *procBuf )
{
	int tplAmount;
	int *tplCur = stpls;
	
	tplAmount = policy_get_subtpls_amount(fid);
	//LOGE("--> fid[%d], amount %d", fid, tplAmount);
	     
	if ( tplAmount < 0 || tplAmount >= TEMPLATE_NUM )
	{
		return -1;
	}

#if GRAY_CODE_VERIFICATION
	// Gray code chipset  checking
	if (fp121_gray_identify(0)) {
		LOGE("----------------------> identify failed.");
	
		// destroy the template data, if chip is a fake AFS121N
		memset(tplCur, 0x5A, TEMPLATE_SZ);
	} else {
		sensor_reset_mode();
	}
#endif

	SET_KEY_2();

	// save to flash
	if (policy_save_subtpl(fid, tplAmount++, (subtpl_t *)tplCur) < 0) {
		return -2;
	}
		
	return 10;

}


int afs_match( int fid, int *stpls, int *procBuf, int updateFlag )
{
	int i, tplAmount;
	int *tplCur = stpls;
	int transCnt, transVec[4];
	int distance, minDistance = 0x7fffffff, minDisID = -1;
	int matchScore, maxMatchScore = 0;
	int minUpdateTime=0x7fffffff, minUpdateID=0, minDisUpdateTime=0;
	int updateID = -1, upateTimes = 0xfffff;

	subtpl_t *subtpl = (subtpl_t *)stpls + 1;
	
	tplAmount = policy_get_subtpls_amount(fid);
	
	if ( tplAmount <= 0 || tplAmount > TEMPLATE_NUM )
	{
		return -1;
	}

	for ( i=0; i<tplAmount; i++ )
	{
		policy_get_subtpl_updatetimes(fid, i, &upateTimes);
		LOGD("--> fid %d, tplN0. %d, updateTime %d", fid, i, upateTimes );
		if ( upateTimes < minUpdateTime )
		{
			minUpdateTime = upateTimes;
			minUpdateID = i;
		}
	}

	LOGD("--> fid %d, Update lvl %d", fid, (minUpdateTime==0x7fffffff)?0:minUpdateTime );

	for ( i=0; i<tplAmount; i++ )
	{
		policy_load_subtpl_header(fid, i, subtpl);

#if ALGO_TIMER_TEST_EN
		timerRestart();
#endif
		transCnt = fastMatch( tplCur, (int*)subtpl, procBuf );
#if ALGO_TIMER_TEST_EN
		LOGE( "fastMatch time cost = %d us, %d x %d minutias", timerElapsedUs(), ((u8*)tplCur)[2], ((u8*)subtpl)[2] );
#endif

		if ( transCnt == 0 )
			continue;

		policy_load_subtpl_body(fid, i, subtpl);
		
#if ALGO_TIMER_TEST_EN
		timerRestart();
#endif
		
		matchScore = accuracyMatch( tplCur, (int*)subtpl, transCnt, transVec, procBuf, (minUpdateTime==0x7fffffff)?0:minUpdateTime );
		
#if ALGO_TIMER_TEST_EN
		LOGE( "accuracyMatch time cost = %d us", timerElapsedUs() );
#endif
		
		if ( matchScore > 0 )
		{
			distance = fixedAbs( transVec[1], transVec[2] );
			if ( distance < minDistance )
			{
				minDistance = distance;
				minDisID = i;
				minDisUpdateTime = subtpl->attr.update_times;
			}
		}
		if ( matchScore > maxMatchScore )
		{
			maxMatchScore = matchScore;
		}
	}

	LOGD("--> match score %d", maxMatchScore );

	if ( maxMatchScore >= ( 6000 - MIN(minUpdateTime-1,4)*250 ) )
	{
		if ( tplAmount < TEMPLATE_NUM )
		{
			SET_KEY_2();

			// save new to flash
			LOGD("--> Update to %d", tplAmount );
			updateID = tplAmount++;
		}
		else if ( updateFlag && ( minDistance >= 256*20 ) )
		{
			SET_KEY_2();
			
			// update existed in flash
			updateID = ( minDisUpdateTime>(minUpdateTime+TEMPLATE_NUM) ) ? minUpdateID : minDisID;
			LOGD("--> Update to %d, minUpdateID %d, minDisID %d", updateID, minUpdateID, minDisID );
		}
		if ( updateID >=0 && policy_save_subtpl(fid, updateID, (subtpl_t *)tplCur) < 0)
		{
			LOGD("--> Update fail");
			// return -2;
		}
	}

	return maxMatchScore;
}


//// <0 match fail, >=0 match success, return movement
int afs_checkMove( int *stpls, int *procBuf )
{
	int *tplCur = stpls;
	int matchScore, transCnt, transVec[4];
	subtpl_t *tplLast = (subtpl_t *)stpls + 1;
	
	if (policy_load_latest_modified_subtpl(tplLast) < 0) {
		LOGE("--> Load latest updated subtpl failed");

		return -1;
	}
	
	transCnt = fastMatch( tplCur, (int*)tplLast, procBuf );

	if ( transCnt == 0 )
		return -1;

	matchScore = accuracyMatch( tplCur, (int*)tplLast, transCnt, transVec, procBuf, 0 );

	return ( matchScore > 0 ) ? (int)fixedAbs( transVec[1], transVec[2] ) : -1;
}
