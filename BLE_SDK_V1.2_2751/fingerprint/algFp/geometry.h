//
//	geometry.h
//
//	U8 angle
//	U8 sin / cos/ atan / abs
//	U16 angles
//	U16 sin / cos/ atan / abs
//	U16 rotate
//	
//	by LYY 2016-05-03
//

#ifndef GEOMETRY_H_LYY_
#define GEOMETRY_H_LYY_

#include "algType.h"


#ifdef __cplusplus
extern "C" {
#endif

// -------------------- U8 angle --------------------

#define difAngleMacroU8( angle1, angle2 )	( (u8)( angle1 - angle2 ) )
u8 difAngleFuncU8( u8 angle1, u8 angle2 );

#define difAngleMacroS8( angle1, angle2 )	( (s8)( angle1 - angle2 ) )
s8 difAngleFuncS8( u8 angle1, u8 angle2 );

#define absDifAngleMacroU8( angle1, angle2 )	( ( ((s8)(angle1-angle2))>0 ) ? ((u8)(angle1-angle2)) : ((u8)(angle2-angle1)) )
u8 absDifAngleFuncU8( u8 angle1, u8 angle2 );

#define aveAngleMacroU8( angle1, angle2 )	( (u8)( (s32)(angle1) + (s32)( ((s8)(angle2-angle1))>>1 ) ) )
u8 aveAngleFuncU8( u8 angle1, u8 angle2 );

// -------------------- U8 sin/cos/atan/abs --------------------

extern const s8 sinTableS8[];

#define sinMacroU8( angle )	( sinTableS8[(u8)(angle)] )
s8 sinFuncU8( u8 angle );

#define cosMacroU8( angle )	( sinTableS8[(u8)(63-(angle))] )
s8 cosFuncU8( u8 angle );

void absAtan2FuncU8( s32 yVal, s32 xVal, u32 *absVal, u8 *angle );

u32 absFuncU8( s32 yVal, s32 xVal );

u8 atan2FuncU8( s32 yVal, s32 xVal );

// -------------------- U16 angle --------------------

#define difAngleMacroU16( angle1, angle2 )	( (u16)( (angle1) - (angle2) ) )
u16 difAngleFuncU16( u16 angle1, u16 angle2 );

#define difAngleMacroS16( angle1, angle2 )	( (s16)( (angle1) - (angle2) ) )
s16 difAngleFuncS16( u16 angle1, u16 angle2 );

#define absDifAngleMacroU16( angle1, angle2 )	( ( ( (s16)( (angle1)-(angle2) ) )>0 ) ? ( (u16)( (angle1)-(angle2) ) ) : ( (u16)( (angle2)-(angle1)) ) )
u16 absDifAngleFuncU16( u16 angle1, u16 angle2 );

#define aveAngleMacroU16( angle1, angle2 )	( (u16)( (s32)(angle1) + (s32)( ( (s16)( (angle2)-(angle1) ) )>>1 ) ) )
u16 aveAngleFuncU16( u16 angle1, u16 angle2 );

// -------------------- U16 sin/cos/atan/abs --------------------

extern s16 sinTableS16[];

#define sinMacroU16( angle )	( sinTableS16[(u16)(angle)] )
s16 sinFuncU16( u16 angle );

#define cosMacroU16( angle )	( sinTableS16[(u16)(16384-(angle))] )
s16 cosFuncU16( u16 angle );

void absAtan2FuncU16( s32 yVal, s32 xVal, u32 *absVal, u16 *angle );

u32 absFuncU16( s32 yVal, s32 xVal );

u16 atan2FuncU16( s32 yVal, s32 xVal );

// -------------------- U16 Rotate --------------------

void rotateVectorU16( s32 yIn, s32 xIn, u16 angle, s32 *yOut, s32 *xOut );

//#if PLATFORM_TYPE == PLATFORM_C0CORE

// -------------------- U16 sin/cos/atan/abs --------------------
#define SinAluU16( angle, sinVal)			\
{														\
	gAluTri16Reg.angleU16 = (unsigned short)angle;		\
	asm(".short 0x0010"); 								\
	sinVal = gAluTri16Reg.sinValS16;					\
}

#define CosAluU16( angle, cosVal )			\
{														\
	gAluTri16Reg.angleU16 = (unsigned short)angle;		\
	asm(".short 0x0010"); 								\
	cosVal = gAluTri16Reg.cosValS16;					\
}

#define AbsAtan2AluU16( yVal, xVal, absVal, angle )  ALU_ABS_ATAN2( yVal, xVal, angle, absVal )

#define AbsAluU16( yVal, xVal, absVal )		\
{														\
	gAluTri16Reg.inYValuS32 = yVal;						\
	gAluTri16Reg.inXValuS32 = xVal;						\
	asm(".short 0x0012"); 								\
	absVal = gAluTri16Reg.absValU32;					\
}

#define Atan2AluU16( yVal, xVal, angle )		\
{														\
	gAluTri16Reg.inYValuS32 = yVal;						\
	gAluTri16Reg.inXValuS32 = xVal;						\
	asm(".short 0x0012"); 								\
	angle  = gAluTri16Reg.angleU16;						\
}

// -------------------- U16 Rotate --------------------

#define RotateVectorAluU16( yIn, xIn, angle, yOut, xOut )		ALU_ROTATE( yIn, xIn, angle, yOut, xOut )	\


//#elif PLATFORM_TYPE == PLATFORM_GENERAL

#define FPALG_FPGA 1
// -------------------- U16 sin/cos/atan/abs --------------------

s16 SinSoftU16( u16 angle );

s16 CosSoftU16( u16 angle );

void AbsAtan2SoftU16( s32 yVal, s32 xVal, u32 *absVal, u16 *angle );

u32 AbsSoftU16( s32 yVal, s32 xVal );

u16 Atan2SoftU16( s32 yVal, s32 xVal );

// -------------------- U16 Rotate --------------------

void RotateVectorSoftU16( s32 yIn, s32 xIn, u16 angle, s32 *yOut, s32 *xOut );

//#endif

#ifdef __cplusplus
}
#endif

#endif
