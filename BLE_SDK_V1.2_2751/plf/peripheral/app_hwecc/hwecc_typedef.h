#ifndef HW_ECC_TYPEDEF_H_
#define HW_ECC_TYPEDEF_H_
typedef enum
{
	ECC_Idle = 0,
	ECC_Busy
}HW_ECC_START_BUSY_FIELD;
typedef enum
{
	With_Inversion = 0,
	Without_Inversion
}HW_ECC_WO_INV_FIELD;
typedef enum
{
	Full_ECC_Calc = 0,
	Multiply_Calc,
	ECC_Smpl_Calc,
	Jcb_Add_Calc,
	Jcb_Dble_Calc,
	Gf_Inv_Calc
}HW_ECC_START_SEL_FIELD;

#endif
