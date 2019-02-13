//////////////////////////////////////////////////////////////////////////////
#ifndef __REG_SYSC_BLE_H__
#define __REG_SYSC_BLE_H__

//Auto-gen by fr
#include "_reg_base_addr.h"

//adc_rssi_ctrl
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t               adc_rssi_ldo_dly : 10; /* 9: 0, ldo stable cycle number for adc_rssi*/
        uint32_t                     reserved_5 :  2; /*11:10,                             NA*/
        uint32_t               adc_rssi_clk_pol :  1; /*12:12, clock polarity for the output clock of adc_rssi*/
        uint32_t                     reserved_4 :  3; /*15:13,                             NA*/
        uint32_t                adc_rssi_adc_en :  1; /*16:16, agc control enable for the adc_rssi*/
        uint32_t                     reserved_3 :  3; /*19:17,                             NA*/
        uint32_t               adc_rssi_avg_num :  3; /*22:20, average number of the adc_rssi, the output of the rssi value can be the average of multitimes capture, the average number is defined as 2^adc_rssi_avg_num*/
        uint32_t                     reserved_2 :  1; /*23:23,                             NA*/
        uint32_t                   cpu_has_rssi :  1; /*24:24, high means adc_rssi capture is trigerred by the control register: cpu_rssi_req.low means adc_rssi capture is trigerred by the ble mac*/
        uint32_t                     reserved_1 :  3; /*27:25,                             NA*/
        uint32_t                   cpu_rssi_req :  1; /*28:28,       adc_rssi capture request*/
        uint32_t                     reserved_0 :  3; /*31:29,                             NA*/
    } bit_field;
} T_SYSC_BLE_ADC_RSSI_CTRL;

//adc_rssi_ready
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                 adc_rssi_ready :  1; /* 0: 0,                             NA*/
        uint32_t                     reserved_0 : 31; /*31: 1,                             NA*/
    } bit_field;
} T_SYSC_BLE_ADC_RSSI_READY;

//adc_rssi_data
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                  adc_rssi_data :  8; /* 7: 0,          adc_rssi capture data*/
        uint32_t                     reserved_0 : 24; /*31: 8,                             NA*/
    } bit_field;
} T_SYSC_BLE_ADC_RSSI_DATA;

//adc_rssi_fsm
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                   adc_rssi_fsm :  4; /* 3: 0,            the fsm of adc_rssi*/
        uint32_t                     reserved_0 : 28; /*31: 4,                             NA*/
    } bit_field;
} T_SYSC_BLE_ADC_RSSI_FSM;

//rx_cap_len
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_1 : 16; /*15: 0,                             NA*/
        uint32_t                  rx_cap_len_m1 : 13; /*28:16, the capture data length of the rx_cap function*/
        uint32_t                     reserved_0 :  3; /*31:29,                             NA*/
    } bit_field;
} T_SYSC_BLE_RX_CAP_LEN;

//rx_cap_start
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                   rx_cap_start :  1; /* 0: 0,   start of the rx_cap function*/
        uint32_t                     reserved_0 : 31; /*31: 1,                             NA*/
    } bit_field;
} T_SYSC_BLE_RX_CAP_START;

//ble_freq
typedef union
{
    uint32_t val;
    struct
    {
        uint32_t                     reserved_2 :  8; /* 7: 0,                             NA*/
        uint32_t                      ble_freq0 :  6; /*13: 8,                             NA*/
        uint32_t                     reserved_1 :  2; /*15:14,                             NA*/
        uint32_t                      ble_freq1 :  6; /*21:16, write 1 to srst*_clr_* clear the corresponding soft reset control regwrite 1 to srst*_set_* set the corresponding soft reset control reg*/
        uint32_t                     reserved_0 : 10; /*31:22,                             NA*/
    } bit_field;
} T_SYSC_BLE_BLE_FREQ;

//Registers Mapping to the same address

typedef struct
{
    volatile        T_SYSC_BLE_ADC_RSSI_CTRL                  adc_rssi_ctrl; /*  0x0,    RW, 0x00000000,                             NA*/
    volatile       T_SYSC_BLE_ADC_RSSI_READY                 adc_rssi_ready; /*  0x4,    RO, 0x00000000,                             NA*/
    volatile        T_SYSC_BLE_ADC_RSSI_DATA                  adc_rssi_data; /*  0x8,    RO, 0x00000000,                             NA*/
    volatile         T_SYSC_BLE_ADC_RSSI_FSM                   adc_rssi_fsm; /*  0xc,    RO, 0x00000000,                             NA*/
    volatile           T_SYSC_BLE_RX_CAP_LEN                     rx_cap_len; /* 0x10,    RW, 0x00000000,                             NA*/
    volatile         T_SYSC_BLE_RX_CAP_START                   rx_cap_start; /* 0x14,    RW, 0x00000000,                             NA*/
    volatile                        uint32_t                  reserved_0[2];
    volatile             T_SYSC_BLE_BLE_FREQ                       ble_freq; /* 0x20,    RW, 0x00101000,                             NA*/
} T_HWP_SYSC_BLE_T;

#define hwp_sysc_ble ((T_HWP_SYSC_BLE_T*)REG_SYSC_BLE_BASE)


__STATIC_INLINE uint32_t sysc_ble_adc_rssi_ctrl_get(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.val;
}

__STATIC_INLINE void sysc_ble_adc_rssi_ctrl_set(uint32_t value)
{
    hwp_sysc_ble->adc_rssi_ctrl.val = value;
}

__STATIC_INLINE void sysc_ble_adc_rssi_ctrl_pack(uint8_t cpu_rssi_req, uint8_t cpu_has_rssi, uint8_t adc_rssi_avg_num, uint8_t adc_rssi_adc_en, uint8_t adc_rssi_clk_pol, uint16_t adc_rssi_ldo_dly)
{
    hwp_sysc_ble->adc_rssi_ctrl.val = (((uint32_t)cpu_rssi_req << 28) | ((uint32_t)cpu_has_rssi << 24) | ((uint32_t)adc_rssi_avg_num << 20) | ((uint32_t)adc_rssi_adc_en << 16) | ((uint32_t)adc_rssi_clk_pol << 12) | ((uint32_t)adc_rssi_ldo_dly << 0));
}

__STATIC_INLINE void sysc_ble_adc_rssi_ctrl_unpack(uint8_t* cpu_rssi_req, uint8_t* cpu_has_rssi, uint8_t* adc_rssi_avg_num, uint8_t* adc_rssi_adc_en, uint8_t* adc_rssi_clk_pol, uint16_t* adc_rssi_ldo_dly)
{
    T_SYSC_BLE_ADC_RSSI_CTRL localVal = hwp_sysc_ble->adc_rssi_ctrl;

    *cpu_rssi_req = localVal.bit_field.cpu_rssi_req;
    *cpu_has_rssi = localVal.bit_field.cpu_has_rssi;
    *adc_rssi_avg_num = localVal.bit_field.adc_rssi_avg_num;
    *adc_rssi_adc_en = localVal.bit_field.adc_rssi_adc_en;
    *adc_rssi_clk_pol = localVal.bit_field.adc_rssi_clk_pol;
    *adc_rssi_ldo_dly = localVal.bit_field.adc_rssi_ldo_dly;
}

__STATIC_INLINE uint8_t sysc_ble_cpu_rssi_req_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.cpu_rssi_req;
}

__STATIC_INLINE void sysc_ble_cpu_rssi_req_setf(uint8_t cpu_rssi_req)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.cpu_rssi_req = cpu_rssi_req;
}

__STATIC_INLINE uint8_t sysc_ble_cpu_has_rssi_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.cpu_has_rssi;
}

__STATIC_INLINE void sysc_ble_cpu_has_rssi_setf(uint8_t cpu_has_rssi)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.cpu_has_rssi = cpu_has_rssi;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_avg_num_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_avg_num;
}

__STATIC_INLINE void sysc_ble_adc_rssi_avg_num_setf(uint8_t adc_rssi_avg_num)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_avg_num = adc_rssi_avg_num;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_adc_en_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_adc_en;
}

__STATIC_INLINE void sysc_ble_adc_rssi_adc_en_setf(uint8_t adc_rssi_adc_en)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_adc_en = adc_rssi_adc_en;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_clk_pol_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_clk_pol;
}

__STATIC_INLINE void sysc_ble_adc_rssi_clk_pol_setf(uint8_t adc_rssi_clk_pol)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_clk_pol = adc_rssi_clk_pol;
}

__STATIC_INLINE uint16_t sysc_ble_adc_rssi_ldo_dly_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_ldo_dly;
}

__STATIC_INLINE void sysc_ble_adc_rssi_ldo_dly_setf(uint16_t adc_rssi_ldo_dly)
{
    hwp_sysc_ble->adc_rssi_ctrl.bit_field.adc_rssi_ldo_dly = adc_rssi_ldo_dly;
}

__STATIC_INLINE uint32_t sysc_ble_adc_rssi_ready_get(void)
{
    return hwp_sysc_ble->adc_rssi_ready.val;
}

__STATIC_INLINE void sysc_ble_adc_rssi_ready_unpack(uint8_t* adc_rssi_ready)
{
    T_SYSC_BLE_ADC_RSSI_READY localVal = hwp_sysc_ble->adc_rssi_ready;

    *adc_rssi_ready = localVal.bit_field.adc_rssi_ready;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_ready_getf(void)
{
    return hwp_sysc_ble->adc_rssi_ready.bit_field.adc_rssi_ready;
}

__STATIC_INLINE uint32_t sysc_ble_adc_rssi_data_get(void)
{
    return hwp_sysc_ble->adc_rssi_data.val;
}

__STATIC_INLINE void sysc_ble_adc_rssi_data_unpack(uint8_t* adc_rssi_data)
{
    T_SYSC_BLE_ADC_RSSI_DATA localVal = hwp_sysc_ble->adc_rssi_data;

    *adc_rssi_data = localVal.bit_field.adc_rssi_data;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_data_getf(void)
{
    return hwp_sysc_ble->adc_rssi_data.bit_field.adc_rssi_data;
}

__STATIC_INLINE uint32_t sysc_ble_adc_rssi_fsm_get(void)
{
    return hwp_sysc_ble->adc_rssi_fsm.val;
}

__STATIC_INLINE void sysc_ble_adc_rssi_fsm_unpack(uint8_t* adc_rssi_fsm)
{
    T_SYSC_BLE_ADC_RSSI_FSM localVal = hwp_sysc_ble->adc_rssi_fsm;

    *adc_rssi_fsm = localVal.bit_field.adc_rssi_fsm;
}

__STATIC_INLINE uint8_t sysc_ble_adc_rssi_fsm_getf(void)
{
    return hwp_sysc_ble->adc_rssi_fsm.bit_field.adc_rssi_fsm;
}

__STATIC_INLINE uint32_t sysc_ble_rx_cap_len_get(void)
{
    return hwp_sysc_ble->rx_cap_len.val;
}

__STATIC_INLINE void sysc_ble_rx_cap_len_set(uint32_t value)
{
    hwp_sysc_ble->rx_cap_len.val = value;
}

__STATIC_INLINE void sysc_ble_rx_cap_len_pack(uint16_t rx_cap_len_m1)
{
    hwp_sysc_ble->rx_cap_len.val = (((uint32_t)rx_cap_len_m1 << 16));
}

__STATIC_INLINE void sysc_ble_rx_cap_len_unpack(uint16_t* rx_cap_len_m1)
{
    T_SYSC_BLE_RX_CAP_LEN localVal = hwp_sysc_ble->rx_cap_len;

    *rx_cap_len_m1 = localVal.bit_field.rx_cap_len_m1;
}

__STATIC_INLINE uint16_t sysc_ble_rx_cap_len_m1_getf(void)
{
    return hwp_sysc_ble->rx_cap_len.bit_field.rx_cap_len_m1;
}

__STATIC_INLINE void sysc_ble_rx_cap_len_m1_setf(uint16_t rx_cap_len_m1)
{
    hwp_sysc_ble->rx_cap_len.bit_field.rx_cap_len_m1 = rx_cap_len_m1;
}

__STATIC_INLINE uint32_t sysc_ble_rx_cap_start_get(void)
{
    return hwp_sysc_ble->rx_cap_start.val;
}

__STATIC_INLINE void sysc_ble_rx_cap_start_set(uint32_t value)
{
    hwp_sysc_ble->rx_cap_start.val = value;
}

__STATIC_INLINE void sysc_ble_rx_cap_start_pack(uint8_t rx_cap_start)
{
    hwp_sysc_ble->rx_cap_start.val = (((uint32_t)rx_cap_start << 0));
}

__STATIC_INLINE uint32_t sysc_ble_ble_freq_get(void)
{
    return hwp_sysc_ble->ble_freq.val;
}

__STATIC_INLINE void sysc_ble_ble_freq_set(uint32_t value)
{
    hwp_sysc_ble->ble_freq.val = value;
}

__STATIC_INLINE void sysc_ble_ble_freq_pack(uint8_t ble_freq1, uint8_t ble_freq0)
{
    hwp_sysc_ble->ble_freq.val = (((uint32_t)ble_freq1 << 16) | ((uint32_t)ble_freq0 << 8));
}

__STATIC_INLINE void sysc_ble_ble_freq_unpack(uint8_t* ble_freq1, uint8_t* ble_freq0)
{
    T_SYSC_BLE_BLE_FREQ localVal = hwp_sysc_ble->ble_freq;

    *ble_freq1 = localVal.bit_field.ble_freq1;
    *ble_freq0 = localVal.bit_field.ble_freq0;
}

__STATIC_INLINE uint8_t sysc_ble_ble_freq1_getf(void)
{
    return hwp_sysc_ble->ble_freq.bit_field.ble_freq1;
}

__STATIC_INLINE void sysc_ble_ble_freq1_setf(uint8_t ble_freq1)
{
    hwp_sysc_ble->ble_freq.bit_field.ble_freq1 = ble_freq1;
}

__STATIC_INLINE uint8_t sysc_ble_ble_freq0_getf(void)
{
    return hwp_sysc_ble->ble_freq.bit_field.ble_freq0;
}

__STATIC_INLINE void sysc_ble_ble_freq0_setf(uint8_t ble_freq0)
{
    hwp_sysc_ble->ble_freq.bit_field.ble_freq0 = ble_freq0;
}
#endif


