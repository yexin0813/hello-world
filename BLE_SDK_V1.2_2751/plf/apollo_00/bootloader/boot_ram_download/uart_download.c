/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stddef.h>
#include "apollo_00.h"
#include "uart_download.h"
//#include "pshare.h"
#include "uart.h"
#include "flash.h"
//#include "ll.h"
//#include "clk_gen.h"
#include "reg_sysc_per.h"
#include "string.h"
#include "boot_memory_map.h"
/*
 * MACROS
 ****************************************************************************************
 */
//const
#define DBG1 (*(volatile uint32_t *)0x0012d000)
#define UART_RAMRUN_HEADER_FLAG     0xad
#define BOOTINIT_FLAG       0x3399
//uart one send pdu size max bytes
#define FLASH_ONE_UART_SEND_PDU_SIZE    4096  
//download reply cmd
#define DOWNLOAD_START      0
#define DOWNLOAD_ACK        1
#define DOWNLOAD_NACK       2
//flash cmd
#define FLASH_READ      0
#define FLASH_WRITE     1
#define FLASH_ERASE     2
//op code cmd
#define OP_PC_CMD                       0x01
#define OP_READ_FLASH_STATUS_REG        0x02
#define OP_READ_APOLLO_00_REG              0x03
#define OP_WRITE_APOLLO_00_REG             0x04
#define OP_WRITE_FLASH_STATUS_REG       0x05
#define OP_WRITE_W25_FLASH_STATUS_REG   0x06
#define OP_WRITE_TO_RAM_AND_RUN         0x07
#define OP_WRITE_TO_RAM_COMPLETE        0x08
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef struct
{
    uint8_t header_flag;
    struct
    {
        uint8_t r_w         :2; // 0:read 1:write 2:erase
        uint8_t section     :1; // 0:other cmd 1:read write flash
        uint8_t op          :5; // operation, only use when section equal 0. 
                                // op = 1 finish download
    }cmd;
    uint16_t length;
    uint32_t addr;
}Uart_Ramrun_Header_t;
typedef struct
{
    Uart_Ramrun_Header_t header;
    uint8_t data[FLASH_ONE_UART_SEND_PDU_SIZE];
}Data_From_Host_t;
typedef void (*read_fn_t)(uint8_t *buf,uint32_t length,void (*callback)(void));
typedef void (*write_fn_t)(uint8_t *buf,uint32_t length,void (*callback)(void));
typedef void (*init_fn_t)(uint16_t divisor);
typedef void (*reset_handler_t)(uint32_t,uint32_t,uint32_t,uint32_t);
typedef struct {
    write_fn_t tx_func;
    read_fn_t rx_func;
}uart_itf_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
//global
extern uint8_t wait_download_finish;
extern uint8_t read_back_finish;
extern uint8_t check_result;
extern uint8_t uart_baud_changed;
extern uint8_t start_ram_run ;
//local
static void uart_download_read(uint8_t *buf,uint32_t length,void (*callback)(void));
static void uart_download_write(uint8_t *buf,uint32_t length,void (*callback)(void));
static void uart_tx_finish(void);
static void uart_read32_register(uint32_t *val,uint32_t reg,uint16_t len);
static void uart_read32_register(uint32_t *val,uint32_t reg,uint16_t len);
static void uart_write_register(uint32_t reg,uint32_t value);
void uart_rx_header_finish(void);



/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
static uart_itf_t uart_itf;
static uint32_t apollo_00_reg_val[256];
static Data_From_Host_t uart_buff;
static uint8_t uart2pc[3][5] = 
{
    {'B','X','S',0x0d,0x0a},        //start
    {'B','X','A',0x0d,0x0a},        //ack
    {'B','X','N',0x0d,0x0a},        //nack
};
//Ram run parameter
static volatile uint8_t  write_to_ram_run = 0;                   //write to RAM and run
static uint32_t ram_run_current_location = RAM_BASE;    //Write to RAM current location
static uint32_t *vec_int_base;  //cannot use local variable! will clear MSP!!


//MUSTNOT IN INTERRUPT
void start_ram_run_function(void)
{
    //uninstall uart
    uart_uninit();
    //RUN APPLICATION
    vec_int_base = (uint32_t *)RAM_BASE;
    __set_MSP(*vec_int_base);
    reset_handler_t *reset_handler = (reset_handler_t *)(vec_int_base + 1);
    // Jump to user's Reset_Handler
    (*reset_handler)(0,0,0,BOOTINIT_FLAG);        //0x3399 indicate bootram has runned.
}

//rx finish parse
void uart_rx_payload_finish()
{
    uint16_t write_len;
    uint16_t buff_offset;
    uint16_t read_len;
    

    if(uart_buff.header.header_flag != UART_RAMRUN_HEADER_FLAG)
    {
        //FLAG ERROR REJECT
        uart_itf.tx_func(uart2pc[DOWNLOAD_NACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive failed
    }
    else
    {   
        //Not flash command
        if(uart_buff.header.cmd.section == 0)
        {
            if((uart_buff.header.cmd.op == OP_PC_CMD) && (uart_buff.header.length == 2))            //PC command
            {
                if((uart_buff.data[0] == 'F') && (uart_buff.data[1] == 'D'))                // file download complete
                {
                    wait_download_finish = 1;
                }
                else if((uart_buff.data[0] == 'C') && (uart_buff.data[1] == 'F'))           // flash check fail
                {
                    check_result = 0xff;
                    return;
                }
                else if((uart_buff.data[0] == 'C') && (uart_buff.data[1] == 'P'))           // flash check pass
                {
                    check_result = 1;
                    return;
                }  
                else if((uart_buff.data[0] == 'P') && (uart_buff.data[1] == 'R'))           // pc is ready
                {
                    uart_baud_changed = 1;
                    return;
                }
            }
						else if((uart_buff.header.cmd.op == OP_READ_FLASH_STATUS_REG) && (uart_buff.header.length == 2))   //read flash status reg   [0]idx  [1] reserved
						{
							uint8_t status=0;
							uint8_t reg_idx = uart_buff.data[0];
							
							status = flash_status_read(reg_idx);
							uart_itf.tx_func(&status,sizeof(status),uart_tx_finish);           		   //tell pc flash status				
						}
						else if((uart_buff.header.cmd.op == OP_READ_APOLLO_00_REG) && (uart_buff.header.length == 2))   //read apollo_00 reg
						{
							read_len = uart_buff.data[0] | (uart_buff.data[1] << 8);
							uart_read32_register(apollo_00_reg_val,uart_buff.header.addr,read_len);
							uart_itf.tx_func((uint8_t *)apollo_00_reg_val,read_len*4,uart_tx_finish);           		//tell pc read apollo_00 reg val
						}
						else if((uart_buff.header.cmd.op == OP_WRITE_APOLLO_00_REG) && (uart_buff.header.length == 4))   //write apollo_00 reg
						{
							uint32_t val;
							val = uart_buff.data[0] | (uart_buff.data[1] << 8) |uart_buff.data[2] << 16 | (uart_buff.data[3] << 24);
							uart_write_register(uart_buff.header.addr,val);
							uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive failed
						}
						else if((uart_buff.header.cmd.op == OP_WRITE_FLASH_STATUS_REG) && (uart_buff.header.length == 2))   //write flash status reg  [0]idx  [1] val
						{
							uint8_t status=0;
							uint8_t reg_idx = uart_buff.data[0];
							uint8_t reg_val = uart_buff.data[1];
							
							status = flash_status_write(reg_idx,reg_val);
							uart_itf.tx_func(&status,sizeof(status),uart_tx_finish);           		   //tell pc flash status	
							//uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive ok
						}
						else if((uart_buff.header.cmd.op == OP_WRITE_W25_FLASH_STATUS_REG) && (uart_buff.header.length == 2))   //write w25 flash status reg [0]val_l  [1] val_h
						{
							uint16_t status=0;
							uint8_t reg_vall = uart_buff.data[0];
							uint8_t reg_valh = uart_buff.data[1];
							
							status = flash_status_write_2bytes(reg_vall,reg_valh);
							uart_itf.tx_func((uint8_t *)&status,sizeof(status),uart_tx_finish);           		   //tell pc flash status	
							//uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);         //tell pc receive ok
						}
            else if((uart_buff.header.cmd.op == OP_WRITE_TO_RAM_AND_RUN) && (uart_buff.header.length == 2))
            {
                write_to_ram_run = 1;
							  uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);         //tell pc receive ok
            }
            else if((uart_buff.header.cmd.op == OP_WRITE_TO_RAM_COMPLETE) && (uart_buff.header.length == 2))
            {
                //start RAM RUN
                start_ram_run = 1;
 							  uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);         //tell pc receive ok
            }
            else
            {
                uart_itf.tx_func(uart2pc[DOWNLOAD_NACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive failed
            }
        }
        //Flash Command
        else if(uart_buff.header.cmd.section == 1)
        {    
            switch(uart_buff.header.cmd.r_w)                // flash operate
            {
                case FLASH_READ:
                    read_len = uart_buff.data[0] | (uart_buff.data[1] << 8);
                    flash_read(uart_buff.header.addr,read_len,&uart_buff.data[0]);
                    uart_itf.tx_func(&uart_buff.data[0],read_len,uart_tx_finish);           //tell pc receive success
                    break;
                case FLASH_WRITE:
                    if(write_to_ram_run == 0)//WRITE TO FLASH
                    {
                        buff_offset = 0;
                        while(uart_buff.header.length)
                        {
                            write_len = uart_buff.header.length > 256 ? 256 : uart_buff.header.length;
                            flash_program(uart_buff.header.addr,write_len,&uart_buff.data[buff_offset]);
                            buff_offset += write_len;
                            
                            uart_buff.header.length -= write_len;
                            uart_buff.header.addr += write_len;
                        }
                    }
                    else//WRITE TO RAM
                    {
                        //COPY TO RAM
                        memcpy((uint32_t *)ram_run_current_location , uart_buff.data , uart_buff.header.length);
                        ram_run_current_location += uart_buff.header.length;
                    }
                    uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive success
                    break;
                case FLASH_ERASE:
										{
                        uint8_t erase_type = uart_buff.data[0];
                        uint32_t erase_addr = uart_buff.header.addr;
                        flash_erase(erase_addr,(erase_t)erase_type);
                        uart_itf.tx_func(uart2pc[DOWNLOAD_ACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive success
										}
                    break;
                default:
                    uart_itf.tx_func(uart2pc[DOWNLOAD_NACK],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc receive failed
                break;
            }
        }
    }
    //Receive next cmd
    uart_itf.rx_func((uint8_t *)&uart_buff,sizeof(Uart_Ramrun_Header_t),uart_rx_header_finish);
}

//rx data
void uart_rx_header_finish()
{
    uart_itf.rx_func((uint8_t *)&uart_buff.data,uart_buff.header.length,uart_rx_payload_finish); 
}


//rx header
void uart_sync_to_pc()
{
    uart_itf.rx_func((uint8_t *)&uart_buff,sizeof(Uart_Ramrun_Header_t),uart_rx_header_finish);               //wait pc to send sync
}

//rx header
void uart_download_start()
{
    uart_itf.tx_func(uart2pc[DOWNLOAD_START],sizeof(uart2pc[0]),uart_tx_finish);           //tell pc chip is ready to receive data  
    uart_itf.rx_func((uint8_t *)&uart_buff,sizeof(Uart_Ramrun_Header_t),uart_rx_header_finish);
}

/*
 * UART TOOLS
 ****************************************************************************************
 */
static void uart_download_read(uint8_t *buf,uint32_t length,void (*callback)(void))
{
	uart_read(buf,length,(void (*)(void*,uint8_t))callback,NULL);
}


static void uart_download_write(uint8_t *buf,uint32_t length,void (*callback)(void))
{
	uart_write(buf, length, (void (*)(void*,uint8_t))callback, NULL);
}

static void uart_tx_finish(void)
{
    
}


static void uart_read32_register(uint32_t *val,uint32_t reg,uint16_t len)
{
	uint16_t i;
	for(i = 0;i < len;i++)
	{
		*(val + i) = *(volatile uint32_t *)reg;	
		 reg = reg + 4;
	}

}


static void uart_write_register(uint32_t reg,uint32_t value)
{
	*(volatile uint32_t *)reg = value;
}

/*
 * UART HARDWARE
 ****************************************************************************************
 */
void clk_uart0_115200(void)
{
//    GLOBAL_INT_DISABLE();

    sysc_per_clkg0_set(1<<9);
    sysc_per_clk_div_param1_pack(9, 5, 17, 16);
    sysc_per_clk_sel_uart0_setf(1);
    sysc_per_clkg0_set(1<<8);

//    GLOBAL_INT_RESTORE();
}

void clk_uart0_921600(void)
{
//    GLOBAL_INT_DISABLE();

    sysc_per_clkg0_set(1<<9);
    sysc_per_clk_div_param1_pack(12, 2, 1, 2);
    sysc_per_clk_sel_uart0_setf(1);
    sysc_per_clkg0_set(1<<8);

//    GLOBAL_INT_RESTORE();
}

void clk_uart0_2000000(void)
{
//    GLOBAL_INT_DISABLE();

    sysc_per_clkg0_set(1<<9);
    sysc_per_clk_div_param1_pack(0, 0, 0, 0);
    sysc_per_clk_sel_uart0_setf(0);
    sysc_per_clkg0_set(1<<8);

//    GLOBAL_INT_RESTORE();
}

void uart_download_init()
{
    uart_itf.rx_func = uart_download_read;
    uart_itf.tx_func = uart_download_write;
    //clk_uart0_2000000();
    clk_uart0_921600();
    //clk_uart0_115200();
    uart_init(1);

	NVIC_EnableIRQ(UART0_IRQn);  
}

void UART0_IRQHandler()
{
    uart_isr();    
}

