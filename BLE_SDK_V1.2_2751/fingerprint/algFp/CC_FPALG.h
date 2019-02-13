#ifndef _CC_FPALG_H_
#define _CC_FPALG_H_
#include "algType.h"
#include "type_def.h"

// init @ system up
//void ccFpAluInit();


// disable hardware interrupt before ALU execute, enable after ALU, for efficiency
#define ALU_MAC8_DIS_INT		1

// disable hardware interrupt before ALU execute, enable after ALU, for MPW version
// mass production version choose 0
#define ALU_ROTATE_DIS_INT		1

// disable hardware interrupt before ALU execute, enable after ALU, for efficiency
#define ALU_PHA8_DIS_INT		0

// -------------------- MAC 8 --------------------

typedef struct
{
	volatile unsigned int m_adrA;
	volatile unsigned int m_adrB;
	volatile unsigned int m_para;
	volatile unsigned int m_result;
} ALU_MAC8_REG;

extern ALU_MAC8_REG  gAluMac8Reg;

#define ALU_INIT_MAC8( adrA, adrB, para ) 			\
{													\
	gAluMac8Reg.m_adrA = adrA;						\
	gAluMac8Reg.m_adrB = adrB;						\
	gAluMac8Reg.m_para = para;						\
}

#define ALU_RLT_MAC8( result ) 						\
{													\
	result = gAluMac8Reg.m_result;					\
}

#if ALU_MAC8_DIS_INT == 0
	#define ALU_UMAC8( adrA, adrB, para, result )	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#else // ALU_MAC8_DIS_INT == 1
	#define ALU_UMAC8( adrA, adrB, para, result)	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#endif

#if ALU_MAC8_DIS_INT == 0
	#define ALU_SMAC8( adrA, adrB, para, result )	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#else // ALU_MAC8_DIS_INT == 1
	#define ALU_SMAC8( adrA, adrB, para, result )	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#endif

#if ALU_MAC8_DIS_INT == 0
	#define ALU_DMAC8( adrA, adrB, para, result ) 	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#else // ALU_MAC8_DIS_INT == 1
	#define ALU_DMAC8( adrA, adrB, para, result ) 	\
	{												\
		ALU_INIT_MAC8( adrA, adrB, para ); 			\
		ALU_RLT_MAC8( result );						\
	}
#endif

/*/-------------------- TRIANGLE 16 --------------------

typedef struct
{
	union
	{
		volatile unsigned int   angleU32;
		volatile unsigned short angleU16;
	};
	union
	{
		volatile signed int   sinValS32;
		volatile signed short sinValS16;
	};
	union
	{
		volatile signed int   cosValS32;
		volatile signed short cosValS16;
	};
	union
	{
		volatile signed int   inXValS32;
		volatile signed short inXValS16;
		volatile signed int   outXValS32;
		volatile signed short outXValS16;
	};
	union
	{
		volatile signed int   inYValS32;
		volatile signed short inYValS16;
		volatile signed int   outYValS32;
		volatile signed short outYValS16;
	};
	
	volatile unsigned int absValU32;
	volatile unsigned int sCutValU32;
	
} ALU_TRI16_REG;
*/

//-------------------- TRIANGLE 16 --------------------

typedef struct
{

		volatile unsigned int   angleU32;
		volatile unsigned short angleU16;

		volatile signed int   sinValS32;
		volatile signed short sinValS16;

		volatile signed int   cosValS32;
		volatile signed short cosValS16;

		volatile signed int   inXValS32;
		volatile signed short inXValS16;
		volatile signed int   outXValS32;
		volatile signed short outXValS16;

		volatile signed int   inYValS32;
		volatile signed short inYValS16;
		volatile signed int   outYValS32;
		volatile signed short outYValS16;

	  volatile unsigned int absValU32;
	  volatile unsigned int sCutValU32;
	
} ALU_TRI16_REG;


extern ALU_TRI16_REG gAluTri16Reg;

#define SCUT_EN 										\
{														\
}

#define SCUT_DIS										\
{														\
}

#define SCUT_SET( val )									\
{														\
	gAluTri16Reg.sCutValU32 = (unsigned int)val;		\
}

#define ALU_SIN_COS( angle, sinVal, cosVal )			\
{														\
	gAluTri16Reg.angleU16 = (unsigned short)angle;		\
	sinVal = gAluTri16Reg.sinValS16;					\
	cosVal = gAluTri16Reg.cosValS16;					\
}

// abs( max( yVal, xVal ) ) < ???
#define ALU_ABS_ATAN2( yVal, xVal, angle, absVal )		\
{														\
	gAluTri16Reg.inYValS32 = yVal;						\
	gAluTri16Reg.inXValS32 = xVal;						\
	angle  = gAluTri16Reg.angleU16;						\
	absVal = gAluTri16Reg.absValU32;					\
}

#if ALU_ROTATE_DIS_INT == 0
	#define ALU_ROTATE( inY, inX, angle, outY, outX )	\
	{													\
		gAluTri16Reg.inYValS32 = inY;					\
		gAluTri16Reg.inXValS32 = inX;					\
		gAluTri16Reg.angleU16 = angle;					\
		outY = gAluTri16Reg.outYValS32;					\
		outX = gAluTri16Reg.outXValS32;					\
	}
#else // ALU_ROTATE_DIS_INT == 1
	#define ALU_ROTATE( inY, inX, angle, outY, outX )	\
	{													\
		gAluTri16Reg.inYValS32 = inY;					\
		gAluTri16Reg.inXValS32 = inX;					\
		gAluTri16Reg.angleU16 = angle;					\
		outY = gAluTri16Reg.outYValS32;					\
		outX = gAluTri16Reg.outXValS32;					\
	}
#endif

// -------------------- PHA 8 --------------------

typedef struct
{
	volatile unsigned int pha;
	volatile unsigned int amp;
	volatile unsigned int ori;
	volatile unsigned int freq;

	volatile unsigned int m_oriVal;
	volatile unsigned int m_preH;
	volatile unsigned int m_preW;

	volatile unsigned int m_imagVal;
	volatile unsigned int m_realVal;

	volatile unsigned int m_adrTensor0;
	volatile unsigned int m_adrTensor1;
	volatile unsigned int m_adrTensor2;
	volatile unsigned int m_adrTensor3;

	volatile unsigned short freqH[4];
	volatile unsigned short freqW[4];

} ALU_PHA8_REG;

extern ALU_PHA8_REG gAluPha8Reg;

#define ALU_SET_ADR_TS0( adrTensor0 )			\
{												\
	gAluPha8Reg.m_adrTensor0 = adrTensor0;		\
}

#define ALU_SET_ADR_TS1( adrTensor1 )			\
{												\
	gAluPha8Reg.m_adrTensor1 = adrTensor1;		\
}

#define ALU_SET_ADR_TS2( adrTensor2 )			\
{												\
	gAluPha8Reg.m_adrTensor2 = adrTensor2;		\
}

#define ALU_SET_ADR_TS3( adrTensor3 )			\
{												\
	gAluPha8Reg.m_adrTensor3 = adrTensor3;		\
}

#define ALU_SET_ORIVAL( oriVal )				\
{												\
	gAluPha8Reg.m_oriVal = oriVal;				\
}

#define ALU_SET_PREH( preH )					\
{												\
	gAluPha8Reg.m_preH = preH;					\
}

#define ALU_SET_PREW( preW )					\
{												\
	gAluPha8Reg.m_preW = preW;					\
}

#define ALU_GET_IMAG( imagVal )					\
{												\
	imagVal = gAluPha8Reg.m_imagVal;			\
}

#define ALU_GET_REAL( realVal )					\
{												\
	realVal = gAluPha8Reg.m_realVal;			\
}

#define ALU_REMAP_TENSOR( )						\
{												\
}

#define ALU_FREQ_COMP( )						\
{												\
}

#define ALU_INTERPOLATION( )					\
{												\
}

#if ALU_PHA8_DIS_INT == 0
	#define ALU_PHASE_ALL( )						\
	{												\
	}
#else
	#define ALU_PHASE_ALL( )						\
	{												\
	}
#endif

#endif //#ifndef _CC_FPALG_H
