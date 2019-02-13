/*
 * AFS121N Sensor Operation
 *
 * Copyright (C) 2017 Microarray Electronic Co., LTD.
 *
 * Written by:
 *     Hans.Huang  hux@microarray.com.cn
 *     
 * Hans.Huang    09/04/17     Function Defines  and MACROs Define for AFS121N sensor
 *                             
 */


#ifndef _AFS121N_H
#define _AFS121N_H

#include "bx_config.h"
#include "osapp_config.h"
#include "rwip_config.h"
#include <stdbool.h>
#include <stdint.h>
#include "task_init.h"
#include "rwip.h"
#include "bx_dbg.h"
#include "log.h"
#include "arch.h"
#include "ble_task.h"
#include "osapp_task.h"
#include "gap.h"
#include "gapm_task.h"
#include "apollo_00.h"
#include "awo.h"
#include "rtc.h"
#include "log.h"
#include "rst_gen.h"
#include "plf.h"
#include "reg_sysc_awo.h"
#include "sys_sleep.h"
#include "app_uart.h"
#include "m_uart.h"
#include "clk_gate.h"
#include "m_flash.h"




/* Product Code and Company Code defines */
#define AFS121N_PRODUCT_CODE_CMD1  (0x00)
#define AFS121N_PRODUCT_CODE_CMD2  (0x04)
#define AFS121N_COMPANY_CODE_CMD1  (0x08)
#define AFS121N_COMPANY_CODE_CMD2  (0x0C)
#define AFS121N_DUMMY_DATA         (0xFF)

/* AFS121N work mode */
#define AFS121N_POWERDOWN_WORK_MODE (0x80)
#define AFS121N_DETECT_WORK_MODE    (0x84)
#define AFS121N_CAPTURE_WORK_MODE   (0x88)
#define AFS121N_RESET_WORK_MODE     (0x8C)

/* AFS121N bulk read command */
#define AFS121N_LINE_READ_CMD       (0x70)
#define AFS121N_FRAME_READ_CMD      (0x78)


#define AFS121N_READ_BUFFER_LENGTH             (121*121)
#define AFS121N_WRITE_BUFFER_LENGTH            (121*121)

#define AFS121N_READ_WRITE_REGISTER_CMD_LENGTH (4)
#define AFS121N_NONE_PARA_CMD_LENGTH           (1)

#define AFS121N_PRODUCT_COMPANY_CODE_LENGTH    (4)
#define AFS121N_MODE_CMD_LENGTH                (1)
#define AFS121N_RX_DUMMY_DATA                  (32)



/* Function Declares */
void read_product_code(uint8_t *code);
void init_afs121n(void);
void sensorSetMode(const uint8_t mode);
void sensorRWReg(uint8_t addr, uint8_t w_data, uint8_t *old_data, uint8_t *new_data);
void sensorReadLine(uint8_t *buff, uint32_t len);
void sensorReadFrame(uint8_t *buff, uint32_t len);
uint8_t get_touch_status(void);
void sensorInit(void);
void hardwareInit(void);


#endif /* End of _AFS121N_H */
