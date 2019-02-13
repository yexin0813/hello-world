
/**
 ****************************************************************************************
 *
 * @file patch_test.c
 *
 * @brief test case of the bus patch.
 *
 * Copyright (C) Apollo
 *
 *
 ****************************************************************************************
 */
#include "bx_config.h"
#include "patch_test.h"
#include "patch.h"
#if (defined(BX_VERF)&&(BX_VERF==1))
#define SIM_RESULT      (*((volatile unsigned long *)(0x2010f004)))
#endif
extern void PATCH1(void);
extern void PATCH2(void);
extern void PATCH3(void);
extern void PATCH4(void);
extern void PATCH5(void);
extern void PATCH6(void);
extern void PATCH7(void);
extern void PATCH8(void);

static uint32_t patch1_jmp = 0;
static uint32_t patch2_jmp = 0;
static uint32_t patch3_jmp = 0;
static uint32_t patch4_jmp = 0;
static uint32_t patch5_jmp = 0;
static uint32_t patch6_jmp = 0;
static uint32_t patch7_jmp = 0;
static uint32_t patch8_jmp = 0;

#pragma push
#pragma O0

__attribute__ ((section("PATCH_TEST_AREA"))) void test1()	/**< error func1 for test */
{
	*(uint32_t*)(0x00120000) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test2()	/**< error func2 for test */
{
	*(uint32_t*)(0x00120004) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test3()	/**< error func3 for test */
{
	*(uint32_t*)(0x00120008) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test4()	/**< error func4 for test */
{
	*(uint32_t*)(0x0012000C) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test5()	/**< error func5 for test */
{
	*(uint32_t*)(0x00120010) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test6()	/**< error func6 for test */
{
	*(uint32_t*)(0x00120014) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test7()	/**< error func7 for test */
{
	*(uint32_t*)(0x00120018) = 0x1288;
}


__attribute__ ((section("PATCH_TEST_AREA"))) void test8()	/**< error func8 for test */
{
	*(uint32_t*)(0x0012001C) = 0x1288;
}

/**
 * @brief patch1 for test
 */
void patch_test1()
{
	*(uint32_t*)(0x00120000) = 0x8756;
}
/**
 * @brief patch2 for test
 */
void patch_test2()
{
	*(uint32_t*)(0x00120004) = 0x8756;
}
/**
 * @brief patch3 for test
 */
void patch_test3()
{
	*(uint32_t*)(0x00120008) = 0x8756;
}
/**
 * @brief patch4 for test
 */
void patch_test4()
{
	*(uint32_t*)(0x0012000C) = 0x8756;
}
/**
 * @brief patch5 for test
 */
void patch_test5()
{
	*(uint32_t*)(0x00120010) = 0x8756;
}
/**
 * @brief patch6 for test
 */
void patch_test6()
{
	*(uint32_t*)(0x00120014) = 0x8756;
}
/**
 * @brief patch7 for test
 */
void patch_test7()
{
	*(uint32_t*)(0x00120018) = 0x8756;
}
/**
 * @brief patch8 for test
 */
void patch_test8()
{
	*(uint32_t*)(0x0012001C) = 0x8756;
}

/**
 * @brief test the function of patch. test1-8 should be patch to patch_test1-8.
 */

void test_patch()
{
	volatile uint8_t failed = 0;
    volatile uint32_t code;
	patch1_jmp = cal_patch_bl((uint32_t)test1+4,(uint32_t)PATCH1);                 // test1 mod 4 == 0 use +4
    patch2_jmp = cal_patch_bl((uint32_t)test2+2,(uint32_t)PATCH2);                 // test2 mod 4 == 2 use +2 
    patch3_jmp = cal_patch_bl((uint32_t)test3+4,(uint32_t)PATCH3);
    patch4_jmp = cal_patch_bl((uint32_t)test4+2,(uint32_t)PATCH4);
    patch5_jmp = cal_patch_bl((uint32_t)test5+4,(uint32_t)PATCH5);
    patch6_jmp = cal_patch_bl((uint32_t)test6+2,(uint32_t)PATCH6);
    patch7_jmp = cal_patch_bl((uint32_t)test7+4,(uint32_t)PATCH7);
    patch8_jmp = cal_patch_bl((uint32_t)test8+2,(uint32_t)PATCH8);


    //test patch

    code = cal_patch_pushlr((uint32_t)test1);
	patch_entrance_exit_addr(0,(uint32_t)test1,code);		//push lr          // address of test1 mod 4 == 0 replace (push lr & nop)
    patch_entrance_exit_addr(1,(uint32_t)test1+4,patch1_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test2);
	patch_entrance_exit_addr(2,(uint32_t)test2,code);		//push lr           // address of test2 mod 4 == 2 replace (nop & push lr)
    patch_entrance_exit_addr(3,(uint32_t)test2+2,patch2_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test3);
	patch_entrance_exit_addr(4,(uint32_t)test3,code);		//push lr
    patch_entrance_exit_addr(5,(uint32_t)test3+4,patch3_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test4);
	patch_entrance_exit_addr(6,(uint32_t)test4,code);		//push lr
    patch_entrance_exit_addr(7,(uint32_t)test4+2,patch4_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test5);
	patch_entrance_exit_addr(8,(uint32_t)test5,code);		//push lr
    patch_entrance_exit_addr(9,(uint32_t)test5+4,patch5_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test6);
	patch_entrance_exit_addr(10,(uint32_t)test6,code);		//push lr
    patch_entrance_exit_addr(11,(uint32_t)test6+2,patch6_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test7);
	patch_entrance_exit_addr(12,(uint32_t)test7,code);		//push lr
    patch_entrance_exit_addr(13,(uint32_t)test7+4,patch7_jmp);			//  bl

    code = cal_patch_pushlr((uint32_t)test8);
	patch_entrance_exit_addr(14,(uint32_t)test8,code);		//push lr
    patch_entrance_exit_addr(15,(uint32_t)test8+2,patch8_jmp);			//  bl

	patch_enable_set(0xFFFF);

	uint8_t i;
    for(i = 0; i < 32; i++)         /**< nop sometime to make sure the patch is enable */
    {
	__asm("nop");
	//LOG(LOG_LVL_INFO, "delay\r\n");// ticks delay
    }


	test1();
	test2();
	test3();
	test4();
	test5();
	test6();
	test7();
	test8();

#if (defined(BX_VERF)&&(BX_VERF==1))
	if(*(uint32_t*)(0x00120000) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x00120004) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x00120008) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x0012000C) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x00120010) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x00120014) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x00120018) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}
	if(*(uint32_t*)(0x0012001C) != 0x8756)
	{
		SIM_RESULT = 0x20;
		SIM_RESULT = 0x30;
		failed = 1;
	}

	if(failed == 0)
	{
		SIM_RESULT = 0x10;
		SIM_RESULT = 0x30;
	}
#endif

}
#pragma pop
