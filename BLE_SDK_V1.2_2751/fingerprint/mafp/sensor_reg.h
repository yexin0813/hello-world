/*
 * sensor_register.h
 *
 *  Created on: 2016-6-1
 *      Author: zhl
 */

#ifndef __SENSOR_REGISTER_H__
#define __SENSOR_REGISTER_H__

#define MA_PRODU_CODE_1_REG         0x00
#define MA_PRODU_CODE_2_REG         0x04
#define MA_COMPANY_CODE_1_REG       0x08
#define MA_COMPANY_CODE_2_REG       0x0C

#define MA_MAIN_CFG_REG             0x10
#define MA_VOLTAGE_CFG_REG          0x14
#define MA_CAPACITY_CFG_REG         0x18
#define MA_BLANK_REG                0x1C

#define MA_GREY_EXPF_REG            0x20
#define MA_GREY_EXPK_REG            0x24
#define MA_GREY_OFST_REG            0x28
#define MA_GREY_STEP_REG            0x2C

#define MA_START_LINE_REG           0x30
#define MA_END_LINE_REG             0x34
#define MA_LINE_STEP_REG            0x38
#define MA_FRAME_ROWS_REG           0x3c

#define MA_FINGER_DET_PARA_T        0x40
#define MA_FINGER_DET_PARA_C        0x44
#define MA_FINGER_DET_PARA_D        0x48
#define MA_FINGER_DET_PARA_L        0x4C

#define MA_CURRENT_LINE_REG         0x50
#define MA_INTERRUPT_REG            0x60
#define MA_READ_LINE_REG            0x70
#define MA_READ_FRAME_REG           0x78

#define MA_POWER_DOWN               0x80
#define MA_DETECT_MODE              0x84
#define MA_CAPTURE_MODE             0x88
#define MA_RESET_MODE               0x8C

#define MA_DUMMY_BYTE               0xFF

#define MA_REF_CTRL_REG             0x1C
#define MA_FINGER_DET_PARA_N        0x40
#define MA_FINGER_DET_PARA_A        0x44

#define MA_REF_DMY_REG              0x20
#define MA_112UM_GREY_OFST_REG      0x24
#define MA_112UM_FINGER_DET_PARA_L  0x48


#endif
