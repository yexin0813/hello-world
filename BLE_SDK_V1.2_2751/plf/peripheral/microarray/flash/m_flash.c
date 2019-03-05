/*
 * Built-in Function defines 
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/04/17     Define functions for built-in flash operation
 */

#include <stdint.h>        
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bx_dbg.h"
#include "spi_flash_cmd.h"
#include "log.h"
#include "flash.h"
#include "app_qspi_wrapper.h"
#ifdef CFG_FREERTOS_SUPPORT
#include "FreeRTOS.h"
#include "task.h"
#endif
#ifdef FLASH_XIP
#include "flash_cache.h"
#endif
#include "flash.h"
#include "m_flash.h"
#include "m_uart.h"
#include "m_timer.h"
#include "tpls_mngr.h"

//#define TEST_FLASH (1)


/** 
  * @brief  FLASH Status  
  */
#define FLASH_ERROR_PG (1)
#define FLASH_COMPLETE (0)

#define ROUND_UP(x, aligned)                    (((x) + (aligned) - 1) / (aligned) * (aligned))





#if (TEST_FLASH==1)
/*
 * This function for test the SPI Flash read and write operations
 *
 *
 *
 */
#define SECTOR_SIZE 0x1000
#define FLASH_SIZE (1<<20)
#define SECTOR_SIZE_IN_WORD (SECTOR_SIZE/sizeof(uint32_t))
volatile uint32_t fbuf[SECTOR_SIZE_IN_WORD];
uint32_t fsrc[SECTOR_SIZE_IN_WORD];


void test_flash_operations(void)
{
    
    uint32_t read[4]  = {0x00};
    uint32_t write[4] = {0x00};
    uint32_t need_us = 0;
    uint32_t j = 0;
    uint32_t data = 0x00;
    uint32_t addr = 0x00;
    uint32_t i = 0;
    
    addr = EFM0_MAIN_BASEADDR;

    write[0] = 0x106b3bab;


    io_cfg_output(23);
    io_pin_clear(23);


    flash_cache_flush_all();
    for (i = 0; i < SECTOR_SIZE/4; i++){
     debugPrint("***xxx:%d = %x\n",i,*((uint32_t *)(EFM0_MAIN_BASEADDR+0x80000+(i*4))));
     
    timerDelayMs(20);
    }


#if 0
    flash_quad_read(EFM0_MAIN_BASEADDR,4,(uint8_t *)read);
    debugPrint("1.Read Flash address %x:%x.\n",EFM0_MAIN_BASEADDR,read[0]);
    timerDelayMs(200);

    flash_program(EFM0_MAIN_BASEADDR,4,(uint8_t *)write);
    flash_quad_read(EFM0_MAIN_BASEADDR,4,(uint8_t *)read);
    debugPrint("Read Flash address %x:%x after write.\n",EFM0_MAIN_BASEADDR,read[0]);

    timerDelayMs(200);
    write[0] = 0x106b3bac;
    flash_program(EFM0_MAIN_BASEADDR,4,(uint8_t *)write);
    flash_quad_read(EFM0_MAIN_BASEADDR,4,(uint8_t *)read);
    debugPrint("2.Read Flash address %x:%x after write.\n",EFM0_MAIN_BASEADDR,read[0]);
#endif    
    
#if 0
    write[0] = 0x55AA5AA5;
   // sysc_cpu_qspi_en_setf(0xf);
    flash_quad_read(EFM0_MAIN_BASEADDR,4,(uint8_t *)read);
    debugPrint("Read Flash address %x:%x.\n",EFM0_MAIN_BASEADDR,read[0]);
    
    timerDelayMs(200);

    /*
     * Will erase the flash when it is factory state, and then write one word in it.
     * Power off the flash and read it again, check the data is non-volatile.
     */
    if (read[0] == 0xFFFFFFFF){
        flash_erase(EFM0_MAIN_BASEADDR,Sector_Erase);
        debugPrint("Read Flash address %x:%x after erase.\n",EFM0_MAIN_BASEADDR,read[0]);
        
        timerDelayMs(200);
        flash_program(EFM0_MAIN_BASEADDR,4,(uint8_t *)write);
        flash_quad_read(EFM0_MAIN_BASEADDR,4,(uint8_t *)read);
        debugPrint("Read Flash address %x:%x after write.\n",EFM0_MAIN_BASEADDR,read[0]);
    }
    #endif
/*
     for (j = 0; j < 1024; j++)
     {
         
         flash_quad_read(addr,4,(uint8_t *)(&fbuf[j]));
         debugPrint("***fbuf[%d]=%x\n",j,fbuf[j]);
         timerDelayMs(2);
         addr += 4;
         
     }

     addr = EFM0_MAIN_BASEADDR;
     for (j = 0; j < 1024; j++)
     {
         
         flash_quad_read(addr,4,(uint8_t *)(&data));
         debugPrint("***1.Read Flash address %d =%x\n",j,data);
         timerDelayMs(20);
         addr += 4;
         
     }
     */

     /* Time cost test */
     timerRestart();

     timerDelayMs(33);
     
     need_us = timerElapsedUs();
     debugPrint("~~~Delay time cost(us). = %u\n",need_us);
     timerDelayMs(200);

    

     /* Time cost test */
     timerRestart();
    
     eflash_page_erase(EFM0_MAIN_BASEADDR+0x80000);
     
     need_us = timerElapsedUs();
     debugPrint("~~~Erase one page time cost(us). = %u\n",need_us);
     timerDelayMs(200);


     /*
     addr = EFM0_MAIN_BASEADDR;
     for (j = 0; j < 1024; j++)
     {
         
         flash_quad_read(addr,4,(uint8_t *)(&data));
         debugPrint("2.***Read Flash address %d =%x\n",j,data);
         timerDelayMs(20);
         addr += 4;
         
     }*/


    

     
     timerRestart();
     flash_quad_read(EFM0_MAIN_BASEADDR+0x80000,SECTOR_SIZE,(uint8_t *)fbuf);
     need_us = timerElapsedUs();
     debugPrint("***Read one page time cost(us). = %u\n",need_us);
     timerDelayMs(200);

     /*
     timerRestart();
     set_gpio_out_val(8,1);
     timerDelayMs(7);
     set_gpio_out_val(8,0);
     need_us = timerElapsedUs();
     debugPrint("***Delay 7ms cost(us). = %u\n",need_us);
     timerDelayMs(200);
     */
     
     for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
     {
        if(fbuf[i] != 0xffffffff)
        {
            
            debugPrint("Erase error!\n");
            BX_ASSERT(0);
            break;
        }
     }
     
     /* program test */
     for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
     {
         fsrc[i] = rand()+2;
         //debugPrint("fsrc[%d]:%x\n",i,fsrc[i]);
         //timerDelayMs(2);
     }
     
     debugPrint("***fsrc[0] = %x\n",fsrc[0]);
     
     timerDelayMs(200);

     timerRestart();
     
     io_pin_set(23);
     eflash_bulk_program(EFM0_MAIN_BASEADDR+0x80000,SECTOR_SIZE/4,fsrc);
     
     io_pin_clear(23);
     //CC_EFLASH_Write(EFM0_MAIN_BASEADDR,SECTOR_SIZE,fsrc);
     need_us = timerElapsedUs();
     debugPrint("~~~Program one page time cost(us). = %u\n",need_us);
     timerDelayMs(200);

     /*
     addr = EFM0_MAIN_BASEADDR;
     for (j = 0; j < 1024; j++)
     {
         
         flash_quad_read(addr,4,(uint8_t *)(&data));
         debugPrint("***Read Flash address %d =%x\n",j,data);
         timerDelayMs(20);
         addr += 4;
         
     }*/

     


     timerRestart();
     flash_quad_read(EFM0_MAIN_BASEADDR+0x80000,SECTOR_SIZE,(uint8_t *)fbuf);
     need_us = timerElapsedUs();
     debugPrint("***Read one page time cost(us). = %u\n",need_us);
     timerDelayMs(200);

     
     for(i=0;i<SECTOR_SIZE_IN_WORD;++i)
     {
        if(fsrc[i]!=fbuf[i])
        {
            debugPrint("Flash program error!\n");
            BX_ASSERT(0);
            break;
        }
     }
     
    debugPrint("***fsrc[1] = %x\n",fsrc[1]);
    timerDelayMs(200);
    debugPrint("***fsrc[2] = %x\n",fsrc[2]);
    timerDelayMs(200);
    debugPrint("***fsrc[3] = %x\n",fsrc[3]);
    timerDelayMs(200);
    
    debugPrint("***to[1] = %x\n",fbuf[1]);
    timerDelayMs(200);
    debugPrint("***to[2] = %x\n",fbuf[2]);
    timerDelayMs(200);
    debugPrint("***to[3] = %x\n",fbuf[3]);

    timerDelayMs(200);

   flash_cache_flush_all();
   for (i = 0; i < SECTOR_SIZE/4; i++){
    debugPrint("***xxx:%d = %x\n",i,*((uint32_t *)(EFM0_MAIN_BASEADDR+0x80000+(i*4))));
    
   timerDelayMs(20);
   }
    
    
    
    

#if 0


    timerRestart();
    flash_erase(EFM0_MAIN_BASEADDR,Sector_Erase);
    need_us = timerElapsedUs();
    debugPrint("***2.Erase one page time cost(us). = %u\n",need_us);
    
    timerDelayMs(200);




    


    memset(fsrc,0x00,SECTOR_SIZE);
    flash_program(EFM0_MAIN_BASEADDR,SECTOR_SIZE,(uint8_t *)fsrc);


    timerRestart();
    flash_program(EFM0_MAIN_BASEADDR,SECTOR_SIZE,(uint8_t *)fsrc);
    need_us = timerElapsedUs();
    debugPrint("***Program one page time cost(us). = %u\n",need_us);
    
    timerDelayMs(200);




    timerRestart();
    flash_erase(EFM0_MAIN_BASEADDR,Sector_Erase);
    need_us = timerElapsedUs();
    debugPrint("***3.Erase one page time cost(us). = %u\n",need_us);
#endif


}
#endif





static uint32_t  get_page_addr(uint32_t addr);


/*
 * Write the data to specific flash address
 * @flashaddr,flash address
 * @datalen, byte length
 * @bufaddr, data buffer for write the flash
 * @Return value, FLASH_COMPLETE   write the flash successful
 *                FLASH_ERROR_PG   flash program error
 *                FLASH_ERROR_WRP  flash write protect
 */

uint8_t eflash_write(uint32_t flashaddr, uint32_t datalen, uint32_t *bufaddr)
{
	uint8_t  ret = FLASH_ERROR_PG;
    uint32_t len = 0;
	uint32_t addr_write = 0x00;
    uint32_t *buf_ptr = (void *)0;

    buf_ptr = bufaddr;
    len = ROUND_UP(datalen, 4) / 4;

	addr_write = flashaddr ;

    
    
    
    //ret = flash_erase(get_page_addr(flashaddr),Sector_Erase);
    
	
	//if (ret != FLASH_COMPLETE){
        
  //      debugPrint("Flash erase error in eflash_write!\n");
	//	return ret;
	//}

    

    
    flash_program(addr_write,len*4,(uint8_t *)buf_ptr);
    

    /*
	while(len--){
        flash_program(addr_write,4,(uint8_t *)buf_ptr);
    
		if (ret != FLASH_COMPLETE){
            debugPrint("Flash program error in eflash_write!\n");
			return ret;
		}
		buf_ptr++;
		addr_write += 4;
	}*/



    flash_cache_flush_all();

    
	return 0;
}


/*
 * Erase one page
 * @addr The flash address for need erase
 * @Return the erase status
 *
 */
uint8_t   eflash_page_erase(uint32_t addr) 
{
	uint8_t  ret = FLASH_ERROR_PG;

   // debugPrint("erase\n\r");

    
    
    ret = flash_erase(get_page_addr(addr),Sector_Erase);


    
    flash_cache_flush_all();

    

    
    

    if (ret == FLASH_COMPLETE){
	    return EFLASH_PROG_ERASE_PASS;
    }else{
        return EFLASH_PROG_ERASE_FAIL;
    }


    
    
}


/*
 * Write one word into flash
 * @addr,address
 * @data,one word ready to write
 */
uint8_t eflash_program (uint32_t addr, uint32_t data)
{

	uint8_t	  ret = FLASH_ERROR_PG;
    uint32_t  flash_addr   = 0x00;
    uint32_t  program_data = 0xFFFFFFFF;


    flash_addr   = addr;
    program_data = data;

    
    ret = flash_program(flash_addr,4,(uint8_t *)(&program_data));



    flash_cache_flush_all();

    


    if (ret == FLASH_COMPLETE){
	    return FLASH_COMPLETE ;
    }else{
        return ret;
    }


    
}





/*
 * Write abitrary length word into flash
 * @addr, flash address
 * @num_words,the totals need to write
 * @data_buf, a pointer to need to write data
 */
uint8_t eflash_bulk_program(uint32_t addr, uint32_t num_words, uint32_t *data_buf)
{

		uint8_t   ret = FLASH_ERROR_PG; 
    uint32_t  write_addr = 0x00;
    uint32_t  *buf_ptr = (void *)0;
    uint32_t  len = 0x00;

   // uint32_t i = 0;
   // uint32_t data = 0x00;

   // debugPrint("bulk\n\r");
    
    len        = num_words;
    write_addr = addr;
    buf_ptr    = data_buf;


    ret = flash_program(write_addr,len*4,(uint8_t *)(buf_ptr));   


    
    flash_cache_flush_all();
    
    

/*
    for (i = 0; i < len; i++)
    {
        flash_read(write_addr,4,(uint8_t *)&data);
        debugPrint("addr:%x,write value:%x,read value:%x\n\r",write_addr,data_buf[i],data);
        timerDelayMs(20);
        write_addr += 4;
    }*/

    /*
	while(len--){
        ret = flash_program(write_addr,4,(uint8_t *)(buf_ptr));   
		write_addr += 4;
		buf_ptr++;	 
		if (ret != FLASH_COMPLETE){	
			return ret;
		}		 	 
	}
	*/

    if (ret != FLASH_COMPLETE){ 
        return ret;
    }else{
	    return FLASH_COMPLETE;
    }

    
    
}





static uint32_t  get_page_addr(uint32_t addr)
{
    
    uint32_t which_page = 0x0000;

    which_page = addr - KEY_MAIN_BASEADDR;

    which_page = which_page / EFLASH_PAGESIZE;

    return (KEY_MAIN_BASEADDR+(which_page*EFLASH_PAGESIZE));
    
}




/*
 * Read data from winbond SPI Flash
 *
 *
 */
void *wb_memcpy(void *dest, const void *src, uint32_t n)
{
    
    uint8_t (*read)(uint32_t src,uint32_t length,uint8_t *dst);

    read = flash_read_func_ptr_get();
    
    read((uint32_t)src,n,(uint8_t *)dest);

    return dest;
}



