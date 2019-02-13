#include "CC_FPALG.h"
//#include "c200.h"

/**********************************
 * ccfpa struct variable
***********************************/
ALU_MAC8_REG gAluMac8Reg;

ALU_TRI16_REG gAluTri16Reg;

ALU_PHA8_REG gAluPha8Reg;

/**********************************
 * ccfpa struct variable init
***********************************/
void ccFpAluInit()
{
	#if 0
	asm("psrset af");
	G_R1_write(&gAluMac8Reg); //alg1_global_struct_addr
	G_R2_write(&gAluTri16Reg); //alg2_global_struct_addr
	G_R3_write(&gAluPha8Reg); //alg3_global_struct_addr
	asm("psrclr af");
	#endif
}

