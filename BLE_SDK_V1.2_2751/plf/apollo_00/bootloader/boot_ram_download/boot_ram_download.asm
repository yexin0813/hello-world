
========================================================================

** ELF Header Information

    File Name: .\Objects\boot_ram_download.axf

    Machine class: ELFCLASS32 (32-bit)
    Data encoding: ELFDATA2LSB (Little endian)
    Header version: EV_CURRENT (Current version)
    Operating System ABI: none
    ABI Version: 0
    File Type: ET_EXEC (Executable) (2)
    Machine: EM_ARM (ARM)

    Image Entry point: 0x0012a089
    Flags: EF_ARM_HASENTRY + EF_ARM_ABI_FLOAT_SOFT (0x05000202)

    ARM ELF revision: 5 (ABI version 2)

    Conforms to Soft float procedure-call standard

    Built with
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]

    Header size: 52 bytes (0x34)
    Program header entry size: 32 bytes (0x20)
    Section header entry size: 40 bytes (0x28)

    Program header entries: 1
    Section header entries: 17

    Program header offset: 161696 (0x000277a0)
    Section header offset: 161728 (0x000277c0)

    Section header string table index: 16

========================================================================

** Program header #0

    Type          : PT_LOAD (1)
    File Offset   : 52 (0x34)
    Virtual Addr  : 0x00129ff0
    Physical Addr : 0x00129ff0
    Size in file  : 4748 bytes (0x128c)
    Size in memory: 18104 bytes (0x46b8)
    Flags         : PF_X + PF_W + PF_R + PF_ARM_ENTRY (0x80000007)
    Alignment     : 8


========================================================================

** Section #1

    Name        : BOOT_HEADER_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC (0x00000002)
    Addr        : 0x00129ff0
    File Offset : 52 (0x34)
    Size        : 16 bytes (0x10)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    0x129ff0:   42 58 32 34 00 a0 12 00 ff ff ff ff 9d a0 12 00    BX24............


====================================

** Section #2

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x0012a000
    File Offset : 68 (0x44)
    Size        : 4692 bytes (0x1254)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $d.realdata
    RESET
    __Vectors
        0x0012a000:    0012e6a8    ....    DCD    1238696
        0x0012a004:    0012a09d    ....    DCD    1220765
        0x0012a008:    0012a0b1    ....    DCD    1220785
        0x0012a00c:    0012a0b3    ....    DCD    1220787
        0x0012a010:    0012a0b5    ....    DCD    1220789
        0x0012a014:    0012a0b7    ....    DCD    1220791
        0x0012a018:    0012a0b9    ....    DCD    1220793
        0x0012a01c:    00000000    ....    DCD    0
        0x0012a020:    00000000    ....    DCD    0
        0x0012a024:    00000000    ....    DCD    0
        0x0012a028:    00000000    ....    DCD    0
        0x0012a02c:    0012a0bb    ....    DCD    1220795
        0x0012a030:    0012a0bd    ....    DCD    1220797
        0x0012a034:    00000000    ....    DCD    0
        0x0012a038:    0012a0bf    ....    DCD    1220799
        0x0012a03c:    0012a0c1    ....    DCD    1220801
        0x0012a040:    0012a0c3    ....    DCD    1220803
        0x0012a044:    0012a0c3    ....    DCD    1220803
        0x0012a048:    0012a0c3    ....    DCD    1220803
        0x0012a04c:    0012a0c3    ....    DCD    1220803
        0x0012a050:    0012a0c3    ....    DCD    1220803
        0x0012a054:    0012a0c3    ....    DCD    1220803
        0x0012a058:    0012a0c3    ....    DCD    1220803
        0x0012a05c:    0012a0c3    ....    DCD    1220803
        0x0012a060:    0012a0c3    ....    DCD    1220803
        0x0012a064:    0012a0c3    ....    DCD    1220803
        0x0012a068:    0012a0c3    ....    DCD    1220803
        0x0012a06c:    0012a151    Q...    DCD    1220945
        0x0012a070:    0012a0c3    ....    DCD    1220803
        0x0012a074:    0012a0c3    ....    DCD    1220803
        0x0012a078:    0012a0c3    ....    DCD    1220803
        0x0012a07c:    0012a0c3    ....    DCD    1220803
        0x0012a080:    0012a0c3    ....    DCD    1220803
        0x0012a084:    0012a0c3    ....    DCD    1220803
    $t
    .ARM.Collect$$$$00000000
    .ARM.Collect$$$$00000001
    __Vectors_End
    __main
    _main_stk
        0x0012a088:    4803        .H      LDR      r0,__lit__00000000 ; [0x12a098] = 0x12e6a8
        0x0012a08a:    4685        .F      MOV      sp,r0
    .ARM.Collect$$$$00000004
    _main_scatterload
        0x0012a08c:    f000f836    ..6.    BL       __scatterload ; 0x12a0fc
    .ARM.Collect$$$$00000008
    .ARM.Collect$$$$0000000A
    .ARM.Collect$$$$0000000B
    __main_after_scatterload
    _main_clock
    _main_cpp_init
    _main_init
        0x0012a090:    4800        .H      LDR      r0,[pc,#0] ; [0x12a094] = 0x12a705
        0x0012a092:    4700        .G      BX       r0
    $d
        0x0012a094:    0012a705    ....    DCD    1222405
    .ARM.Collect$$$$00002712
    __lit__00000000
    .ARM.Collect$$$$0000000D
    .ARM.Collect$$$$0000000F
    __rt_final_cpp
    __rt_final_exit
        0x0012a098:    0012e6a8    ....    DCD    1238696
    $t
    .text
    Reset_Handler
        0x0012a09c:    4809        .H      LDR      r0,[pc,#36] ; [0x12a0c4] = 0x12a121
        0x0012a09e:    4780        .G      BLX      r0
        0x0012a0a0:    4809        .H      LDR      r0,[pc,#36] ; [0x12a0c8] = 0x12a089
        0x0012a0a2:    4700        .G      BX       r0
    exception_exit
        0x0012a0a4:    4809        .H      LDR      r0,[pc,#36] ; [0x12a0cc] = 0x12a09d
        0x0012a0a6:    9006        ..      STR      r0,[sp,#0x18]
        0x0012a0a8:    4809        .H      LDR      r0,[pc,#36] ; [0x12a0d0] = 0xf1000000
        0x0012a0aa:    9007        ..      STR      r0,[sp,#0x1c]
        0x0012a0ac:    4809        .H      LDR      r0,[pc,#36] ; [0x12a0d4] = 0xfffffff9
        0x0012a0ae:    4700        .G      BX       r0
    NMI_Handler
        0x0012a0b0:    e7fe        ..      B        NMI_Handler ; 0x12a0b0
    HardFault_Handler
        0x0012a0b2:    e7fe        ..      B        HardFault_Handler ; 0x12a0b2
    MemManage_Handler
        0x0012a0b4:    e7fe        ..      B        MemManage_Handler ; 0x12a0b4
    BusFault_Handler
        0x0012a0b6:    e7fe        ..      B        BusFault_Handler ; 0x12a0b6
    UsageFault_Handler
        0x0012a0b8:    e7fe        ..      B        UsageFault_Handler ; 0x12a0b8
    SVC_Handler
        0x0012a0ba:    e7fe        ..      B        SVC_Handler ; 0x12a0ba
    DebugMon_Handler
        0x0012a0bc:    e7fe        ..      B        DebugMon_Handler ; 0x12a0bc
    PendSV_Handler
        0x0012a0be:    e7fe        ..      B        PendSV_Handler ; 0x12a0be
    SysTick_Handler
        0x0012a0c0:    e7fe        ..      B        SysTick_Handler ; 0x12a0c0
    BLE_LP_IRQHandler
    BLE_MAC_IRQHandler
    DMAC_IRQHandler
    ECC_IRQHandler
    EXT_INTR_IRQHandler
    GPIO_IRQHandler
    IIC0_IRQHandler
    IIC1_IRQHandler
    QSPI_IRQHandler
    RTC_IRQHandler
    SFT_IRQHandler
    SPIM0_IRQHandler
    SPIM1_IRQHandler
    SPIS_IRQHandler
    TIMER_IRQHandler
    UART1_IRQHandler
    WDT_IRQHandler
        0x0012a0c2:    e7fe        ..      B        BLE_LP_IRQHandler ; 0x12a0c2
    $d
        0x0012a0c4:    0012a121    !...    DCD    1220897
        0x0012a0c8:    0012a089    ....    DCD    1220745
        0x0012a0cc:    0012a09d    ....    DCD    1220765
        0x0012a0d0:    f1000000    ....    DCD    4043309056
        0x0012a0d4:    fffffff9    ....    DCD    4294967289
    $t
    .text
    __aeabi_memcpy
    __aeabi_memcpy4
    __aeabi_memcpy8
        0x0012a0d8:    4603        .F      MOV      r3,r0
        0x0012a0da:    430b        .C      ORRS     r3,r3,r1
        0x0012a0dc:    079b        ..      LSLS     r3,r3,#30
        0x0012a0de:    d003        ..      BEQ      0x12a0e8 ; __aeabi_memcpy + 16
        0x0012a0e0:    e009        ..      B        0x12a0f6 ; __aeabi_memcpy + 30
        0x0012a0e2:    c908        ..      LDM      r1!,{r3}
        0x0012a0e4:    1f12        ..      SUBS     r2,r2,#4
        0x0012a0e6:    c008        ..      STM      r0!,{r3}
        0x0012a0e8:    2a04        .*      CMP      r2,#4
        0x0012a0ea:    d2fa        ..      BCS      0x12a0e2 ; __aeabi_memcpy + 10
        0x0012a0ec:    e003        ..      B        0x12a0f6 ; __aeabi_memcpy + 30
        0x0012a0ee:    780b        .x      LDRB     r3,[r1,#0]
        0x0012a0f0:    7003        .p      STRB     r3,[r0,#0]
        0x0012a0f2:    1c40        @.      ADDS     r0,r0,#1
        0x0012a0f4:    1c49        I.      ADDS     r1,r1,#1
        0x0012a0f6:    1e52        R.      SUBS     r2,r2,#1
        0x0012a0f8:    d2f9        ..      BCS      0x12a0ee ; __aeabi_memcpy + 22
        0x0012a0fa:    4770        pG      BX       lr
    .text
    __scatterload
    __scatterload_rt2
        0x0012a0fc:    4c06        .L      LDR      r4,[pc,#24] ; [0x12a118] = 0x12b244
        0x0012a0fe:    2501        .%      MOVS     r5,#1
        0x0012a100:    4e06        .N      LDR      r6,[pc,#24] ; [0x12a11c] = 0x12b254
        0x0012a102:    e005        ..      B        0x12a110 ; __scatterload + 20
        0x0012a104:    68e3        .h      LDR      r3,[r4,#0xc]
        0x0012a106:    cc07        ..      LDM      r4!,{r0-r2}
        0x0012a108:    432b        +C      ORRS     r3,r3,r5
        0x0012a10a:    3c0c        .<      SUBS     r4,r4,#0xc
        0x0012a10c:    4798        .G      BLX      r3
        0x0012a10e:    3410        .4      ADDS     r4,r4,#0x10
        0x0012a110:    42b4        .B      CMP      r4,r6
        0x0012a112:    d3f7        ..      BCC      0x12a104 ; __scatterload + 8
        0x0012a114:    f7ffffbc    ....    BL       __main_after_scatterload ; 0x12a090
    $d
        0x0012a118:    0012b244    D...    DCD    1225284
        0x0012a11c:    0012b254    T...    DCD    1225300
    $t
    i.SystemInit
    SystemInit
        0x0012a120:    b530        0.      PUSH     {r4,r5,lr}
        0x0012a122:    bf00        ..      NOP      
        0x0012a124:    bf00        ..      NOP      
        0x0012a126:    bf00        ..      NOP      
        0x0012a128:    f3bf8f5f    .._.    DMB      
        0x0012a12c:    bf00        ..      NOP      
        0x0012a12e:    bf00        ..      NOP      
        0x0012a130:    bf00        ..      NOP      
        0x0012a132:    2495        .$      MOVS     r4,#0x95
        0x0012a134:    0364        d.      LSLS     r4,r4,#13
        0x0012a136:    4d05        .M      LDR      r5,[pc,#20] ; [0x12a14c] = 0xe000ed00
        0x0012a138:    60ac        .`      STR      r4,[r5,#8]
        0x0012a13a:    bf00        ..      NOP      
        0x0012a13c:    bf00        ..      NOP      
        0x0012a13e:    bf00        ..      NOP      
        0x0012a140:    f3bf8f4f    ..O.    DSB      
        0x0012a144:    bf00        ..      NOP      
        0x0012a146:    bf00        ..      NOP      
        0x0012a148:    bf00        ..      NOP      
        0x0012a14a:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x0012a14c:    e000ed00    ....    DCD    3758157056
    $t
    i.UART0_IRQHandler
    UART0_IRQHandler
        0x0012a150:    b510        ..      PUSH     {r4,lr}
        0x0012a152:    f000fd61    ..a.    BL       uart_isr ; 0x12ac18
        0x0012a156:    bd10        ..      POP      {r4,pc}
    i.__scatterload_copy
    __scatterload_copy
        0x0012a158:    e002        ..      B        0x12a160 ; __scatterload_copy + 8
        0x0012a15a:    c808        ..      LDM      r0!,{r3}
        0x0012a15c:    1f12        ..      SUBS     r2,r2,#4
        0x0012a15e:    c108        ..      STM      r1!,{r3}
        0x0012a160:    2a00        .*      CMP      r2,#0
        0x0012a162:    d1fa        ..      BNE      0x12a15a ; __scatterload_copy + 2
        0x0012a164:    4770        pG      BX       lr
    i.__scatterload_null
    __scatterload_null
        0x0012a166:    4770        pG      BX       lr
    i.__scatterload_zeroinit
    __scatterload_zeroinit
        0x0012a168:    2000        .       MOVS     r0,#0
        0x0012a16a:    e001        ..      B        0x12a170 ; __scatterload_zeroinit + 8
        0x0012a16c:    c101        ..      STM      r1!,{r0}
        0x0012a16e:    1f12        ..      SUBS     r2,r2,#4
        0x0012a170:    2a00        .*      CMP      r2,#0
        0x0012a172:    d1fb        ..      BNE      0x12a16c ; __scatterload_zeroinit + 4
        0x0012a174:    4770        pG      BX       lr
        0x0012a176:    0000        ..      MOVS     r0,r0
    i.clk_gate_per_g0
    clk_gate_per_g0
        0x0012a178:    bf00        ..      NOP      
        0x0012a17a:    490f        .I      LDR      r1,[pc,#60] ; [0x12a1b8] = 0x20149000
        0x0012a17c:    6108        .a      STR      r0,[r1,#0x10]
        0x0012a17e:    bf00        ..      NOP      
        0x0012a180:    490e        .I      LDR      r1,[pc,#56] ; [0x12a1bc] = 0xaaaaaaaa
        0x0012a182:    4001        .@      ANDS     r1,r1,r0
        0x0012a184:    2900        .)      CMP      r1,#0
        0x0012a186:    d007        ..      BEQ      0x12a198 ; clk_gate_per_g0 + 32
        0x0012a188:    bf00        ..      NOP      
        0x0012a18a:    bf00        ..      NOP      
        0x0012a18c:    490a        .I      LDR      r1,[pc,#40] ; [0x12a1b8] = 0x20149000
        0x0012a18e:    6909        .i      LDR      r1,[r1,#0x10]
        0x0012a190:    4001        .@      ANDS     r1,r1,r0
        0x0012a192:    2900        .)      CMP      r1,#0
        0x0012a194:    d1f9        ..      BNE      0x12a18a ; clk_gate_per_g0 + 18
        0x0012a196:    e00e        ..      B        0x12a1b6 ; clk_gate_per_g0 + 62
        0x0012a198:    4908        .I      LDR      r1,[pc,#32] ; [0x12a1bc] = 0xaaaaaaaa
        0x0012a19a:    43c9        .C      MVNS     r1,r1
        0x0012a19c:    4001        .@      ANDS     r1,r1,r0
        0x0012a19e:    2900        .)      CMP      r1,#0
        0x0012a1a0:    d008        ..      BEQ      0x12a1b4 ; clk_gate_per_g0 + 60
        0x0012a1a2:    bf00        ..      NOP      
        0x0012a1a4:    bf00        ..      NOP      
        0x0012a1a6:    4904        .I      LDR      r1,[pc,#16] ; [0x12a1b8] = 0x20149000
        0x0012a1a8:    6909        .i      LDR      r1,[r1,#0x10]
        0x0012a1aa:    4602        .F      MOV      r2,r0
        0x0012a1ac:    438a        .C      BICS     r2,r2,r1
        0x0012a1ae:    2a00        .*      CMP      r2,#0
        0x0012a1b0:    d1f8        ..      BNE      0x12a1a4 ; clk_gate_per_g0 + 44
        0x0012a1b2:    e000        ..      B        0x12a1b6 ; clk_gate_per_g0 + 62
        0x0012a1b4:    bf00        ..      NOP      
        0x0012a1b6:    4770        pG      BX       lr
    $d
        0x0012a1b8:    20149000    ...     DCD    538218496
        0x0012a1bc:    aaaaaaaa    ....    DCD    2863311530
    $t
    i.clk_uart0_921600
    clk_uart0_921600
        0x0012a1c0:    b510        ..      PUSH     {r4,lr}
        0x0012a1c2:    2001        .       MOVS     r0,#1
        0x0012a1c4:    0240        @.      LSLS     r0,r0,#9
        0x0012a1c6:    f000fc41    ..A.    BL       sysc_per_clkg0_set ; 0x12aa4c
        0x0012a1ca:    2302        .#      MOVS     r3,#2
        0x0012a1cc:    2201        ."      MOVS     r2,#1
        0x0012a1ce:    4619        .F      MOV      r1,r3
        0x0012a1d0:    200c        .       MOVS     r0,#0xc
        0x0012a1d2:    f000fbcb    ....    BL       sysc_per_clk_div_param1_pack ; 0x12a96c
        0x0012a1d6:    2001        .       MOVS     r0,#1
        0x0012a1d8:    f000fbd4    ....    BL       sysc_per_clk_sel_uart0_setf ; 0x12a984
        0x0012a1dc:    20ff        .       MOVS     r0,#0xff
        0x0012a1de:    3001        .0      ADDS     r0,#1
        0x0012a1e0:    f000fc34    ..4.    BL       sysc_per_clkg0_set ; 0x12aa4c
        0x0012a1e4:    bd10        ..      POP      {r4,pc}
        0x0012a1e6:    0000        ..      MOVS     r0,r0
    i.dmac_block_ts_config
    dmac_block_ts_config
        0x0012a1e8:    b530        0.      PUSH     {r4,r5,lr}
        0x0012a1ea:    4a15        .J      LDR      r2,[pc,#84] ; [0x12a240] = 0xfff
        0x0012a1ec:    4290        .B      CMP      r0,r2
        0x0012a1ee:    d913        ..      BLS      0x12a218 ; dmac_block_ts_config + 48
        0x0012a1f0:    bf00        ..      NOP      
        0x0012a1f2:    2458        X$      MOVS     r4,#0x58
        0x0012a1f4:    434c        LC      MULS     r4,r1,r4
        0x0012a1f6:    4d13        .M      LDR      r5,[pc,#76] ; [0x12a244] = 0x20120000
        0x0012a1f8:    1964        d.      ADDS     r4,r4,r5
        0x0012a1fa:    69e2        .i      LDR      r2,[r4,#0x1c]
        0x0012a1fc:    0b14        ..      LSRS     r4,r2,#12
        0x0012a1fe:    0324        $.      LSLS     r4,r4,#12
        0x0012a200:    4d0f        .M      LDR      r5,[pc,#60] ; [0x12a240] = 0xfff
        0x0012a202:    432c        ,C      ORRS     r4,r4,r5
        0x0012a204:    4623        #F      MOV      r3,r4
        0x0012a206:    2458        X$      MOVS     r4,#0x58
        0x0012a208:    434c        LC      MULS     r4,r1,r4
        0x0012a20a:    4d0e        .M      LDR      r5,[pc,#56] ; [0x12a244] = 0x20120000
        0x0012a20c:    1964        d.      ADDS     r4,r4,r5
        0x0012a20e:    61e3        .a      STR      r3,[r4,#0x1c]
        0x0012a210:    bf00        ..      NOP      
        0x0012a212:    4a0b        .J      LDR      r2,[pc,#44] ; [0x12a240] = 0xfff
        0x0012a214:    1a80        ..      SUBS     r0,r0,r2
        0x0012a216:    e012        ..      B        0x12a23e ; dmac_block_ts_config + 86
        0x0012a218:    bf00        ..      NOP      
        0x0012a21a:    2458        X$      MOVS     r4,#0x58
        0x0012a21c:    434c        LC      MULS     r4,r1,r4
        0x0012a21e:    4d09        .M      LDR      r5,[pc,#36] ; [0x12a244] = 0x20120000
        0x0012a220:    1964        d.      ADDS     r4,r4,r5
        0x0012a222:    69e2        .i      LDR      r2,[r4,#0x1c]
        0x0012a224:    0504        ..      LSLS     r4,r0,#20
        0x0012a226:    0d24        $.      LSRS     r4,r4,#20
        0x0012a228:    0b15        ..      LSRS     r5,r2,#12
        0x0012a22a:    032d        -.      LSLS     r5,r5,#12
        0x0012a22c:    432c        ,C      ORRS     r4,r4,r5
        0x0012a22e:    4623        #F      MOV      r3,r4
        0x0012a230:    2458        X$      MOVS     r4,#0x58
        0x0012a232:    434c        LC      MULS     r4,r1,r4
        0x0012a234:    4d03        .M      LDR      r5,[pc,#12] ; [0x12a244] = 0x20120000
        0x0012a236:    1964        d.      ADDS     r4,r4,r5
        0x0012a238:    61e3        .a      STR      r3,[r4,#0x1c]
        0x0012a23a:    bf00        ..      NOP      
        0x0012a23c:    2000        .       MOVS     r0,#0
        0x0012a23e:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x0012a240:    00000fff    ....    DCD    4095
        0x0012a244:    20120000    ...     DCD    538050560
    $t
    i.dmac_init
    dmac_init
        0x0012a248:    2001        .       MOVS     r0,#1
        0x0012a24a:    4903        .I      LDR      r1,[pc,#12] ; [0x12a258] = 0x20120380
        0x0012a24c:    6188        .a      STR      r0,[r1,#0x18]
        0x0012a24e:    20ff        .       MOVS     r0,#0xff
        0x0012a250:    4901        .I      LDR      r1,[pc,#4] ; [0x12a258] = 0x20120380
        0x0012a252:    3980        .9      SUBS     r1,r1,#0x80
        0x0012a254:    6388        .c      STR      r0,[r1,#0x38]
        0x0012a256:    4770        pG      BX       lr
    $d
        0x0012a258:    20120380    ...     DCD    538051456
    $t
    i.dmac_start
    dmac_start
        0x0012a25c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012a25e:    4604        .F      MOV      r4,r0
        0x0012a260:    460d        .F      MOV      r5,r1
        0x0012a262:    4629        )F      MOV      r1,r5
        0x0012a264:    68a0        .h      LDR      r0,[r4,#8]
        0x0012a266:    f7ffffbf    ....    BL       dmac_block_ts_config ; 0x12a1e8
        0x0012a26a:    9000        ..      STR      r0,[sp,#0]
        0x0012a26c:    89a0        ..      LDRH     r0,[r4,#0xc]
        0x0012a26e:    0440        @.      LSLS     r0,r0,#17
        0x0012a270:    0f40        @.      LSRS     r0,r0,#29
        0x0012a272:    2800        .(      CMP      r0,#0
        0x0012a274:    d009        ..      BEQ      0x12a28a ; dmac_start + 46
        0x0012a276:    2801        .(      CMP      r0,#1
        0x0012a278:    d004        ..      BEQ      0x12a284 ; dmac_start + 40
        0x0012a27a:    2802        .(      CMP      r0,#2
        0x0012a27c:    d108        ..      BNE      0x12a290 ; dmac_start + 52
        0x0012a27e:    2602        .&      MOVS     r6,#2
        0x0012a280:    2700        .'      MOVS     r7,#0
        0x0012a282:    e006        ..      B        0x12a292 ; dmac_start + 54
        0x0012a284:    2600        .&      MOVS     r6,#0
        0x0012a286:    2702        .'      MOVS     r7,#2
        0x0012a288:    e003        ..      B        0x12a292 ; dmac_start + 54
        0x0012a28a:    2600        .&      MOVS     r6,#0
        0x0012a28c:    2700        .'      MOVS     r7,#0
        0x0012a28e:    e000        ..      B        0x12a292 ; dmac_start + 54
        0x0012a290:    bf00        ..      NOP      
        0x0012a292:    bf00        ..      NOP      
        0x0012a294:    89a0        ..      LDRH     r0,[r4,#0xc]
        0x0012a296:    0440        @.      LSLS     r0,r0,#17
        0x0012a298:    0f40        @.      LSRS     r0,r0,#29
        0x0012a29a:    0500        ..      LSLS     r0,r0,#20
        0x0012a29c:    2103        .!      MOVS     r1,#3
        0x0012a29e:    0509        ..      LSLS     r1,r1,#20
        0x0012a2a0:    4008        .@      ANDS     r0,r0,r1
        0x0012a2a2:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012a2a4:    05c9        ..      LSLS     r1,r1,#23
        0x0012a2a6:    0f49        I.      LSRS     r1,r1,#29
        0x0012a2a8:    0389        ..      LSLS     r1,r1,#14
        0x0012a2aa:    2207        ."      MOVS     r2,#7
        0x0012a2ac:    0392        ..      LSLS     r2,r2,#14
        0x0012a2ae:    4011        .@      ANDS     r1,r1,r2
        0x0012a2b0:    4308        .C      ORRS     r0,r0,r1
        0x0012a2b2:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012a2b4:    0509        ..      LSLS     r1,r1,#20
        0x0012a2b6:    0f49        I.      LSRS     r1,r1,#29
        0x0012a2b8:    02c9        ..      LSLS     r1,r1,#11
        0x0012a2ba:    10d2        ..      ASRS     r2,r2,#3
        0x0012a2bc:    4011        .@      ANDS     r1,r1,r2
        0x0012a2be:    4308        .C      ORRS     r0,r0,r1
        0x0012a2c0:    0271        q.      LSLS     r1,r6,#9
        0x0012a2c2:    2203        ."      MOVS     r2,#3
        0x0012a2c4:    0252        R.      LSLS     r2,r2,#9
        0x0012a2c6:    4011        .@      ANDS     r1,r1,r2
        0x0012a2c8:    4308        .C      ORRS     r0,r0,r1
        0x0012a2ca:    01f9        ..      LSLS     r1,r7,#7
        0x0012a2cc:    1092        ..      ASRS     r2,r2,#2
        0x0012a2ce:    4011        .@      ANDS     r1,r1,r2
        0x0012a2d0:    4308        .C      ORRS     r0,r0,r1
        0x0012a2d2:    68e1        .h      LDR      r1,[r4,#0xc]
        0x0012a2d4:    0109        ..      LSLS     r1,r1,#4
        0x0012a2d6:    2270        p"      MOVS     r2,#0x70
        0x0012a2d8:    4011        .@      ANDS     r1,r1,r2
        0x0012a2da:    4308        .C      ORRS     r0,r0,r1
        0x0012a2dc:    7b21        !{      LDRB     r1,[r4,#0xc]
        0x0012a2de:    0689        ..      LSLS     r1,r1,#26
        0x0012a2e0:    0f49        I.      LSRS     r1,r1,#29
        0x0012a2e2:    0049        I.      LSLS     r1,r1,#1
        0x0012a2e4:    220e        ."      MOVS     r2,#0xe
        0x0012a2e6:    4011        .@      ANDS     r1,r1,r2
        0x0012a2e8:    4308        .C      ORRS     r0,r0,r1
        0x0012a2ea:    2158        X!      MOVS     r1,#0x58
        0x0012a2ec:    4369        iC      MULS     r1,r5,r1
        0x0012a2ee:    4a1b        .J      LDR      r2,[pc,#108] ; [0x12a35c] = 0x20120000
        0x0012a2f0:    1889        ..      ADDS     r1,r1,r2
        0x0012a2f2:    6188        .a      STR      r0,[r1,#0x18]
        0x0012a2f4:    2100        .!      MOVS     r1,#0
        0x0012a2f6:    2058        X       MOVS     r0,#0x58
        0x0012a2f8:    4368        hC      MULS     r0,r5,r0
        0x0012a2fa:    1880        ..      ADDS     r0,r0,r2
        0x0012a2fc:    6401        .d      STR      r1,[r0,#0x40]
        0x0012a2fe:    68e0        .h      LDR      r0,[r4,#0xc]
        0x0012a300:    0240        @.      LSLS     r0,r0,#9
        0x0012a302:    0f00        ..      LSRS     r0,r0,#28
        0x0012a304:    02c0        ..      LSLS     r0,r0,#11
        0x0012a306:    210f        .!      MOVS     r1,#0xf
        0x0012a308:    02c9        ..      LSLS     r1,r1,#11
        0x0012a30a:    4008        .@      ANDS     r0,r0,r1
        0x0012a30c:    68e1        .h      LDR      r1,[r4,#0xc]
        0x0012a30e:    0349        I.      LSLS     r1,r1,#13
        0x0012a310:    0f09        ..      LSRS     r1,r1,#28
        0x0012a312:    01c9        ..      LSLS     r1,r1,#7
        0x0012a314:    220f        ."      MOVS     r2,#0xf
        0x0012a316:    01d2        ..      LSLS     r2,r2,#7
        0x0012a318:    4011        .@      ANDS     r1,r1,r2
        0x0012a31a:    4308        .C      ORRS     r0,r0,r1
        0x0012a31c:    1c80        ..      ADDS     r0,r0,#2
        0x0012a31e:    2158        X!      MOVS     r1,#0x58
        0x0012a320:    4369        iC      MULS     r1,r5,r1
        0x0012a322:    4a0e        .J      LDR      r2,[pc,#56] ; [0x12a35c] = 0x20120000
        0x0012a324:    1889        ..      ADDS     r1,r1,r2
        0x0012a326:    6448        Hd      STR      r0,[r1,#0x44]
        0x0012a328:    2058        X       MOVS     r0,#0x58
        0x0012a32a:    4368        hC      MULS     r0,r5,r0
        0x0012a32c:    1880        ..      ADDS     r0,r0,r2
        0x0012a32e:    6821        !h      LDR      r1,[r4,#0]
        0x0012a330:    6001        .`      STR      r1,[r0,#0]
        0x0012a332:    2058        X       MOVS     r0,#0x58
        0x0012a334:    4368        hC      MULS     r0,r5,r0
        0x0012a336:    1880        ..      ADDS     r0,r0,r2
        0x0012a338:    6861        ah      LDR      r1,[r4,#4]
        0x0012a33a:    6081        .`      STR      r1,[r0,#8]
        0x0012a33c:    2001        .       MOVS     r0,#1
        0x0012a33e:    40a8        .@      LSLS     r0,r0,r5
        0x0012a340:    0680        ..      LSLS     r0,r0,#26
        0x0012a342:    0e80        ..      LSRS     r0,r0,#26
        0x0012a344:    2101        .!      MOVS     r1,#1
        0x0012a346:    40a9        .@      LSLS     r1,r1,r5
        0x0012a348:    0209        ..      LSLS     r1,r1,#8
        0x0012a34a:    223f        ?"      MOVS     r2,#0x3f
        0x0012a34c:    0212        ..      LSLS     r2,r2,#8
        0x0012a34e:    4011        .@      ANDS     r1,r1,r2
        0x0012a350:    4308        .C      ORRS     r0,r0,r1
        0x0012a352:    4903        .I      LDR      r1,[pc,#12] ; [0x12a360] = 0x20120380
        0x0012a354:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a356:    9800        ..      LDR      r0,[sp,#0]
        0x0012a358:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012a35a:    0000        ..      DCW    0
        0x0012a35c:    20120000    ...     DCD    538050560
        0x0012a360:    20120380    ...     DCD    538051456
    $t
    i.dmac_transfer_wait
    dmac_transfer_wait
        0x0012a364:    b570        p.      PUSH     {r4-r6,lr}
        0x0012a366:    4605        .F      MOV      r5,r0
        0x0012a368:    460c        .F      MOV      r4,r1
        0x0012a36a:    bf00        ..      NOP      
        0x0012a36c:    4817        .H      LDR      r0,[pc,#92] ; [0x12a3cc] = 0x201202c0
        0x0012a36e:    6800        .h      LDR      r0,[r0,#0]
        0x0012a370:    2101        .!      MOVS     r1,#1
        0x0012a372:    40a1        .@      LSLS     r1,r1,r4
        0x0012a374:    4008        .@      ANDS     r0,r0,r1
        0x0012a376:    2800        .(      CMP      r0,#0
        0x0012a378:    d0f8        ..      BEQ      0x12a36c ; dmac_transfer_wait + 8
        0x0012a37a:    2001        .       MOVS     r0,#1
        0x0012a37c:    40a0        .@      LSLS     r0,r0,r4
        0x0012a37e:    4913        .I      LDR      r1,[pc,#76] ; [0x12a3cc] = 0x201202c0
        0x0012a380:    3140        @1      ADDS     r1,r1,#0x40
        0x0012a382:    6388        .c      STR      r0,[r1,#0x38]
        0x0012a384:    e01f        ..      B        0x12a3c6 ; dmac_transfer_wait + 98
        0x0012a386:    4621        !F      MOV      r1,r4
        0x0012a388:    4628        (F      MOV      r0,r5
        0x0012a38a:    f7ffff2d    ..-.    BL       dmac_block_ts_config ; 0x12a1e8
        0x0012a38e:    4605        .F      MOV      r5,r0
        0x0012a390:    2001        .       MOVS     r0,#1
        0x0012a392:    40a0        .@      LSLS     r0,r0,r4
        0x0012a394:    0680        ..      LSLS     r0,r0,#26
        0x0012a396:    0e80        ..      LSRS     r0,r0,#26
        0x0012a398:    2101        .!      MOVS     r1,#1
        0x0012a39a:    40a1        .@      LSLS     r1,r1,r4
        0x0012a39c:    0209        ..      LSLS     r1,r1,#8
        0x0012a39e:    223f        ?"      MOVS     r2,#0x3f
        0x0012a3a0:    0212        ..      LSLS     r2,r2,#8
        0x0012a3a2:    4011        .@      ANDS     r1,r1,r2
        0x0012a3a4:    4308        .C      ORRS     r0,r0,r1
        0x0012a3a6:    4909        .I      LDR      r1,[pc,#36] ; [0x12a3cc] = 0x201202c0
        0x0012a3a8:    31c0        .1      ADDS     r1,r1,#0xc0
        0x0012a3aa:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a3ac:    bf00        ..      NOP      
        0x0012a3ae:    4807        .H      LDR      r0,[pc,#28] ; [0x12a3cc] = 0x201202c0
        0x0012a3b0:    6800        .h      LDR      r0,[r0,#0]
        0x0012a3b2:    2101        .!      MOVS     r1,#1
        0x0012a3b4:    40a1        .@      LSLS     r1,r1,r4
        0x0012a3b6:    4008        .@      ANDS     r0,r0,r1
        0x0012a3b8:    2800        .(      CMP      r0,#0
        0x0012a3ba:    d0f8        ..      BEQ      0x12a3ae ; dmac_transfer_wait + 74
        0x0012a3bc:    2001        .       MOVS     r0,#1
        0x0012a3be:    40a0        .@      LSLS     r0,r0,r4
        0x0012a3c0:    4902        .I      LDR      r1,[pc,#8] ; [0x12a3cc] = 0x201202c0
        0x0012a3c2:    3140        @1      ADDS     r1,r1,#0x40
        0x0012a3c4:    6388        .c      STR      r0,[r1,#0x38]
        0x0012a3c6:    2d00        .-      CMP      r5,#0
        0x0012a3c8:    d1dd        ..      BNE      0x12a386 ; dmac_transfer_wait + 34
        0x0012a3ca:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012a3cc:    201202c0    ...     DCD    538051264
    $t
    i.flash_erase
    flash_erase
        0x0012a3d0:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012a3d2:    4604        .F      MOV      r4,r0
        0x0012a3d4:    460d        .F      MOV      r5,r1
        0x0012a3d6:    f000f98d    ....    BL       flash_write_enable ; 0x12a6f4
        0x0012a3da:    2d03        .-      CMP      r5,#3
        0x0012a3dc:    d108        ..      BNE      0x12a3f0 ; flash_erase + 32
        0x0012a3de:    a022        ".      ADR      r0,{pc}+0x8a ; 0x12a468
        0x0012a3e0:    6800        .h      LDR      r0,[r0,#0]
        0x0012a3e2:    9000        ..      STR      r0,[sp,#0]
        0x0012a3e4:    2601        .&      MOVS     r6,#1
        0x0012a3e6:    4631        1F      MOV      r1,r6
        0x0012a3e8:    4668        hF      MOV      r0,sp
        0x0012a3ea:    f000fa95    ....    BL       qspi_std_write ; 0x12a918
        0x0012a3ee:    e036        6.      B        0x12a45e ; flash_erase + 142
        0x0012a3f0:    2d02        .-      CMP      r5,#2
        0x0012a3f2:    d110        ..      BNE      0x12a416 ; flash_erase + 70
        0x0012a3f4:    a01d        ..      ADR      r0,{pc}+0x78 ; 0x12a46c
        0x0012a3f6:    6800        .h      LDR      r0,[r0,#0]
        0x0012a3f8:    9000        ..      STR      r0,[sp,#0]
        0x0012a3fa:    0220         .      LSLS     r0,r4,#8
        0x0012a3fc:    0e01        ..      LSRS     r1,r0,#24
        0x0012a3fe:    4668        hF      MOV      r0,sp
        0x0012a400:    7041        Ap      STRB     r1,[r0,#1]
        0x0012a402:    0420         .      LSLS     r0,r4,#16
        0x0012a404:    0e01        ..      LSRS     r1,r0,#24
        0x0012a406:    4668        hF      MOV      r0,sp
        0x0012a408:    7081        .p      STRB     r1,[r0,#2]
        0x0012a40a:    70c4        .p      STRB     r4,[r0,#3]
        0x0012a40c:    2604        .&      MOVS     r6,#4
        0x0012a40e:    4631        1F      MOV      r1,r6
        0x0012a410:    f000fa82    ....    BL       qspi_std_write ; 0x12a918
        0x0012a414:    e023        #.      B        0x12a45e ; flash_erase + 142
        0x0012a416:    2d01        .-      CMP      r5,#1
        0x0012a418:    d110        ..      BNE      0x12a43c ; flash_erase + 108
        0x0012a41a:    a015        ..      ADR      r0,{pc}+0x56 ; 0x12a470
        0x0012a41c:    6800        .h      LDR      r0,[r0,#0]
        0x0012a41e:    9000        ..      STR      r0,[sp,#0]
        0x0012a420:    0220         .      LSLS     r0,r4,#8
        0x0012a422:    0e01        ..      LSRS     r1,r0,#24
        0x0012a424:    4668        hF      MOV      r0,sp
        0x0012a426:    7041        Ap      STRB     r1,[r0,#1]
        0x0012a428:    0420         .      LSLS     r0,r4,#16
        0x0012a42a:    0e01        ..      LSRS     r1,r0,#24
        0x0012a42c:    4668        hF      MOV      r0,sp
        0x0012a42e:    7081        .p      STRB     r1,[r0,#2]
        0x0012a430:    70c4        .p      STRB     r4,[r0,#3]
        0x0012a432:    2604        .&      MOVS     r6,#4
        0x0012a434:    4631        1F      MOV      r1,r6
        0x0012a436:    f000fa6f    ..o.    BL       qspi_std_write ; 0x12a918
        0x0012a43a:    e010        ..      B        0x12a45e ; flash_erase + 142
        0x0012a43c:    a00d        ..      ADR      r0,{pc}+0x38 ; 0x12a474
        0x0012a43e:    6800        .h      LDR      r0,[r0,#0]
        0x0012a440:    9000        ..      STR      r0,[sp,#0]
        0x0012a442:    0220         .      LSLS     r0,r4,#8
        0x0012a444:    0e01        ..      LSRS     r1,r0,#24
        0x0012a446:    4668        hF      MOV      r0,sp
        0x0012a448:    7041        Ap      STRB     r1,[r0,#1]
        0x0012a44a:    0420         .      LSLS     r0,r4,#16
        0x0012a44c:    0e01        ..      LSRS     r1,r0,#24
        0x0012a44e:    4668        hF      MOV      r0,sp
        0x0012a450:    7081        .p      STRB     r1,[r0,#2]
        0x0012a452:    70c4        .p      STRB     r4,[r0,#3]
        0x0012a454:    2604        .&      MOVS     r6,#4
        0x0012a456:    4631        1F      MOV      r1,r6
        0x0012a458:    f000fa5e    ..^.    BL       qspi_std_write ; 0x12a918
        0x0012a45c:    bf00        ..      NOP      
        0x0012a45e:    f000f80b    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a462:    2000        .       MOVS     r0,#0
        0x0012a464:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012a466:    0000        ..      DCW    0
        0x0012a468:    00000060    `...    DCD    96
        0x0012a46c:    000000d8    ....    DCD    216
        0x0012a470:    00000052    R...    DCD    82
        0x0012a474:    00000020     ...    DCD    32
    $t
    i.flash_operation_wait
    flash_operation_wait
        0x0012a478:    b510        ..      PUSH     {r4,lr}
        0x0012a47a:    bf00        ..      NOP      
        0x0012a47c:    f000f89a    ....    BL       flash_read_status_reg_1 ; 0x12a5b4
        0x0012a480:    07c1        ..      LSLS     r1,r0,#31
        0x0012a482:    0fc9        ..      LSRS     r1,r1,#31
        0x0012a484:    2900        .)      CMP      r1,#0
        0x0012a486:    d1f9        ..      BNE      0x12a47c ; flash_operation_wait + 4
        0x0012a488:    bd10        ..      POP      {r4,pc}
    i.flash_program
    flash_program
        0x0012a48a:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012a48c:    b085        ..      SUB      sp,sp,#0x14
        0x0012a48e:    4604        .F      MOV      r4,r0
        0x0012a490:    460d        .F      MOV      r5,r1
        0x0012a492:    4617        .F      MOV      r7,r2
        0x0012a494:    0a20         .      LSRS     r0,r4,#8
        0x0012a496:    0200        ..      LSLS     r0,r0,#8
        0x0012a498:    9004        ..      STR      r0,[sp,#0x10]
        0x0012a49a:    9804        ..      LDR      r0,[sp,#0x10]
        0x0012a49c:    42a0        .B      CMP      r0,r4
        0x0012a49e:    d014        ..      BEQ      0x12a4ca ; flash_program + 64
        0x0012a4a0:    9804        ..      LDR      r0,[sp,#0x10]
        0x0012a4a2:    30ff        .0      ADDS     r0,r0,#0xff
        0x0012a4a4:    3001        .0      ADDS     r0,#1
        0x0012a4a6:    1b00        ..      SUBS     r0,r0,r4
        0x0012a4a8:    9000        ..      STR      r0,[sp,#0]
        0x0012a4aa:    9800        ..      LDR      r0,[sp,#0]
        0x0012a4ac:    4285        .B      CMP      r5,r0
        0x0012a4ae:    d201        ..      BCS      0x12a4b4 ; flash_program + 42
        0x0012a4b0:    4628        (F      MOV      r0,r5
        0x0012a4b2:    e000        ..      B        0x12a4b6 ; flash_program + 44
        0x0012a4b4:    9800        ..      LDR      r0,[sp,#0]
        0x0012a4b6:    4606        .F      MOV      r6,r0
        0x0012a4b8:    463a        :F      MOV      r2,r7
        0x0012a4ba:    4631        1F      MOV      r1,r6
        0x0012a4bc:    4620         F      MOV      r0,r4
        0x0012a4be:    f000f829    ..).    BL       flash_program_base ; 0x12a514
        0x0012a4c2:    19bf        ..      ADDS     r7,r7,r6
        0x0012a4c4:    19a4        ..      ADDS     r4,r4,r6
        0x0012a4c6:    1bad        ..      SUBS     r5,r5,r6
        0x0012a4c8:    bf00        ..      NOP      
        0x0012a4ca:    0228        (.      LSLS     r0,r5,#8
        0x0012a4cc:    0c00        ..      LSRS     r0,r0,#16
        0x0012a4ce:    9003        ..      STR      r0,[sp,#0xc]
        0x0012a4d0:    b2e8        ..      UXTB     r0,r5
        0x0012a4d2:    9002        ..      STR      r0,[sp,#8]
        0x0012a4d4:    2000        .       MOVS     r0,#0
        0x0012a4d6:    9001        ..      STR      r0,[sp,#4]
        0x0012a4d8:    e00d        ..      B        0x12a4f6 ; flash_program + 108
        0x0012a4da:    463a        :F      MOV      r2,r7
        0x0012a4dc:    21ff        .!      MOVS     r1,#0xff
        0x0012a4de:    3101        .1      ADDS     r1,#1
        0x0012a4e0:    4620         F      MOV      r0,r4
        0x0012a4e2:    f000f817    ....    BL       flash_program_base ; 0x12a514
        0x0012a4e6:    34ff        .4      ADDS     r4,r4,#0xff
        0x0012a4e8:    3401        .4      ADDS     r4,#1
        0x0012a4ea:    37ff        .7      ADDS     r7,r7,#0xff
        0x0012a4ec:    3701        .7      ADDS     r7,#1
        0x0012a4ee:    9801        ..      LDR      r0,[sp,#4]
        0x0012a4f0:    1c40        @.      ADDS     r0,r0,#1
        0x0012a4f2:    b280        ..      UXTH     r0,r0
        0x0012a4f4:    9001        ..      STR      r0,[sp,#4]
        0x0012a4f6:    9903        ..      LDR      r1,[sp,#0xc]
        0x0012a4f8:    9801        ..      LDR      r0,[sp,#4]
        0x0012a4fa:    4288        .B      CMP      r0,r1
        0x0012a4fc:    dbed        ..      BLT      0x12a4da ; flash_program + 80
        0x0012a4fe:    9802        ..      LDR      r0,[sp,#8]
        0x0012a500:    2800        .(      CMP      r0,#0
        0x0012a502:    d004        ..      BEQ      0x12a50e ; flash_program + 132
        0x0012a504:    463a        :F      MOV      r2,r7
        0x0012a506:    4620         F      MOV      r0,r4
        0x0012a508:    9902        ..      LDR      r1,[sp,#8]
        0x0012a50a:    f000f803    ....    BL       flash_program_base ; 0x12a514
        0x0012a50e:    2000        .       MOVS     r0,#0
        0x0012a510:    b005        ..      ADD      sp,sp,#0x14
        0x0012a512:    bdf0        ..      POP      {r4-r7,pc}
    i.flash_program_base
    flash_program_base
        0x0012a514:    b570        p.      PUSH     {r4-r6,lr}
        0x0012a516:    4604        .F      MOV      r4,r0
        0x0012a518:    460d        .F      MOV      r5,r1
        0x0012a51a:    4616        .F      MOV      r6,r2
        0x0012a51c:    f000f8ea    ....    BL       flash_write_enable ; 0x12a6f4
        0x0012a520:    462b        +F      MOV      r3,r5
        0x0012a522:    4632        2F      MOV      r2,r6
        0x0012a524:    4621        !F      MOV      r1,r4
        0x0012a526:    2002        .       MOVS     r0,#2
        0x0012a528:    f000f948    ..H.    BL       qspi_flash_program ; 0x12a7bc
        0x0012a52c:    f7ffffa4    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a530:    bd70        p.      POP      {r4-r6,pc}
        0x0012a532:    0000        ..      MOVS     r0,r0
    i.flash_read
    flash_read
        0x0012a534:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012a536:    4604        .F      MOV      r4,r0
        0x0012a538:    460d        .F      MOV      r5,r1
        0x0012a53a:    4616        .F      MOV      r6,r2
        0x0012a53c:    0c28        (.      LSRS     r0,r5,#16
        0x0012a53e:    9002        ..      STR      r0,[sp,#8]
        0x0012a540:    b2af        ..      UXTH     r7,r5
        0x0012a542:    2000        .       MOVS     r0,#0
        0x0012a544:    9001        ..      STR      r0,[sp,#4]
        0x0012a546:    e019        ..      B        0x12a57c ; flash_read + 72
        0x0012a548:    a019        ..      ADR      r0,{pc}+0x68 ; 0x12a5b0
        0x0012a54a:    6800        .h      LDR      r0,[r0,#0]
        0x0012a54c:    9000        ..      STR      r0,[sp,#0]
        0x0012a54e:    0220         .      LSLS     r0,r4,#8
        0x0012a550:    0e01        ..      LSRS     r1,r0,#24
        0x0012a552:    4668        hF      MOV      r0,sp
        0x0012a554:    7041        Ap      STRB     r1,[r0,#1]
        0x0012a556:    0420         .      LSLS     r0,r4,#16
        0x0012a558:    0e01        ..      LSRS     r1,r0,#24
        0x0012a55a:    4668        hF      MOV      r0,sp
        0x0012a55c:    7081        .p      STRB     r1,[r0,#2]
        0x0012a55e:    70c4        .p      STRB     r4,[r0,#3]
        0x0012a560:    2301        .#      MOVS     r3,#1
        0x0012a562:    041b        ..      LSLS     r3,r3,#16
        0x0012a564:    4632        2F      MOV      r2,r6
        0x0012a566:    2104        .!      MOVS     r1,#4
        0x0012a568:    f000f994    ....    BL       qspi_std_read ; 0x12a894
        0x0012a56c:    2001        .       MOVS     r0,#1
        0x0012a56e:    0400        ..      LSLS     r0,r0,#16
        0x0012a570:    1824        $.      ADDS     r4,r4,r0
        0x0012a572:    1836        6.      ADDS     r6,r6,r0
        0x0012a574:    9801        ..      LDR      r0,[sp,#4]
        0x0012a576:    1c40        @.      ADDS     r0,r0,#1
        0x0012a578:    b280        ..      UXTH     r0,r0
        0x0012a57a:    9001        ..      STR      r0,[sp,#4]
        0x0012a57c:    9902        ..      LDR      r1,[sp,#8]
        0x0012a57e:    9801        ..      LDR      r0,[sp,#4]
        0x0012a580:    4288        .B      CMP      r0,r1
        0x0012a582:    dbe1        ..      BLT      0x12a548 ; flash_read + 20
        0x0012a584:    2f00        ./      CMP      r7,#0
        0x0012a586:    d011        ..      BEQ      0x12a5ac ; flash_read + 120
        0x0012a588:    a009        ..      ADR      r0,{pc}+0x28 ; 0x12a5b0
        0x0012a58a:    6800        .h      LDR      r0,[r0,#0]
        0x0012a58c:    9000        ..      STR      r0,[sp,#0]
        0x0012a58e:    0220         .      LSLS     r0,r4,#8
        0x0012a590:    0e01        ..      LSRS     r1,r0,#24
        0x0012a592:    4668        hF      MOV      r0,sp
        0x0012a594:    7041        Ap      STRB     r1,[r0,#1]
        0x0012a596:    0420         .      LSLS     r0,r4,#16
        0x0012a598:    0e01        ..      LSRS     r1,r0,#24
        0x0012a59a:    4668        hF      MOV      r0,sp
        0x0012a59c:    7081        .p      STRB     r1,[r0,#2]
        0x0012a59e:    70c4        .p      STRB     r4,[r0,#3]
        0x0012a5a0:    463b        ;F      MOV      r3,r7
        0x0012a5a2:    4632        2F      MOV      r2,r6
        0x0012a5a4:    2104        .!      MOVS     r1,#4
        0x0012a5a6:    f000f975    ..u.    BL       qspi_std_read ; 0x12a894
        0x0012a5aa:    bf00        ..      NOP      
        0x0012a5ac:    2000        .       MOVS     r0,#0
        0x0012a5ae:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x0012a5b0:    00000003    ....    DCD    3
    $t
    i.flash_read_status_reg_1
    flash_read_status_reg_1
        0x0012a5b4:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012a5b6:    2105        .!      MOVS     r1,#5
        0x0012a5b8:    9100        ..      STR      r1,[sp,#0]
        0x0012a5ba:    2301        .#      MOVS     r3,#1
        0x0012a5bc:    aa01        ..      ADD      r2,sp,#4
        0x0012a5be:    4619        .F      MOV      r1,r3
        0x0012a5c0:    4668        hF      MOV      r0,sp
        0x0012a5c2:    f000f967    ..g.    BL       qspi_std_read ; 0x12a894
        0x0012a5c6:    4668        hF      MOV      r0,sp
        0x0012a5c8:    7900        .y      LDRB     r0,[r0,#4]
        0x0012a5ca:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_status_read
    flash_status_read
        0x0012a5cc:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012a5ce:    4604        .F      MOV      r4,r0
        0x0012a5d0:    2000        .       MOVS     r0,#0
        0x0012a5d2:    9000        ..      STR      r0,[sp,#0]
        0x0012a5d4:    2c00        .,      CMP      r4,#0
        0x0012a5d6:    d004        ..      BEQ      0x12a5e2 ; flash_status_read + 22
        0x0012a5d8:    2c01        .,      CMP      r4,#1
        0x0012a5da:    d00b        ..      BEQ      0x12a5f4 ; flash_status_read + 40
        0x0012a5dc:    2c02        .,      CMP      r4,#2
        0x0012a5de:    d11b        ..      BNE      0x12a618 ; flash_status_read + 76
        0x0012a5e0:    e011        ..      B        0x12a606 ; flash_status_read + 58
        0x0012a5e2:    2005        .       MOVS     r0,#5
        0x0012a5e4:    9000        ..      STR      r0,[sp,#0]
        0x0012a5e6:    2301        .#      MOVS     r3,#1
        0x0012a5e8:    aa01        ..      ADD      r2,sp,#4
        0x0012a5ea:    4619        .F      MOV      r1,r3
        0x0012a5ec:    4668        hF      MOV      r0,sp
        0x0012a5ee:    f000f951    ..Q.    BL       qspi_std_read ; 0x12a894
        0x0012a5f2:    e012        ..      B        0x12a61a ; flash_status_read + 78
        0x0012a5f4:    2035        5       MOVS     r0,#0x35
        0x0012a5f6:    9000        ..      STR      r0,[sp,#0]
        0x0012a5f8:    2301        .#      MOVS     r3,#1
        0x0012a5fa:    aa01        ..      ADD      r2,sp,#4
        0x0012a5fc:    4619        .F      MOV      r1,r3
        0x0012a5fe:    4668        hF      MOV      r0,sp
        0x0012a600:    f000f948    ..H.    BL       qspi_std_read ; 0x12a894
        0x0012a604:    e009        ..      B        0x12a61a ; flash_status_read + 78
        0x0012a606:    2015        .       MOVS     r0,#0x15
        0x0012a608:    9000        ..      STR      r0,[sp,#0]
        0x0012a60a:    2301        .#      MOVS     r3,#1
        0x0012a60c:    aa01        ..      ADD      r2,sp,#4
        0x0012a60e:    4619        .F      MOV      r1,r3
        0x0012a610:    4668        hF      MOV      r0,sp
        0x0012a612:    f000f93f    ..?.    BL       qspi_std_read ; 0x12a894
        0x0012a616:    e000        ..      B        0x12a61a ; flash_status_read + 78
        0x0012a618:    bf00        ..      NOP      
        0x0012a61a:    bf00        ..      NOP      
        0x0012a61c:    4668        hF      MOV      r0,sp
        0x0012a61e:    7900        .y      LDRB     r0,[r0,#4]
        0x0012a620:    bd1c        ..      POP      {r2-r4,pc}
        0x0012a622:    0000        ..      MOVS     r0,r0
    i.flash_status_write
    flash_status_write
        0x0012a624:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012a626:    4604        .F      MOV      r4,r0
        0x0012a628:    460d        .F      MOV      r5,r1
        0x0012a62a:    a020         .      ADR      r0,{pc}+0x82 ; 0x12a6ac
        0x0012a62c:    6800        .h      LDR      r0,[r0,#0]
        0x0012a62e:    9000        ..      STR      r0,[sp,#0]
        0x0012a630:    4668        hF      MOV      r0,sp
        0x0012a632:    7045        Ep      STRB     r5,[r0,#1]
        0x0012a634:    2600        .&      MOVS     r6,#0
        0x0012a636:    2c00        .,      CMP      r4,#0
        0x0012a638:    d004        ..      BEQ      0x12a644 ; flash_status_write + 32
        0x0012a63a:    2c01        .,      CMP      r4,#1
        0x0012a63c:    d012        ..      BEQ      0x12a664 ; flash_status_write + 64
        0x0012a63e:    2c02        .,      CMP      r4,#2
        0x0012a640:    d130        0.      BNE      0x12a6a4 ; flash_status_write + 128
        0x0012a642:    e01f        ..      B        0x12a684 ; flash_status_write + 96
        0x0012a644:    f000f856    ..V.    BL       flash_write_enable ; 0x12a6f4
        0x0012a648:    2001        .       MOVS     r0,#1
        0x0012a64a:    4669        iF      MOV      r1,sp
        0x0012a64c:    7008        .p      STRB     r0,[r1,#0]
        0x0012a64e:    2102        .!      MOVS     r1,#2
        0x0012a650:    4668        hF      MOV      r0,sp
        0x0012a652:    f000f961    ..a.    BL       qspi_std_write ; 0x12a918
        0x0012a656:    f7ffff0f    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a65a:    4620         F      MOV      r0,r4
        0x0012a65c:    f7ffffb6    ....    BL       flash_status_read ; 0x12a5cc
        0x0012a660:    4606        .F      MOV      r6,r0
        0x0012a662:    e020         .      B        0x12a6a6 ; flash_status_write + 130
        0x0012a664:    f000f846    ..F.    BL       flash_write_enable ; 0x12a6f4
        0x0012a668:    2031        1       MOVS     r0,#0x31
        0x0012a66a:    4669        iF      MOV      r1,sp
        0x0012a66c:    7008        .p      STRB     r0,[r1,#0]
        0x0012a66e:    2102        .!      MOVS     r1,#2
        0x0012a670:    4668        hF      MOV      r0,sp
        0x0012a672:    f000f951    ..Q.    BL       qspi_std_write ; 0x12a918
        0x0012a676:    f7fffeff    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a67a:    4620         F      MOV      r0,r4
        0x0012a67c:    f7ffffa6    ....    BL       flash_status_read ; 0x12a5cc
        0x0012a680:    4606        .F      MOV      r6,r0
        0x0012a682:    e010        ..      B        0x12a6a6 ; flash_status_write + 130
        0x0012a684:    f000f836    ..6.    BL       flash_write_enable ; 0x12a6f4
        0x0012a688:    2011        .       MOVS     r0,#0x11
        0x0012a68a:    4669        iF      MOV      r1,sp
        0x0012a68c:    7008        .p      STRB     r0,[r1,#0]
        0x0012a68e:    2102        .!      MOVS     r1,#2
        0x0012a690:    4668        hF      MOV      r0,sp
        0x0012a692:    f000f941    ..A.    BL       qspi_std_write ; 0x12a918
        0x0012a696:    f7fffeef    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a69a:    4620         F      MOV      r0,r4
        0x0012a69c:    f7ffff96    ....    BL       flash_status_read ; 0x12a5cc
        0x0012a6a0:    4606        .F      MOV      r6,r0
        0x0012a6a2:    e000        ..      B        0x12a6a6 ; flash_status_write + 130
        0x0012a6a4:    bf00        ..      NOP      
        0x0012a6a6:    bf00        ..      NOP      
        0x0012a6a8:    4630        0F      MOV      r0,r6
        0x0012a6aa:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012a6ac:    00000031    1...    DCD    49
    $t
    i.flash_status_write_2bytes
    flash_status_write_2bytes
        0x0012a6b0:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012a6b2:    4604        .F      MOV      r4,r0
        0x0012a6b4:    460d        .F      MOV      r5,r1
        0x0012a6b6:    a00e        ..      ADR      r0,{pc}+0x3a ; 0x12a6f0
        0x0012a6b8:    6800        .h      LDR      r0,[r0,#0]
        0x0012a6ba:    9000        ..      STR      r0,[sp,#0]
        0x0012a6bc:    4668        hF      MOV      r0,sp
        0x0012a6be:    7044        Dp      STRB     r4,[r0,#1]
        0x0012a6c0:    7085        .p      STRB     r5,[r0,#2]
        0x0012a6c2:    2600        .&      MOVS     r6,#0
        0x0012a6c4:    f000f816    ....    BL       flash_write_enable ; 0x12a6f4
        0x0012a6c8:    2103        .!      MOVS     r1,#3
        0x0012a6ca:    4668        hF      MOV      r0,sp
        0x0012a6cc:    f000f924    ..$.    BL       qspi_std_write ; 0x12a918
        0x0012a6d0:    f7fffed2    ....    BL       flash_operation_wait ; 0x12a478
        0x0012a6d4:    2000        .       MOVS     r0,#0
        0x0012a6d6:    f7ffff79    ..y.    BL       flash_status_read ; 0x12a5cc
        0x0012a6da:    4606        .F      MOV      r6,r0
        0x0012a6dc:    2001        .       MOVS     r0,#1
        0x0012a6de:    f7ffff75    ..u.    BL       flash_status_read ; 0x12a5cc
        0x0012a6e2:    0200        ..      LSLS     r0,r0,#8
        0x0012a6e4:    21ff        .!      MOVS     r1,#0xff
        0x0012a6e6:    0209        ..      LSLS     r1,r1,#8
        0x0012a6e8:    4008        .@      ANDS     r0,r0,r1
        0x0012a6ea:    4306        .C      ORRS     r6,r6,r0
        0x0012a6ec:    4630        0F      MOV      r0,r6
        0x0012a6ee:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012a6f0:    00000001    ....    DCD    1
    $t
    i.flash_write_enable
    flash_write_enable
        0x0012a6f4:    b508        ..      PUSH     {r3,lr}
        0x0012a6f6:    2006        .       MOVS     r0,#6
        0x0012a6f8:    9000        ..      STR      r0,[sp,#0]
        0x0012a6fa:    2101        .!      MOVS     r1,#1
        0x0012a6fc:    4668        hF      MOV      r0,sp
        0x0012a6fe:    f000f90b    ....    BL       qspi_std_write ; 0x12a918
        0x0012a702:    bd08        ..      POP      {r3,pc}
    i.main
    main
        0x0012a704:    b510        ..      PUSH     {r4,lr}
        0x0012a706:    2022        "       MOVS     r0,#0x22
        0x0012a708:    4910        .I      LDR      r1,[pc,#64] ; [0x12a74c] = 0x130100
        0x0012a70a:    6008        .`      STR      r0,[r1,#0]
        0x0012a70c:    f000f826    ..&.    BL       open_pinshare ; 0x12a75c
        0x0012a710:    f7fffd9a    ....    BL       dmac_init ; 0x12a248
        0x0012a714:    2101        .!      MOVS     r1,#1
        0x0012a716:    2002        .       MOVS     r0,#2
        0x0012a718:    f000f870    ..p.    BL       qspi_init ; 0x12a7fc
        0x0012a71c:    f000f9aa    ....    BL       uart_download_init ; 0x12aa74
        0x0012a720:    f000fd00    ....    BL       uart_sync_to_pc ; 0x12b124
        0x0012a724:    bf00        ..      NOP      
        0x0012a726:    480a        .H      LDR      r0,[pc,#40] ; [0x12a750] = 0x12b256
        0x0012a728:    7800        .x      LDRB     r0,[r0,#0]
        0x0012a72a:    2800        .(      CMP      r0,#0
        0x0012a72c:    d0fb        ..      BEQ      0x12a726 ; main + 34
        0x0012a72e:    f000f9c7    ....    BL       uart_download_start ; 0x12aac0
        0x0012a732:    e005        ..      B        0x12a740 ; main + 60
        0x0012a734:    4807        .H      LDR      r0,[pc,#28] ; [0x12a754] = 0x12b257
        0x0012a736:    7800        .x      LDRB     r0,[r0,#0]
        0x0012a738:    2800        .(      CMP      r0,#0
        0x0012a73a:    d001        ..      BEQ      0x12a740 ; main + 60
        0x0012a73c:    f000f8fc    ....    BL       start_ram_run_function ; 0x12a938
        0x0012a740:    4805        .H      LDR      r0,[pc,#20] ; [0x12a758] = 0x12b258
        0x0012a742:    7800        .x      LDRB     r0,[r0,#0]
        0x0012a744:    2800        .(      CMP      r0,#0
        0x0012a746:    d0f5        ..      BEQ      0x12a734 ; main + 48
        0x0012a748:    2000        .       MOVS     r0,#0
        0x0012a74a:    bd10        ..      POP      {r4,pc}
    $d
        0x0012a74c:    00130100    ....    DCD    1245440
        0x0012a750:    0012b256    V...    DCD    1225302
        0x0012a754:    0012b257    W...    DCD    1225303
        0x0012a758:    0012b258    X...    DCD    1225304
    $t
    i.open_pinshare
    open_pinshare
        0x0012a75c:    2003        .       MOVS     r0,#3
        0x0012a75e:    4916        .I      LDR      r1,[pc,#88] ; [0x12a7b8] = 0x20132000
        0x0012a760:    6a09        .j      LDR      r1,[r1,#0x20]
        0x0012a762:    0909        ..      LSRS     r1,r1,#4
        0x0012a764:    0109        ..      LSLS     r1,r1,#4
        0x0012a766:    4301        .C      ORRS     r1,r1,r0
        0x0012a768:    4a13        .J      LDR      r2,[pc,#76] ; [0x12a7b8] = 0x20132000
        0x0012a76a:    6211        .b      STR      r1,[r2,#0x20]
        0x0012a76c:    bf00        ..      NOP      
        0x0012a76e:    2000        .       MOVS     r0,#0
        0x0012a770:    4611        .F      MOV      r1,r2
        0x0012a772:    6b49        Ik      LDR      r1,[r1,#0x34]
        0x0012a774:    220f        ."      MOVS     r2,#0xf
        0x0012a776:    0212        ..      LSLS     r2,r2,#8
        0x0012a778:    4391        .C      BICS     r1,r1,r2
        0x0012a77a:    0202        ..      LSLS     r2,r0,#8
        0x0012a77c:    230f        .#      MOVS     r3,#0xf
        0x0012a77e:    021b        ..      LSLS     r3,r3,#8
        0x0012a780:    401a        .@      ANDS     r2,r2,r3
        0x0012a782:    4311        .C      ORRS     r1,r1,r2
        0x0012a784:    4a0c        .J      LDR      r2,[pc,#48] ; [0x12a7b8] = 0x20132000
        0x0012a786:    6351        Qc      STR      r1,[r2,#0x34]
        0x0012a788:    bf00        ..      NOP      
        0x0012a78a:    2001        .       MOVS     r0,#1
        0x0012a78c:    4611        .F      MOV      r1,r2
        0x0012a78e:    6b49        Ik      LDR      r1,[r1,#0x34]
        0x0012a790:    011a        ..      LSLS     r2,r3,#4
        0x0012a792:    4391        .C      BICS     r1,r1,r2
        0x0012a794:    0302        ..      LSLS     r2,r0,#12
        0x0012a796:    011b        ..      LSLS     r3,r3,#4
        0x0012a798:    401a        .@      ANDS     r2,r2,r3
        0x0012a79a:    4311        .C      ORRS     r1,r1,r2
        0x0012a79c:    4a06        .J      LDR      r2,[pc,#24] ; [0x12a7b8] = 0x20132000
        0x0012a79e:    6351        Qc      STR      r1,[r2,#0x34]
        0x0012a7a0:    bf00        ..      NOP      
        0x0012a7a2:    2003        .       MOVS     r0,#3
        0x0012a7a4:    0280        ..      LSLS     r0,r0,#10
        0x0012a7a6:    4611        .F      MOV      r1,r2
        0x0012a7a8:    6a49        Ij      LDR      r1,[r1,#0x24]
        0x0012a7aa:    0d89        ..      LSRS     r1,r1,#22
        0x0012a7ac:    0589        ..      LSLS     r1,r1,#22
        0x0012a7ae:    4301        .C      ORRS     r1,r1,r0
        0x0012a7b0:    6251        Qb      STR      r1,[r2,#0x24]
        0x0012a7b2:    bf00        ..      NOP      
        0x0012a7b4:    4770        pG      BX       lr
    $d
        0x0012a7b6:    0000        ..      DCW    0
        0x0012a7b8:    20132000    . .     DCD    538124288
    $t
    i.qspi_flash_program
    qspi_flash_program
        0x0012a7bc:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012a7be:    4605        .F      MOV      r5,r0
        0x0012a7c0:    460c        .F      MOV      r4,r1
        0x0012a7c2:    4616        .F      MOV      r6,r2
        0x0012a7c4:    461f        .F      MOV      r7,r3
        0x0012a7c6:    2140        @!      MOVS     r1,#0x40
        0x0012a7c8:    480b        .H      LDR      r0,[pc,#44] ; [0x12a7f8] = 0x20300000
        0x0012a7ca:    f000f825    ..%.    BL       qspi_std_byte_write_dma_config ; 0x12a818
        0x0012a7ce:    480a        .H      LDR      r0,[pc,#40] ; [0x12a7f8] = 0x20300000
        0x0012a7d0:    3040        @0      ADDS     r0,r0,#0x40
        0x0012a7d2:    6205        .b      STR      r5,[r0,#0x20]
        0x0012a7d4:    0220         .      LSLS     r0,r4,#8
        0x0012a7d6:    0e00        ..      LSRS     r0,r0,#24
        0x0012a7d8:    4907        .I      LDR      r1,[pc,#28] ; [0x12a7f8] = 0x20300000
        0x0012a7da:    3140        @1      ADDS     r1,r1,#0x40
        0x0012a7dc:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a7de:    0420         .      LSLS     r0,r4,#16
        0x0012a7e0:    0e00        ..      LSRS     r0,r0,#24
        0x0012a7e2:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a7e4:    b2e0        ..      UXTB     r0,r4
        0x0012a7e6:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a7e8:    463b        ;F      MOV      r3,r7
        0x0012a7ea:    4632        2F      MOV      r2,r6
        0x0012a7ec:    2101        .!      MOVS     r1,#1
        0x0012a7ee:    4802        .H      LDR      r0,[pc,#8] ; [0x12a7f8] = 0x20300000
        0x0012a7f0:    f000f820    .. .    BL       qspi_std_byte_write_dma_start ; 0x12a834
        0x0012a7f4:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012a7f6:    0000        ..      DCW    0
        0x0012a7f8:    20300000    ..0     DCD    540016640
    $t
    i.qspi_init
    qspi_init
        0x0012a7fc:    2200        ."      MOVS     r2,#0
        0x0012a7fe:    4b05        .K      LDR      r3,[pc,#20] ; [0x12a814] = 0x20300000
        0x0012a800:    609a        .`      STR      r2,[r3,#8]
        0x0012a802:    461a        .F      MOV      r2,r3
        0x0012a804:    6150        Pa      STR      r0,[r2,#0x14]
        0x0012a806:    4a03        .J      LDR      r2,[pc,#12] ; [0x12a814] = 0x20300000
        0x0012a808:    32c0        .2      ADDS     r2,r2,#0xc0
        0x0012a80a:    6311        .c      STR      r1,[r2,#0x30]
        0x0012a80c:    2200        ."      MOVS     r2,#0
        0x0012a80e:    611a        .a      STR      r2,[r3,#0x10]
        0x0012a810:    4770        pG      BX       lr
    $d
        0x0012a812:    0000        ..      DCW    0
        0x0012a814:    20300000    ..0     DCD    540016640
    $t
    i.qspi_std_byte_write_dma_config
    qspi_std_byte_write_dma_config
        0x0012a818:    4a05        .J      LDR      r2,[pc,#20] ; [0x12a830] = 0x70100
        0x0012a81a:    6002        .`      STR      r2,[r0,#0]
        0x0012a81c:    1f0a        ..      SUBS     r2,r1,#4
        0x0012a81e:    6502        .e      STR      r2,[r0,#0x50]
        0x0012a820:    2200        ."      MOVS     r2,#0
        0x0012a822:    64c2        .d      STR      r2,[r0,#0x4c]
        0x0012a824:    2202        ."      MOVS     r2,#2
        0x0012a826:    64c2        .d      STR      r2,[r0,#0x4c]
        0x0012a828:    2201        ."      MOVS     r2,#1
        0x0012a82a:    6082        .`      STR      r2,[r0,#8]
        0x0012a82c:    4770        pG      BX       lr
    $d
        0x0012a82e:    0000        ..      DCW    0
        0x0012a830:    00070100    ....    DCD    459008
    $t
    i.qspi_std_byte_write_dma_start
    qspi_std_byte_write_dma_start
        0x0012a834:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012a836:    b085        ..      SUB      sp,sp,#0x14
        0x0012a838:    4604        .F      MOV      r4,r0
        0x0012a83a:    460d        .F      MOV      r5,r1
        0x0012a83c:    4616        .F      MOV      r6,r2
        0x0012a83e:    461f        .F      MOV      r7,r3
        0x0012a840:    4813        .H      LDR      r0,[pc,#76] ; [0x12a890] = 0x12b1f4
        0x0012a842:    4601        .F      MOV      r1,r0
        0x0012a844:    c90e        ..      LDM      r1,{r1-r3}
        0x0012a846:    68c0        .h      LDR      r0,[r0,#0xc]
        0x0012a848:    9303        ..      STR      r3,[sp,#0xc]
        0x0012a84a:    9202        ..      STR      r2,[sp,#8]
        0x0012a84c:    9101        ..      STR      r1,[sp,#4]
        0x0012a84e:    9004        ..      STR      r0,[sp,#0x10]
        0x0012a850:    9601        ..      STR      r6,[sp,#4]
        0x0012a852:    4620         F      MOV      r0,r4
        0x0012a854:    3060        `0      ADDS     r0,r0,#0x60
        0x0012a856:    9002        ..      STR      r0,[sp,#8]
        0x0012a858:    9703        ..      STR      r7,[sp,#0xc]
        0x0012a85a:    2100        .!      MOVS     r1,#0
        0x0012a85c:    a801        ..      ADD      r0,sp,#4
        0x0012a85e:    f7fffcfd    ....    BL       dmac_start ; 0x12a25c
        0x0012a862:    9000        ..      STR      r0,[sp,#0]
        0x0012a864:    6125        %a      STR      r5,[r4,#0x10]
        0x0012a866:    2100        .!      MOVS     r1,#0
        0x0012a868:    9800        ..      LDR      r0,[sp,#0]
        0x0012a86a:    f7fffd7b    ..{.    BL       dmac_transfer_wait ; 0x12a364
        0x0012a86e:    bf00        ..      NOP      
        0x0012a870:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012a872:    2104        .!      MOVS     r1,#4
        0x0012a874:    4008        .@      ANDS     r0,r0,r1
        0x0012a876:    0880        ..      LSRS     r0,r0,#2
        0x0012a878:    2800        .(      CMP      r0,#0
        0x0012a87a:    d0f9        ..      BEQ      0x12a870 ; qspi_std_byte_write_dma_start + 60
        0x0012a87c:    bf00        ..      NOP      
        0x0012a87e:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012a880:    07c0        ..      LSLS     r0,r0,#31
        0x0012a882:    0fc0        ..      LSRS     r0,r0,#31
        0x0012a884:    2800        .(      CMP      r0,#0
        0x0012a886:    d1fa        ..      BNE      0x12a87e ; qspi_std_byte_write_dma_start + 74
        0x0012a888:    60a0        .`      STR      r0,[r4,#8]
        0x0012a88a:    6120         a      STR      r0,[r4,#0x10]
        0x0012a88c:    b005        ..      ADD      sp,sp,#0x14
        0x0012a88e:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012a890:    0012b1f4    ....    DCD    1225204
    $t
    i.qspi_std_read
    qspi_std_read
        0x0012a894:    b5ff        ..      PUSH     {r0-r7,lr}
        0x0012a896:    b085        ..      SUB      sp,sp,#0x14
        0x0012a898:    4606        .F      MOV      r6,r0
        0x0012a89a:    460f        .F      MOV      r7,r1
        0x0012a89c:    461c        .F      MOV      r4,r3
        0x0012a89e:    481b        .H      LDR      r0,[pc,#108] ; [0x12a90c] = 0x70300
        0x0012a8a0:    491b        .I      LDR      r1,[pc,#108] ; [0x12a910] = 0x20300000
        0x0012a8a2:    6008        .`      STR      r0,[r1,#0]
        0x0012a8a4:    1e60        `.      SUBS     r0,r4,#1
        0x0012a8a6:    6048        H`      STR      r0,[r1,#4]
        0x0012a8a8:    2007        .       MOVS     r0,#7
        0x0012a8aa:    4919        .I      LDR      r1,[pc,#100] ; [0x12a910] = 0x20300000
        0x0012a8ac:    3140        @1      ADDS     r1,r1,#0x40
        0x0012a8ae:    6148        Ha      STR      r0,[r1,#0x14]
        0x0012a8b0:    2000        .       MOVS     r0,#0
        0x0012a8b2:    60c8        .`      STR      r0,[r1,#0xc]
        0x0012a8b4:    2001        .       MOVS     r0,#1
        0x0012a8b6:    60c8        .`      STR      r0,[r1,#0xc]
        0x0012a8b8:    4915        .I      LDR      r1,[pc,#84] ; [0x12a910] = 0x20300000
        0x0012a8ba:    6088        .`      STR      r0,[r1,#8]
        0x0012a8bc:    2500        .%      MOVS     r5,#0
        0x0012a8be:    e005        ..      B        0x12a8cc ; qspi_std_read + 56
        0x0012a8c0:    5d70        p]      LDRB     r0,[r6,r5]
        0x0012a8c2:    4913        .I      LDR      r1,[pc,#76] ; [0x12a910] = 0x20300000
        0x0012a8c4:    3140        @1      ADDS     r1,r1,#0x40
        0x0012a8c6:    6208        .b      STR      r0,[r1,#0x20]
        0x0012a8c8:    1c68        h.      ADDS     r0,r5,#1
        0x0012a8ca:    b2c5        ..      UXTB     r5,r0
        0x0012a8cc:    42bd        .B      CMP      r5,r7
        0x0012a8ce:    dbf7        ..      BLT      0x12a8c0 ; qspi_std_read + 44
        0x0012a8d0:    4810        .H      LDR      r0,[pc,#64] ; [0x12a914] = 0x12b204
        0x0012a8d2:    4601        .F      MOV      r1,r0
        0x0012a8d4:    c90e        ..      LDM      r1,{r1-r3}
        0x0012a8d6:    68c0        .h      LDR      r0,[r0,#0xc]
        0x0012a8d8:    9303        ..      STR      r3,[sp,#0xc]
        0x0012a8da:    9202        ..      STR      r2,[sp,#8]
        0x0012a8dc:    9101        ..      STR      r1,[sp,#4]
        0x0012a8de:    9004        ..      STR      r0,[sp,#0x10]
        0x0012a8e0:    9807        ..      LDR      r0,[sp,#0x1c]
        0x0012a8e2:    9002        ..      STR      r0,[sp,#8]
        0x0012a8e4:    9403        ..      STR      r4,[sp,#0xc]
        0x0012a8e6:    2100        .!      MOVS     r1,#0
        0x0012a8e8:    a801        ..      ADD      r0,sp,#4
        0x0012a8ea:    f7fffcb7    ....    BL       dmac_start ; 0x12a25c
        0x0012a8ee:    9000        ..      STR      r0,[sp,#0]
        0x0012a8f0:    2001        .       MOVS     r0,#1
        0x0012a8f2:    4907        .I      LDR      r1,[pc,#28] ; [0x12a910] = 0x20300000
        0x0012a8f4:    6108        .a      STR      r0,[r1,#0x10]
        0x0012a8f6:    2100        .!      MOVS     r1,#0
        0x0012a8f8:    9800        ..      LDR      r0,[sp,#0]
        0x0012a8fa:    f7fffd33    ..3.    BL       dmac_transfer_wait ; 0x12a364
        0x0012a8fe:    2000        .       MOVS     r0,#0
        0x0012a900:    4903        .I      LDR      r1,[pc,#12] ; [0x12a910] = 0x20300000
        0x0012a902:    6088        .`      STR      r0,[r1,#8]
        0x0012a904:    6108        .a      STR      r0,[r1,#0x10]
        0x0012a906:    b009        ..      ADD      sp,sp,#0x24
        0x0012a908:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012a90a:    0000        ..      DCW    0
        0x0012a90c:    00070300    ....    DCD    459520
        0x0012a910:    20300000    ..0     DCD    540016640
        0x0012a914:    0012b204    ....    DCD    1225220
    $t
    i.qspi_std_write
    qspi_std_write
        0x0012a918:    b570        p.      PUSH     {r4-r6,lr}
        0x0012a91a:    4604        .F      MOV      r4,r0
        0x0012a91c:    460d        .F      MOV      r5,r1
        0x0012a91e:    2140        @!      MOVS     r1,#0x40
        0x0012a920:    4804        .H      LDR      r0,[pc,#16] ; [0x12a934] = 0x20300000
        0x0012a922:    f7ffff79    ..y.    BL       qspi_std_byte_write_dma_config ; 0x12a818
        0x0012a926:    462b        +F      MOV      r3,r5
        0x0012a928:    4622        "F      MOV      r2,r4
        0x0012a92a:    2101        .!      MOVS     r1,#1
        0x0012a92c:    4801        .H      LDR      r0,[pc,#4] ; [0x12a934] = 0x20300000
        0x0012a92e:    f7ffff81    ....    BL       qspi_std_byte_write_dma_start ; 0x12a834
        0x0012a932:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012a934:    20300000    ..0     DCD    540016640
    $t
    i.start_ram_run_function
    start_ram_run_function
        0x0012a938:    b570        p.      PUSH     {r4-r6,lr}
        0x0012a93a:    f000fc3b    ..;.    BL       uart_uninit ; 0x12b1b4
        0x0012a93e:    2001        .       MOVS     r0,#1
        0x0012a940:    0500        ..      LSLS     r0,r0,#20
        0x0012a942:    4908        .I      LDR      r1,[pc,#32] ; [0x12a964] = 0x12b278
        0x0012a944:    6008        .`      STR      r0,[r1,#0]
        0x0012a946:    6809        .h      LDR      r1,[r1,#0]
        0x0012a948:    6808        .h      LDR      r0,[r1,#0]
        0x0012a94a:    f3808808    ....    MSR      MSP,r0
        0x0012a94e:    bf00        ..      NOP      
        0x0012a950:    4804        .H      LDR      r0,[pc,#16] ; [0x12a964] = 0x12b278
        0x0012a952:    6800        .h      LDR      r0,[r0,#0]
        0x0012a954:    1d04        ..      ADDS     r4,r0,#4
        0x0012a956:    4b04        .K      LDR      r3,[pc,#16] ; [0x12a968] = 0x3399
        0x0012a958:    2200        ."      MOVS     r2,#0
        0x0012a95a:    4611        .F      MOV      r1,r2
        0x0012a95c:    4610        .F      MOV      r0,r2
        0x0012a95e:    6825        %h      LDR      r5,[r4,#0]
        0x0012a960:    47a8        .G      BLX      r5
        0x0012a962:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012a964:    0012b278    x...    DCD    1225336
        0x0012a968:    00003399    .3..    DCD    13209
    $t
    i.sysc_per_clk_div_param1_pack
    sysc_per_clk_div_param1_pack
        0x0012a96c:    b530        0.      PUSH     {r4,r5,lr}
        0x0012a96e:    0604        ..      LSLS     r4,r0,#24
        0x0012a970:    040d        ..      LSLS     r5,r1,#16
        0x0012a972:    432c        ,C      ORRS     r4,r4,r5
        0x0012a974:    0215        ..      LSLS     r5,r2,#8
        0x0012a976:    432c        ,C      ORRS     r4,r4,r5
        0x0012a978:    431c        .C      ORRS     r4,r4,r3
        0x0012a97a:    4d01        .M      LDR      r5,[pc,#4] ; [0x12a980] = 0x20149000
        0x0012a97c:    60ac        .`      STR      r4,[r5,#8]
        0x0012a97e:    bd30        0.      POP      {r4,r5,pc}
    $d
        0x0012a980:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clk_sel_uart0_setf
    sysc_per_clk_sel_uart0_setf
        0x0012a984:    4906        .I      LDR      r1,[pc,#24] ; [0x12a9a0] = 0x20149000
        0x0012a986:    6809        .h      LDR      r1,[r1,#0]
        0x0012a988:    2203        ."      MOVS     r2,#3
        0x0012a98a:    0212        ..      LSLS     r2,r2,#8
        0x0012a98c:    4391        .C      BICS     r1,r1,r2
        0x0012a98e:    0202        ..      LSLS     r2,r0,#8
        0x0012a990:    2303        .#      MOVS     r3,#3
        0x0012a992:    021b        ..      LSLS     r3,r3,#8
        0x0012a994:    401a        .@      ANDS     r2,r2,r3
        0x0012a996:    4311        .C      ORRS     r1,r1,r2
        0x0012a998:    4a01        .J      LDR      r2,[pc,#4] ; [0x12a9a0] = 0x20149000
        0x0012a99a:    6011        .`      STR      r1,[r2,#0]
        0x0012a99c:    4770        pG      BX       lr
    $d
        0x0012a99e:    0000        ..      DCW    0
        0x0012a9a0:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clkg0_pack
    sysc_per_clkg0_pack
        0x0012a9a4:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012a9a6:    9d06        ..      LDR      r5,[sp,#0x18]
        0x0012a9a8:    9c05        ..      LDR      r4,[sp,#0x14]
        0x0012a9aa:    06c6        ..      LSLS     r6,r0,#27
        0x0012a9ac:    068f        ..      LSLS     r7,r1,#26
        0x0012a9ae:    433e        >C      ORRS     r6,r6,r7
        0x0012a9b0:    0657        W.      LSLS     r7,r2,#25
        0x0012a9b2:    433e        >C      ORRS     r6,r6,r7
        0x0012a9b4:    061f        ..      LSLS     r7,r3,#24
        0x0012a9b6:    433e        >C      ORRS     r6,r6,r7
        0x0012a9b8:    05e7        ..      LSLS     r7,r4,#23
        0x0012a9ba:    433e        >C      ORRS     r6,r6,r7
        0x0012a9bc:    05af        ..      LSLS     r7,r5,#22
        0x0012a9be:    433e        >C      ORRS     r6,r6,r7
        0x0012a9c0:    9f07        ..      LDR      r7,[sp,#0x1c]
        0x0012a9c2:    057f        ..      LSLS     r7,r7,#21
        0x0012a9c4:    433e        >C      ORRS     r6,r6,r7
        0x0012a9c6:    9f08        ..      LDR      r7,[sp,#0x20]
        0x0012a9c8:    053f        ?.      LSLS     r7,r7,#20
        0x0012a9ca:    433e        >C      ORRS     r6,r6,r7
        0x0012a9cc:    9f09        ..      LDR      r7,[sp,#0x24]
        0x0012a9ce:    04ff        ..      LSLS     r7,r7,#19
        0x0012a9d0:    433e        >C      ORRS     r6,r6,r7
        0x0012a9d2:    9f0a        ..      LDR      r7,[sp,#0x28]
        0x0012a9d4:    04bf        ..      LSLS     r7,r7,#18
        0x0012a9d6:    433e        >C      ORRS     r6,r6,r7
        0x0012a9d8:    9f0b        ..      LDR      r7,[sp,#0x2c]
        0x0012a9da:    047f        ..      LSLS     r7,r7,#17
        0x0012a9dc:    433e        >C      ORRS     r6,r6,r7
        0x0012a9de:    9f0c        ..      LDR      r7,[sp,#0x30]
        0x0012a9e0:    043f        ?.      LSLS     r7,r7,#16
        0x0012a9e2:    433e        >C      ORRS     r6,r6,r7
        0x0012a9e4:    9f0d        ..      LDR      r7,[sp,#0x34]
        0x0012a9e6:    03ff        ..      LSLS     r7,r7,#15
        0x0012a9e8:    433e        >C      ORRS     r6,r6,r7
        0x0012a9ea:    9f0e        ..      LDR      r7,[sp,#0x38]
        0x0012a9ec:    03bf        ..      LSLS     r7,r7,#14
        0x0012a9ee:    433e        >C      ORRS     r6,r6,r7
        0x0012a9f0:    9f0f        ..      LDR      r7,[sp,#0x3c]
        0x0012a9f2:    037f        ..      LSLS     r7,r7,#13
        0x0012a9f4:    433e        >C      ORRS     r6,r6,r7
        0x0012a9f6:    9f10        ..      LDR      r7,[sp,#0x40]
        0x0012a9f8:    033f        ?.      LSLS     r7,r7,#12
        0x0012a9fa:    433e        >C      ORRS     r6,r6,r7
        0x0012a9fc:    9f11        ..      LDR      r7,[sp,#0x44]
        0x0012a9fe:    02ff        ..      LSLS     r7,r7,#11
        0x0012aa00:    433e        >C      ORRS     r6,r6,r7
        0x0012aa02:    9f12        ..      LDR      r7,[sp,#0x48]
        0x0012aa04:    02bf        ..      LSLS     r7,r7,#10
        0x0012aa06:    433e        >C      ORRS     r6,r6,r7
        0x0012aa08:    9f13        ..      LDR      r7,[sp,#0x4c]
        0x0012aa0a:    027f        ..      LSLS     r7,r7,#9
        0x0012aa0c:    433e        >C      ORRS     r6,r6,r7
        0x0012aa0e:    9f14        ..      LDR      r7,[sp,#0x50]
        0x0012aa10:    023f        ?.      LSLS     r7,r7,#8
        0x0012aa12:    433e        >C      ORRS     r6,r6,r7
        0x0012aa14:    9f15        ..      LDR      r7,[sp,#0x54]
        0x0012aa16:    01ff        ..      LSLS     r7,r7,#7
        0x0012aa18:    433e        >C      ORRS     r6,r6,r7
        0x0012aa1a:    9f16        ..      LDR      r7,[sp,#0x58]
        0x0012aa1c:    01bf        ..      LSLS     r7,r7,#6
        0x0012aa1e:    433e        >C      ORRS     r6,r6,r7
        0x0012aa20:    9f17        ..      LDR      r7,[sp,#0x5c]
        0x0012aa22:    017f        ..      LSLS     r7,r7,#5
        0x0012aa24:    433e        >C      ORRS     r6,r6,r7
        0x0012aa26:    9f18        ..      LDR      r7,[sp,#0x60]
        0x0012aa28:    013f        ?.      LSLS     r7,r7,#4
        0x0012aa2a:    433e        >C      ORRS     r6,r6,r7
        0x0012aa2c:    9f19        ..      LDR      r7,[sp,#0x64]
        0x0012aa2e:    00ff        ..      LSLS     r7,r7,#3
        0x0012aa30:    433e        >C      ORRS     r6,r6,r7
        0x0012aa32:    9f1a        ..      LDR      r7,[sp,#0x68]
        0x0012aa34:    00bf        ..      LSLS     r7,r7,#2
        0x0012aa36:    433e        >C      ORRS     r6,r6,r7
        0x0012aa38:    9f1b        ..      LDR      r7,[sp,#0x6c]
        0x0012aa3a:    007f        ..      LSLS     r7,r7,#1
        0x0012aa3c:    433e        >C      ORRS     r6,r6,r7
        0x0012aa3e:    9f1c        ..      LDR      r7,[sp,#0x70]
        0x0012aa40:    433e        >C      ORRS     r6,r6,r7
        0x0012aa42:    4f01        .O      LDR      r7,[pc,#4] ; [0x12aa48] = 0x20149000
        0x0012aa44:    613e        >a      STR      r6,[r7,#0x10]
        0x0012aa46:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012aa48:    20149000    ...     DCD    538218496
    $t
    i.sysc_per_clkg0_set
    sysc_per_clkg0_set
        0x0012aa4c:    4901        .I      LDR      r1,[pc,#4] ; [0x12aa54] = 0x20149000
        0x0012aa4e:    6108        .a      STR      r0,[r1,#0x10]
        0x0012aa50:    4770        pG      BX       lr
    $d
        0x0012aa52:    0000        ..      DCW    0
        0x0012aa54:    20149000    ...     DCD    538218496
    $t
    i.uart_dlab_setf
    uart_dlab_setf
        0x0012aa58:    4905        .I      LDR      r1,[pc,#20] ; [0x12aa70] = 0x20143000
        0x0012aa5a:    68c9        .h      LDR      r1,[r1,#0xc]
        0x0012aa5c:    2280        ."      MOVS     r2,#0x80
        0x0012aa5e:    4391        .C      BICS     r1,r1,r2
        0x0012aa60:    01c2        ..      LSLS     r2,r0,#7
        0x0012aa62:    2380        .#      MOVS     r3,#0x80
        0x0012aa64:    401a        .@      ANDS     r2,r2,r3
        0x0012aa66:    4311        .C      ORRS     r1,r1,r2
        0x0012aa68:    4a01        .J      LDR      r2,[pc,#4] ; [0x12aa70] = 0x20143000
        0x0012aa6a:    60d1        .`      STR      r1,[r2,#0xc]
        0x0012aa6c:    4770        pG      BX       lr
    $d
        0x0012aa6e:    0000        ..      DCW    0
        0x0012aa70:    20143000    .0.     DCD    538193920
    $t
    i.uart_download_init
    uart_download_init
        0x0012aa74:    b510        ..      PUSH     {r4,lr}
        0x0012aa76:    4808        .H      LDR      r0,[pc,#32] ; [0x12aa98] = 0x12aaa9
        0x0012aa78:    4908        .I      LDR      r1,[pc,#32] ; [0x12aa9c] = 0x12b25c
        0x0012aa7a:    6048        H`      STR      r0,[r1,#4]
        0x0012aa7c:    4808        .H      LDR      r0,[pc,#32] ; [0x12aaa0] = 0x12aaf1
        0x0012aa7e:    6008        .`      STR      r0,[r1,#0]
        0x0012aa80:    f7fffb9e    ....    BL       clk_uart0_921600 ; 0x12a1c0
        0x0012aa84:    2001        .       MOVS     r0,#1
        0x0012aa86:    f000f859    ..Y.    BL       uart_init ; 0x12ab3c
        0x0012aa8a:    200b        .       MOVS     r0,#0xb
        0x0012aa8c:    2101        .!      MOVS     r1,#1
        0x0012aa8e:    4081        .@      LSLS     r1,r1,r0
        0x0012aa90:    4a04        .J      LDR      r2,[pc,#16] ; [0x12aaa4] = 0xe000e100
        0x0012aa92:    6011        .`      STR      r1,[r2,#0]
        0x0012aa94:    bf00        ..      NOP      
        0x0012aa96:    bd10        ..      POP      {r4,pc}
    $d
        0x0012aa98:    0012aaa9    ....    DCD    1223337
        0x0012aa9c:    0012b25c    \...    DCD    1225308
        0x0012aaa0:    0012aaf1    ....    DCD    1223409
        0x0012aaa4:    e000e100    ....    DCD    3758153984
    $t
    i.uart_download_read
    uart_download_read
        0x0012aaa8:    b570        p.      PUSH     {r4-r6,lr}
        0x0012aaaa:    4604        .F      MOV      r4,r0
        0x0012aaac:    460d        .F      MOV      r5,r1
        0x0012aaae:    4616        .F      MOV      r6,r2
        0x0012aab0:    2300        .#      MOVS     r3,#0
        0x0012aab2:    4632        2F      MOV      r2,r6
        0x0012aab4:    4629        )F      MOV      r1,r5
        0x0012aab6:    4620         F      MOV      r0,r4
        0x0012aab8:    f000f8d6    ....    BL       uart_read ; 0x12ac68
        0x0012aabc:    bd70        p.      POP      {r4-r6,pc}
        0x0012aabe:    0000        ..      MOVS     r0,r0
    i.uart_download_start
    uart_download_start
        0x0012aac0:    b510        ..      PUSH     {r4,lr}
        0x0012aac2:    4806        .H      LDR      r0,[pc,#24] ; [0x12aadc] = 0x12b25c
        0x0012aac4:    4a06        .J      LDR      r2,[pc,#24] ; [0x12aae0] = 0x12b1b1
        0x0012aac6:    2105        .!      MOVS     r1,#5
        0x0012aac8:    6803        .h      LDR      r3,[r0,#0]
        0x0012aaca:    4806        .H      LDR      r0,[pc,#24] ; [0x12aae4] = 0x12b264
        0x0012aacc:    4798        .G      BLX      r3
        0x0012aace:    4803        .H      LDR      r0,[pc,#12] ; [0x12aadc] = 0x12b25c
        0x0012aad0:    4a05        .J      LDR      r2,[pc,#20] ; [0x12aae8] = 0x12ad89
        0x0012aad2:    2108        .!      MOVS     r1,#8
        0x0012aad4:    6843        Ch      LDR      r3,[r0,#4]
        0x0012aad6:    4805        .H      LDR      r0,[pc,#20] ; [0x12aaec] = 0x12b67c
        0x0012aad8:    4798        .G      BLX      r3
        0x0012aada:    bd10        ..      POP      {r4,pc}
    $d
        0x0012aadc:    0012b25c    \...    DCD    1225308
        0x0012aae0:    0012b1b1    ....    DCD    1225137
        0x0012aae4:    0012b264    d...    DCD    1225316
        0x0012aae8:    0012ad89    ....    DCD    1224073
        0x0012aaec:    0012b67c    |...    DCD    1226364
    $t
    i.uart_download_write
    uart_download_write
        0x0012aaf0:    b570        p.      PUSH     {r4-r6,lr}
        0x0012aaf2:    4604        .F      MOV      r4,r0
        0x0012aaf4:    460d        .F      MOV      r5,r1
        0x0012aaf6:    4616        .F      MOV      r6,r2
        0x0012aaf8:    2300        .#      MOVS     r3,#0
        0x0012aafa:    4632        2F      MOV      r2,r6
        0x0012aafc:    4629        )F      MOV      r1,r5
        0x0012aafe:    4620         F      MOV      r0,r4
        0x0012ab00:    f000fb66    ..f.    BL       uart_write ; 0x12b1d0
        0x0012ab04:    bd70        p.      POP      {r4-r6,pc}
        0x0012ab06:    0000        ..      MOVS     r0,r0
    i.uart_erbfi_setf
    uart_erbfi_setf
        0x0012ab08:    4904        .I      LDR      r1,[pc,#16] ; [0x12ab1c] = 0x20143000
        0x0012ab0a:    6849        Ih      LDR      r1,[r1,#4]
        0x0012ab0c:    0849        I.      LSRS     r1,r1,#1
        0x0012ab0e:    0049        I.      LSLS     r1,r1,#1
        0x0012ab10:    07c2        ..      LSLS     r2,r0,#31
        0x0012ab12:    0fd2        ..      LSRS     r2,r2,#31
        0x0012ab14:    4311        .C      ORRS     r1,r1,r2
        0x0012ab16:    4a01        .J      LDR      r2,[pc,#4] ; [0x12ab1c] = 0x20143000
        0x0012ab18:    6051        Q`      STR      r1,[r2,#4]
        0x0012ab1a:    4770        pG      BX       lr
    $d
        0x0012ab1c:    20143000    .0.     DCD    538193920
    $t
    i.uart_etbei_setf
    uart_etbei_setf
        0x0012ab20:    4905        .I      LDR      r1,[pc,#20] ; [0x12ab38] = 0x20143000
        0x0012ab22:    6849        Ih      LDR      r1,[r1,#4]
        0x0012ab24:    2202        ."      MOVS     r2,#2
        0x0012ab26:    4391        .C      BICS     r1,r1,r2
        0x0012ab28:    0042        B.      LSLS     r2,r0,#1
        0x0012ab2a:    2302        .#      MOVS     r3,#2
        0x0012ab2c:    401a        .@      ANDS     r2,r2,r3
        0x0012ab2e:    4311        .C      ORRS     r1,r1,r2
        0x0012ab30:    4a01        .J      LDR      r2,[pc,#4] ; [0x12ab38] = 0x20143000
        0x0012ab32:    6051        Q`      STR      r1,[r2,#4]
        0x0012ab34:    4770        pG      BX       lr
    $d
        0x0012ab36:    0000        ..      DCW    0
        0x0012ab38:    20143000    .0.     DCD    538193920
    $t
    i.uart_init
    uart_init
        0x0012ab3c:    b5f1        ..      PUSH     {r0,r4-r7,lr}
        0x0012ab3e:    b098        ..      SUB      sp,sp,#0x60
        0x0012ab40:    2000        .       MOVS     r0,#0
        0x0012ab42:    9014        ..      STR      r0,[sp,#0x50]
        0x0012ab44:    9015        ..      STR      r0,[sp,#0x54]
        0x0012ab46:    9016        ..      STR      r0,[sp,#0x58]
        0x0012ab48:    9017        ..      STR      r0,[sp,#0x5c]
        0x0012ab4a:    9010        ..      STR      r0,[sp,#0x40]
        0x0012ab4c:    9011        ..      STR      r0,[sp,#0x44]
        0x0012ab4e:    9012        ..      STR      r0,[sp,#0x48]
        0x0012ab50:    9013        ..      STR      r0,[sp,#0x4c]
        0x0012ab52:    2001        .       MOVS     r0,#1
        0x0012ab54:    2100        .!      MOVS     r1,#0
        0x0012ab56:    910c        ..      STR      r1,[sp,#0x30]
        0x0012ab58:    900d        ..      STR      r0,[sp,#0x34]
        0x0012ab5a:    910e        ..      STR      r1,[sp,#0x38]
        0x0012ab5c:    9108        ..      STR      r1,[sp,#0x20]
        0x0012ab5e:    9109        ..      STR      r1,[sp,#0x24]
        0x0012ab60:    910a        ..      STR      r1,[sp,#0x28]
        0x0012ab62:    910b        ..      STR      r1,[sp,#0x2c]
        0x0012ab64:    9104        ..      STR      r1,[sp,#0x10]
        0x0012ab66:    9105        ..      STR      r1,[sp,#0x14]
        0x0012ab68:    9106        ..      STR      r1,[sp,#0x18]
        0x0012ab6a:    9107        ..      STR      r1,[sp,#0x1c]
        0x0012ab6c:    900f        ..      STR      r0,[sp,#0x3c]
        0x0012ab6e:    2000        .       MOVS     r0,#0
        0x0012ab70:    9000        ..      STR      r0,[sp,#0]
        0x0012ab72:    9001        ..      STR      r0,[sp,#4]
        0x0012ab74:    9002        ..      STR      r0,[sp,#8]
        0x0012ab76:    4603        .F      MOV      r3,r0
        0x0012ab78:    4602        .F      MOV      r2,r0
        0x0012ab7a:    4601        .F      MOV      r1,r0
        0x0012ab7c:    9003        ..      STR      r0,[sp,#0xc]
        0x0012ab7e:    f7ffff11    ....    BL       sysc_per_clkg0_pack ; 0x12a9a4
        0x0012ab82:    9818        ..      LDR      r0,[sp,#0x60]
        0x0012ab84:    f000fabc    ....    BL       uart_set_baudrate ; 0x12b100
        0x0012ab88:    2000        .       MOVS     r0,#0
        0x0012ab8a:    4921        !I      LDR      r1,[pc,#132] ; [0x12ac10] = 0x20143000
        0x0012ab8c:    68c9        .h      LDR      r1,[r1,#0xc]
        0x0012ab8e:    2208        ."      MOVS     r2,#8
        0x0012ab90:    4391        .C      BICS     r1,r1,r2
        0x0012ab92:    00c2        ..      LSLS     r2,r0,#3
        0x0012ab94:    2308        .#      MOVS     r3,#8
        0x0012ab96:    401a        .@      ANDS     r2,r2,r3
        0x0012ab98:    4311        .C      ORRS     r1,r1,r2
        0x0012ab9a:    4a1d        .J      LDR      r2,[pc,#116] ; [0x12ac10] = 0x20143000
        0x0012ab9c:    60d1        .`      STR      r1,[r2,#0xc]
        0x0012ab9e:    bf00        ..      NOP      
        0x0012aba0:    bf00        ..      NOP      
        0x0012aba2:    4611        .F      MOV      r1,r2
        0x0012aba4:    68c9        .h      LDR      r1,[r1,#0xc]
        0x0012aba6:    2204        ."      MOVS     r2,#4
        0x0012aba8:    4391        .C      BICS     r1,r1,r2
        0x0012abaa:    0082        ..      LSLS     r2,r0,#2
        0x0012abac:    2304        .#      MOVS     r3,#4
        0x0012abae:    401a        .@      ANDS     r2,r2,r3
        0x0012abb0:    4311        .C      ORRS     r1,r1,r2
        0x0012abb2:    4a17        .J      LDR      r2,[pc,#92] ; [0x12ac10] = 0x20143000
        0x0012abb4:    60d1        .`      STR      r1,[r2,#0xc]
        0x0012abb6:    bf00        ..      NOP      
        0x0012abb8:    2003        .       MOVS     r0,#3
        0x0012abba:    4611        .F      MOV      r1,r2
        0x0012abbc:    68c9        .h      LDR      r1,[r1,#0xc]
        0x0012abbe:    4381        .C      BICS     r1,r1,r0
        0x0012abc0:    4301        .C      ORRS     r1,r1,r0
        0x0012abc2:    60d1        .`      STR      r1,[r2,#0xc]
        0x0012abc4:    bf00        ..      NOP      
        0x0012abc6:    2000        .       MOVS     r0,#0
        0x0012abc8:    4601        .F      MOV      r1,r0
        0x0012abca:    2201        ."      MOVS     r2,#1
        0x0012abcc:    4603        .F      MOV      r3,r0
        0x0012abce:    4604        .F      MOV      r4,r0
        0x0012abd0:    01c5        ..      LSLS     r5,r0,#7
        0x0012abd2:    00ce        ..      LSLS     r6,r1,#3
        0x0012abd4:    4335        5C      ORRS     r5,r5,r6
        0x0012abd6:    0096        ..      LSLS     r6,r2,#2
        0x0012abd8:    4335        5C      ORRS     r5,r5,r6
        0x0012abda:    005e        ^.      LSLS     r6,r3,#1
        0x0012abdc:    4335        5C      ORRS     r5,r5,r6
        0x0012abde:    4325        %C      ORRS     r5,r5,r4
        0x0012abe0:    4e0b        .N      LDR      r6,[pc,#44] ; [0x12ac10] = 0x20143000
        0x0012abe2:    6075        u`      STR      r5,[r6,#4]
        0x0012abe4:    bf00        ..      NOP      
        0x0012abe6:    4602        .F      MOV      r2,r0
        0x0012abe8:    2501        .%      MOVS     r5,#1
        0x0012abea:    0186        ..      LSLS     r6,r0,#6
        0x0012abec:    010f        ..      LSLS     r7,r1,#4
        0x0012abee:    433e        >C      ORRS     r6,r6,r7
        0x0012abf0:    00d7        ..      LSLS     r7,r2,#3
        0x0012abf2:    433e        >C      ORRS     r6,r6,r7
        0x0012abf4:    009f        ..      LSLS     r7,r3,#2
        0x0012abf6:    433e        >C      ORRS     r6,r6,r7
        0x0012abf8:    0067        g.      LSLS     r7,r4,#1
        0x0012abfa:    433e        >C      ORRS     r6,r6,r7
        0x0012abfc:    432e        .C      ORRS     r6,r6,r5
        0x0012abfe:    4f04        .O      LDR      r7,[pc,#16] ; [0x12ac10] = 0x20143000
        0x0012ac00:    60be        .`      STR      r6,[r7,#8]
        0x0012ac02:    bf00        ..      NOP      
        0x0012ac04:    2100        .!      MOVS     r1,#0
        0x0012ac06:    4803        .H      LDR      r0,[pc,#12] ; [0x12ac14] = 0x12c6a4
        0x0012ac08:    7001        .p      STRB     r1,[r0,#0]
        0x0012ac0a:    b019        ..      ADD      sp,sp,#0x64
        0x0012ac0c:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012ac0e:    0000        ..      DCW    0
        0x0012ac10:    20143000    .0.     DCD    538193920
        0x0012ac14:    0012c6a4    ....    DCD    1230500
    $t
    i.uart_isr
    uart_isr
        0x0012ac18:    b510        ..      PUSH     {r4,lr}
        0x0012ac1a:    e01f        ..      B        0x12ac5c ; uart_isr + 68
        0x0012ac1c:    bf00        ..      NOP      
        0x0012ac1e:    4811        .H      LDR      r0,[pc,#68] ; [0x12ac64] = 0x20143000
        0x0012ac20:    6880        .h      LDR      r0,[r0,#8]
        0x0012ac22:    0704        ..      LSLS     r4,r0,#28
        0x0012ac24:    0f24        $.      LSRS     r4,r4,#28
        0x0012ac26:    bf00        ..      NOP      
        0x0012ac28:    2c01        .,      CMP      r4,#1
        0x0012ac2a:    d100        ..      BNE      0x12ac2e ; uart_isr + 22
        0x0012ac2c:    e017        ..      B        0x12ac5e ; uart_isr + 70
        0x0012ac2e:    2c02        .,      CMP      r4,#2
        0x0012ac30:    d00c        ..      BEQ      0x12ac4c ; uart_isr + 52
        0x0012ac32:    2c04        .,      CMP      r4,#4
        0x0012ac34:    d007        ..      BEQ      0x12ac46 ; uart_isr + 46
        0x0012ac36:    2c06        .,      CMP      r4,#6
        0x0012ac38:    d002        ..      BEQ      0x12ac40 ; uart_isr + 40
        0x0012ac3a:    2c0c        .,      CMP      r4,#0xc
        0x0012ac3c:    d10c        ..      BNE      0x12ac58 ; uart_isr + 64
        0x0012ac3e:    e008        ..      B        0x12ac52 ; uart_isr + 58
        0x0012ac40:    f000f874    ..t.    BL       uart_rec_error_isr ; 0x12ad2c
        0x0012ac44:    e009        ..      B        0x12ac5a ; uart_isr + 66
        0x0012ac46:    f000f82d    ..-.    BL       uart_rec_data_avail_isr ; 0x12aca4
        0x0012ac4a:    e006        ..      B        0x12ac5a ; uart_isr + 66
        0x0012ac4c:    f000fa78    ..x.    BL       uart_thr_empty_isr ; 0x12b140
        0x0012ac50:    e003        ..      B        0x12ac5a ; uart_isr + 66
        0x0012ac52:    f000f827    ..'.    BL       uart_rec_data_avail_isr ; 0x12aca4
        0x0012ac56:    e000        ..      B        0x12ac5a ; uart_isr + 66
        0x0012ac58:    bf00        ..      NOP      
        0x0012ac5a:    bf00        ..      NOP      
        0x0012ac5c:    e7de        ..      B        0x12ac1c ; uart_isr + 4
        0x0012ac5e:    bf00        ..      NOP      
        0x0012ac60:    bd10        ..      POP      {r4,pc}
    $d
        0x0012ac62:    0000        ..      DCW    0
        0x0012ac64:    20143000    .0.     DCD    538193920
    $t
    i.uart_read
    uart_read
        0x0012ac68:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012ac6a:    4604        .F      MOV      r4,r0
        0x0012ac6c:    460d        .F      MOV      r5,r1
        0x0012ac6e:    4616        .F      MOV      r6,r2
        0x0012ac70:    461f        .F      MOV      r7,r3
        0x0012ac72:    4804        .H      LDR      r0,[pc,#16] ; [0x12ac84] = 0x12c684
        0x0012ac74:    6105        .a      STR      r5,[r0,#0x10]
        0x0012ac76:    6144        Da      STR      r4,[r0,#0x14]
        0x0012ac78:    6186        .a      STR      r6,[r0,#0x18]
        0x0012ac7a:    61c7        .a      STR      r7,[r0,#0x1c]
        0x0012ac7c:    2001        .       MOVS     r0,#1
        0x0012ac7e:    f7ffff43    ..C.    BL       uart_erbfi_setf ; 0x12ab08
        0x0012ac82:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012ac84:    0012c684    ....    DCD    1230468
    $t
    i.uart_read32_register
    uart_read32_register
        0x0012ac88:    b530        0.      PUSH     {r4,r5,lr}
        0x0012ac8a:    4603        .F      MOV      r3,r0
        0x0012ac8c:    2000        .       MOVS     r0,#0
        0x0012ac8e:    e005        ..      B        0x12ac9c ; uart_read32_register + 20
        0x0012ac90:    680c        .h      LDR      r4,[r1,#0]
        0x0012ac92:    0085        ..      LSLS     r5,r0,#2
        0x0012ac94:    515c        \Q      STR      r4,[r3,r5]
        0x0012ac96:    1d09        ..      ADDS     r1,r1,#4
        0x0012ac98:    1c44        D.      ADDS     r4,r0,#1
        0x0012ac9a:    b2a0        ..      UXTH     r0,r4
        0x0012ac9c:    4290        .B      CMP      r0,r2
        0x0012ac9e:    dbf7        ..      BLT      0x12ac90 ; uart_read32_register + 8
        0x0012aca0:    bd30        0.      POP      {r4,r5,pc}
        0x0012aca2:    0000        ..      MOVS     r0,r0
    i.uart_rec_data_avail_isr
    uart_rec_data_avail_isr
        0x0012aca4:    b570        p.      PUSH     {r4-r6,lr}
        0x0012aca6:    2400        .$      MOVS     r4,#0
        0x0012aca8:    2500        .%      MOVS     r5,#0
        0x0012acaa:    e032        2.      B        0x12ad12 ; uart_rec_data_avail_isr + 110
        0x0012acac:    bf00        ..      NOP      
        0x0012acae:    481d        .H      LDR      r0,[pc,#116] ; [0x12ad24] = 0x20143000
        0x0012acb0:    6800        .h      LDR      r0,[r0,#0]
        0x0012acb2:    b2c1        ..      UXTB     r1,r0
        0x0012acb4:    481c        .H      LDR      r0,[pc,#112] ; [0x12ad28] = 0x12c684
        0x0012acb6:    6940        @i      LDR      r0,[r0,#0x14]
        0x0012acb8:    7001        .p      STRB     r1,[r0,#0]
        0x0012acba:    481b        .H      LDR      r0,[pc,#108] ; [0x12ad28] = 0x12c684
        0x0012acbc:    6900        .i      LDR      r0,[r0,#0x10]
        0x0012acbe:    1e40        @.      SUBS     r0,r0,#1
        0x0012acc0:    4919        .I      LDR      r1,[pc,#100] ; [0x12ad28] = 0x12c684
        0x0012acc2:    6108        .a      STR      r0,[r1,#0x10]
        0x0012acc4:    4608        .F      MOV      r0,r1
        0x0012acc6:    6940        @i      LDR      r0,[r0,#0x14]
        0x0012acc8:    1c40        @.      ADDS     r0,r0,#1
        0x0012acca:    6148        Ha      STR      r0,[r1,#0x14]
        0x0012accc:    4608        .F      MOV      r0,r1
        0x0012acce:    6900        .i      LDR      r0,[r0,#0x10]
        0x0012acd0:    2800        .(      CMP      r0,#0
        0x0012acd2:    d11e        ..      BNE      0x12ad12 ; uart_rec_data_avail_isr + 110
        0x0012acd4:    2100        .!      MOVS     r1,#0
        0x0012acd6:    4814        .H      LDR      r0,[pc,#80] ; [0x12ad28] = 0x12c684
        0x0012acd8:    6141        Aa      STR      r1,[r0,#0x14]
        0x0012acda:    2000        .       MOVS     r0,#0
        0x0012acdc:    f7ffff14    ....    BL       uart_erbfi_setf ; 0x12ab08
        0x0012ace0:    4811        .H      LDR      r0,[pc,#68] ; [0x12ad28] = 0x12c684
        0x0012ace2:    6984        .i      LDR      r4,[r0,#0x18]
        0x0012ace4:    69c5        .i      LDR      r5,[r0,#0x1c]
        0x0012ace6:    2c00        .,      CMP      r4,#0
        0x0012ace8:    d011        ..      BEQ      0x12ad0e ; uart_rec_data_avail_isr + 106
        0x0012acea:    2100        .!      MOVS     r1,#0
        0x0012acec:    6181        .a      STR      r1,[r0,#0x18]
        0x0012acee:    61c1        .a      STR      r1,[r0,#0x1c]
        0x0012acf0:    3020         0      ADDS     r0,r0,#0x20
        0x0012acf2:    7800        .x      LDRB     r0,[r0,#0]
        0x0012acf4:    2800        .(      CMP      r0,#0
        0x0012acf6:    d102        ..      BNE      0x12acfe ; uart_rec_data_avail_isr + 90
        0x0012acf8:    4628        (F      MOV      r0,r5
        0x0012acfa:    47a0        .G      BLX      r4
        0x0012acfc:    e008        ..      B        0x12ad10 ; uart_rec_data_avail_isr + 108
        0x0012acfe:    2100        .!      MOVS     r1,#0
        0x0012ad00:    4809        .H      LDR      r0,[pc,#36] ; [0x12ad28] = 0x12c684
        0x0012ad02:    3020         0      ADDS     r0,r0,#0x20
        0x0012ad04:    7001        .p      STRB     r1,[r0,#0]
        0x0012ad06:    2101        .!      MOVS     r1,#1
        0x0012ad08:    4628        (F      MOV      r0,r5
        0x0012ad0a:    47a0        .G      BLX      r4
        0x0012ad0c:    e000        ..      B        0x12ad10 ; uart_rec_data_avail_isr + 108
        0x0012ad0e:    bf00        ..      NOP      
        0x0012ad10:    e006        ..      B        0x12ad20 ; uart_rec_data_avail_isr + 124
        0x0012ad12:    bf00        ..      NOP      
        0x0012ad14:    4803        .H      LDR      r0,[pc,#12] ; [0x12ad24] = 0x20143000
        0x0012ad16:    6940        @i      LDR      r0,[r0,#0x14]
        0x0012ad18:    07c0        ..      LSLS     r0,r0,#31
        0x0012ad1a:    0fc0        ..      LSRS     r0,r0,#31
        0x0012ad1c:    2800        .(      CMP      r0,#0
        0x0012ad1e:    d1c5        ..      BNE      0x12acac ; uart_rec_data_avail_isr + 8
        0x0012ad20:    bf00        ..      NOP      
        0x0012ad22:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012ad24:    20143000    .0.     DCD    538193920
        0x0012ad28:    0012c684    ....    DCD    1230468
    $t
    i.uart_rec_error_isr
    uart_rec_error_isr
        0x0012ad2c:    b570        p.      PUSH     {r4-r6,lr}
        0x0012ad2e:    2400        .$      MOVS     r4,#0
        0x0012ad30:    2600        .&      MOVS     r6,#0
        0x0012ad32:    bf00        ..      NOP      
        0x0012ad34:    4812        .H      LDR      r0,[pc,#72] ; [0x12ad80] = 0x20143000
        0x0012ad36:    6940        @i      LDR      r0,[r0,#0x14]
        0x0012ad38:    b2c5        ..      UXTB     r5,r0
        0x0012ad3a:    07a8        ..      LSLS     r0,r5,#30
        0x0012ad3c:    0fc0        ..      LSRS     r0,r0,#31
        0x0012ad3e:    2800        .(      CMP      r0,#0
        0x0012ad40:    d014        ..      BEQ      0x12ad6c ; uart_rec_error_isr + 64
        0x0012ad42:    e003        ..      B        0x12ad4c ; uart_rec_error_isr + 32
        0x0012ad44:    bf00        ..      NOP      
        0x0012ad46:    480e        .H      LDR      r0,[pc,#56] ; [0x12ad80] = 0x20143000
        0x0012ad48:    6800        .h      LDR      r0,[r0,#0]
        0x0012ad4a:    bf00        ..      NOP      
        0x0012ad4c:    bf00        ..      NOP      
        0x0012ad4e:    480c        .H      LDR      r0,[pc,#48] ; [0x12ad80] = 0x20143000
        0x0012ad50:    6940        @i      LDR      r0,[r0,#0x14]
        0x0012ad52:    07c0        ..      LSLS     r0,r0,#31
        0x0012ad54:    0fc0        ..      LSRS     r0,r0,#31
        0x0012ad56:    2800        .(      CMP      r0,#0
        0x0012ad58:    d1f4        ..      BNE      0x12ad44 ; uart_rec_error_isr + 24
        0x0012ad5a:    2c00        .,      CMP      r4,#0
        0x0012ad5c:    d006        ..      BEQ      0x12ad6c ; uart_rec_error_isr + 64
        0x0012ad5e:    2100        .!      MOVS     r1,#0
        0x0012ad60:    4808        .H      LDR      r0,[pc,#32] ; [0x12ad84] = 0x12c684
        0x0012ad62:    6181        .a      STR      r1,[r0,#0x18]
        0x0012ad64:    61c1        .a      STR      r1,[r0,#0x1c]
        0x0012ad66:    2101        .!      MOVS     r1,#1
        0x0012ad68:    4630        0F      MOV      r0,r6
        0x0012ad6a:    47a0        .G      BLX      r4
        0x0012ad6c:    0728        (.      LSLS     r0,r5,#28
        0x0012ad6e:    0fc0        ..      LSRS     r0,r0,#31
        0x0012ad70:    2800        .(      CMP      r0,#0
        0x0012ad72:    d003        ..      BEQ      0x12ad7c ; uart_rec_error_isr + 80
        0x0012ad74:    2101        .!      MOVS     r1,#1
        0x0012ad76:    4803        .H      LDR      r0,[pc,#12] ; [0x12ad84] = 0x12c684
        0x0012ad78:    3020         0      ADDS     r0,r0,#0x20
        0x0012ad7a:    7001        .p      STRB     r1,[r0,#0]
        0x0012ad7c:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012ad7e:    0000        ..      DCW    0
        0x0012ad80:    20143000    .0.     DCD    538193920
        0x0012ad84:    0012c684    ....    DCD    1230468
    $t
    i.uart_rx_header_finish
    uart_rx_header_finish
        0x0012ad88:    b510        ..      PUSH     {r4,lr}
        0x0012ad8a:    4804        .H      LDR      r0,[pc,#16] ; [0x12ad9c] = 0x12b67c
        0x0012ad8c:    8841        A.      LDRH     r1,[r0,#2]
        0x0012ad8e:    4804        .H      LDR      r0,[pc,#16] ; [0x12ada0] = 0x12b25c
        0x0012ad90:    4a04        .J      LDR      r2,[pc,#16] ; [0x12ada4] = 0x12ada9
        0x0012ad92:    6843        Ch      LDR      r3,[r0,#4]
        0x0012ad94:    4801        .H      LDR      r0,[pc,#4] ; [0x12ad9c] = 0x12b67c
        0x0012ad96:    3008        .0      ADDS     r0,r0,#8
        0x0012ad98:    4798        .G      BLX      r3
        0x0012ad9a:    bd10        ..      POP      {r4,pc}
    $d
        0x0012ad9c:    0012b67c    |...    DCD    1226364
        0x0012ada0:    0012b25c    \...    DCD    1225308
        0x0012ada4:    0012ada9    ....    DCD    1224105
    $t
    i.uart_rx_payload_finish
    uart_rx_payload_finish
        0x0012ada8:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012adaa:    48c9        .H      LDR      r0,[pc,#804] ; [0x12b0d0] = 0x12b67c
        0x0012adac:    7800        .x      LDRB     r0,[r0,#0]
        0x0012adae:    28ad        .(      CMP      r0,#0xad
        0x0012adb0:    d006        ..      BEQ      0x12adc0 ; uart_rx_payload_finish + 24
        0x0012adb2:    48c8        .H      LDR      r0,[pc,#800] ; [0x12b0d4] = 0x12b25c
        0x0012adb4:    4ac8        .J      LDR      r2,[pc,#800] ; [0x12b0d8] = 0x12b1b1
        0x0012adb6:    2105        .!      MOVS     r1,#5
        0x0012adb8:    6803        .h      LDR      r3,[r0,#0]
        0x0012adba:    48c8        .H      LDR      r0,[pc,#800] ; [0x12b0dc] = 0x12b26e
        0x0012adbc:    4798        .G      BLX      r3
        0x0012adbe:    e17e        ~.      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012adc0:    48c3        .H      LDR      r0,[pc,#780] ; [0x12b0d0] = 0x12b67c
        0x0012adc2:    7840        @x      LDRB     r0,[r0,#1]
        0x0012adc4:    0740        @.      LSLS     r0,r0,#29
        0x0012adc6:    0fc0        ..      LSRS     r0,r0,#31
        0x0012adc8:    2800        .(      CMP      r0,#0
        0x0012adca:    d17d        }.      BNE      0x12aec8 ; uart_rx_payload_finish + 288
        0x0012adcc:    48c0        .H      LDR      r0,[pc,#768] ; [0x12b0d0] = 0x12b67c
        0x0012adce:    7840        @x      LDRB     r0,[r0,#1]
        0x0012add0:    08c0        ..      LSRS     r0,r0,#3
        0x0012add2:    2801        .(      CMP      r0,#1
        0x0012add4:    d133        3.      BNE      0x12ae3e ; uart_rx_payload_finish + 150
        0x0012add6:    48be        .H      LDR      r0,[pc,#760] ; [0x12b0d0] = 0x12b67c
        0x0012add8:    8840        @.      LDRH     r0,[r0,#2]
        0x0012adda:    2802        .(      CMP      r0,#2
        0x0012addc:    d12f        /.      BNE      0x12ae3e ; uart_rx_payload_finish + 150
        0x0012adde:    48bc        .H      LDR      r0,[pc,#752] ; [0x12b0d0] = 0x12b67c
        0x0012ade0:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012ade2:    2846        F(      CMP      r0,#0x46
        0x0012ade4:    d107        ..      BNE      0x12adf6 ; uart_rx_payload_finish + 78
        0x0012ade6:    48ba        .H      LDR      r0,[pc,#744] ; [0x12b0d0] = 0x12b67c
        0x0012ade8:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012adea:    2844        D(      CMP      r0,#0x44
        0x0012adec:    d103        ..      BNE      0x12adf6 ; uart_rx_payload_finish + 78
        0x0012adee:    2001        .       MOVS     r0,#1
        0x0012adf0:    49bb        .I      LDR      r1,[pc,#748] ; [0x12b0e0] = 0x12b254
        0x0012adf2:    7008        .p      STRB     r0,[r1,#0]
        0x0012adf4:    e163        c.      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012adf6:    48b6        .H      LDR      r0,[pc,#728] ; [0x12b0d0] = 0x12b67c
        0x0012adf8:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012adfa:    2843        C(      CMP      r0,#0x43
        0x0012adfc:    d107        ..      BNE      0x12ae0e ; uart_rx_payload_finish + 102
        0x0012adfe:    48b4        .H      LDR      r0,[pc,#720] ; [0x12b0d0] = 0x12b67c
        0x0012ae00:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012ae02:    2846        F(      CMP      r0,#0x46
        0x0012ae04:    d103        ..      BNE      0x12ae0e ; uart_rx_payload_finish + 102
        0x0012ae06:    20ff        .       MOVS     r0,#0xff
        0x0012ae08:    49b6        .I      LDR      r1,[pc,#728] ; [0x12b0e4] = 0x12b255
        0x0012ae0a:    7008        .p      STRB     r0,[r1,#0]
        0x0012ae0c:    bdfe        ..      POP      {r1-r7,pc}
        0x0012ae0e:    48b0        .H      LDR      r0,[pc,#704] ; [0x12b0d0] = 0x12b67c
        0x0012ae10:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012ae12:    2843        C(      CMP      r0,#0x43
        0x0012ae14:    d107        ..      BNE      0x12ae26 ; uart_rx_payload_finish + 126
        0x0012ae16:    48ae        .H      LDR      r0,[pc,#696] ; [0x12b0d0] = 0x12b67c
        0x0012ae18:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012ae1a:    2850        P(      CMP      r0,#0x50
        0x0012ae1c:    d103        ..      BNE      0x12ae26 ; uart_rx_payload_finish + 126
        0x0012ae1e:    2001        .       MOVS     r0,#1
        0x0012ae20:    49b0        .I      LDR      r1,[pc,#704] ; [0x12b0e4] = 0x12b255
        0x0012ae22:    7008        .p      STRB     r0,[r1,#0]
        0x0012ae24:    e7f2        ..      B        0x12ae0c ; uart_rx_payload_finish + 100
        0x0012ae26:    48aa        .H      LDR      r0,[pc,#680] ; [0x12b0d0] = 0x12b67c
        0x0012ae28:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012ae2a:    2850        P(      CMP      r0,#0x50
        0x0012ae2c:    d1c7        ..      BNE      0x12adbe ; uart_rx_payload_finish + 22
        0x0012ae2e:    48a8        .H      LDR      r0,[pc,#672] ; [0x12b0d0] = 0x12b67c
        0x0012ae30:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012ae32:    2852        R(      CMP      r0,#0x52
        0x0012ae34:    d1c3        ..      BNE      0x12adbe ; uart_rx_payload_finish + 22
        0x0012ae36:    2001        .       MOVS     r0,#1
        0x0012ae38:    49ab        .I      LDR      r1,[pc,#684] ; [0x12b0e8] = 0x12b256
        0x0012ae3a:    7008        .p      STRB     r0,[r1,#0]
        0x0012ae3c:    e7e6        ..      B        0x12ae0c ; uart_rx_payload_finish + 100
        0x0012ae3e:    48a4        .H      LDR      r0,[pc,#656] ; [0x12b0d0] = 0x12b67c
        0x0012ae40:    7840        @x      LDRB     r0,[r0,#1]
        0x0012ae42:    08c0        ..      LSRS     r0,r0,#3
        0x0012ae44:    2802        .(      CMP      r0,#2
        0x0012ae46:    d112        ..      BNE      0x12ae6e ; uart_rx_payload_finish + 198
        0x0012ae48:    48a1        .H      LDR      r0,[pc,#644] ; [0x12b0d0] = 0x12b67c
        0x0012ae4a:    8840        @.      LDRH     r0,[r0,#2]
        0x0012ae4c:    2802        .(      CMP      r0,#2
        0x0012ae4e:    d10e        ..      BNE      0x12ae6e ; uart_rx_payload_finish + 198
        0x0012ae50:    2000        .       MOVS     r0,#0
        0x0012ae52:    9001        ..      STR      r0,[sp,#4]
        0x0012ae54:    489e        .H      LDR      r0,[pc,#632] ; [0x12b0d0] = 0x12b67c
        0x0012ae56:    7a07        .z      LDRB     r7,[r0,#8]
        0x0012ae58:    4638        8F      MOV      r0,r7
        0x0012ae5a:    f7fffbb7    ....    BL       flash_status_read ; 0x12a5cc
        0x0012ae5e:    9001        ..      STR      r0,[sp,#4]
        0x0012ae60:    489c        .H      LDR      r0,[pc,#624] ; [0x12b0d4] = 0x12b25c
        0x0012ae62:    4a9d        .J      LDR      r2,[pc,#628] ; [0x12b0d8] = 0x12b1b1
        0x0012ae64:    2101        .!      MOVS     r1,#1
        0x0012ae66:    6803        .h      LDR      r3,[r0,#0]
        0x0012ae68:    a801        ..      ADD      r0,sp,#4
        0x0012ae6a:    4798        .G      BLX      r3
        0x0012ae6c:    e127        '.      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012ae6e:    4898        .H      LDR      r0,[pc,#608] ; [0x12b0d0] = 0x12b67c
        0x0012ae70:    7840        @x      LDRB     r0,[r0,#1]
        0x0012ae72:    08c0        ..      LSRS     r0,r0,#3
        0x0012ae74:    2803        .(      CMP      r0,#3
        0x0012ae76:    d115        ..      BNE      0x12aea4 ; uart_rx_payload_finish + 252
        0x0012ae78:    4895        .H      LDR      r0,[pc,#596] ; [0x12b0d0] = 0x12b67c
        0x0012ae7a:    8840        @.      LDRH     r0,[r0,#2]
        0x0012ae7c:    2802        .(      CMP      r0,#2
        0x0012ae7e:    d111        ..      BNE      0x12aea4 ; uart_rx_payload_finish + 252
        0x0012ae80:    4893        .H      LDR      r0,[pc,#588] ; [0x12b0d0] = 0x12b67c
        0x0012ae82:    7a04        .z      LDRB     r4,[r0,#8]
        0x0012ae84:    4601        .F      MOV      r1,r0
        0x0012ae86:    7a49        Iz      LDRB     r1,[r1,#9]
        0x0012ae88:    0209        ..      LSLS     r1,r1,#8
        0x0012ae8a:    430c        .C      ORRS     r4,r4,r1
        0x0012ae8c:    4622        "F      MOV      r2,r4
        0x0012ae8e:    6841        Ah      LDR      r1,[r0,#4]
        0x0012ae90:    4896        .H      LDR      r0,[pc,#600] ; [0x12b0ec] = 0x12b27c
        0x0012ae92:    f7fffef9    ....    BL       uart_read32_register ; 0x12ac88
        0x0012ae96:    00a1        ..      LSLS     r1,r4,#2
        0x0012ae98:    488e        .H      LDR      r0,[pc,#568] ; [0x12b0d4] = 0x12b25c
        0x0012ae9a:    4a8f        .J      LDR      r2,[pc,#572] ; [0x12b0d8] = 0x12b1b1
        0x0012ae9c:    6803        .h      LDR      r3,[r0,#0]
        0x0012ae9e:    4893        .H      LDR      r0,[pc,#588] ; [0x12b0ec] = 0x12b27c
        0x0012aea0:    4798        .G      BLX      r3
        0x0012aea2:    e10c        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012aea4:    488a        .H      LDR      r0,[pc,#552] ; [0x12b0d0] = 0x12b67c
        0x0012aea6:    7840        @x      LDRB     r0,[r0,#1]
        0x0012aea8:    08c0        ..      LSRS     r0,r0,#3
        0x0012aeaa:    2804        .(      CMP      r0,#4
        0x0012aeac:    d121        !.      BNE      0x12aef2 ; uart_rx_payload_finish + 330
        0x0012aeae:    4888        .H      LDR      r0,[pc,#544] ; [0x12b0d0] = 0x12b67c
        0x0012aeb0:    8840        @.      LDRH     r0,[r0,#2]
        0x0012aeb2:    2804        .(      CMP      r0,#4
        0x0012aeb4:    d11d        ..      BNE      0x12aef2 ; uart_rx_payload_finish + 330
        0x0012aeb6:    4886        .H      LDR      r0,[pc,#536] ; [0x12b0d0] = 0x12b67c
        0x0012aeb8:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012aeba:    4985        .I      LDR      r1,[pc,#532] ; [0x12b0d0] = 0x12b67c
        0x0012aebc:    7a49        Iz      LDRB     r1,[r1,#9]
        0x0012aebe:    0209        ..      LSLS     r1,r1,#8
        0x0012aec0:    4308        .C      ORRS     r0,r0,r1
        0x0012aec2:    4983        .I      LDR      r1,[pc,#524] ; [0x12b0d0] = 0x12b67c
        0x0012aec4:    7a89        .z      LDRB     r1,[r1,#0xa]
        0x0012aec6:    e000        ..      B        0x12aeca ; uart_rx_payload_finish + 290
        0x0012aec8:    e078        x.      B        0x12afbc ; uart_rx_payload_finish + 532
        0x0012aeca:    0409        ..      LSLS     r1,r1,#16
        0x0012aecc:    4308        .C      ORRS     r0,r0,r1
        0x0012aece:    4980        .I      LDR      r1,[pc,#512] ; [0x12b0d0] = 0x12b67c
        0x0012aed0:    7ac9        .z      LDRB     r1,[r1,#0xb]
        0x0012aed2:    0609        ..      LSLS     r1,r1,#24
        0x0012aed4:    4308        .C      ORRS     r0,r0,r1
        0x0012aed6:    4607        .F      MOV      r7,r0
        0x0012aed8:    497d        }I      LDR      r1,[pc,#500] ; [0x12b0d0] = 0x12b67c
        0x0012aeda:    6848        Hh      LDR      r0,[r1,#4]
        0x0012aedc:    4639        9F      MOV      r1,r7
        0x0012aede:    f000f987    ....    BL       uart_write_register ; 0x12b1f0
        0x0012aee2:    487c        |H      LDR      r0,[pc,#496] ; [0x12b0d4] = 0x12b25c
        0x0012aee4:    4a7c        |J      LDR      r2,[pc,#496] ; [0x12b0d8] = 0x12b1b1
        0x0012aee6:    2105        .!      MOVS     r1,#5
        0x0012aee8:    6803        .h      LDR      r3,[r0,#0]
        0x0012aeea:    487c        |H      LDR      r0,[pc,#496] ; [0x12b0dc] = 0x12b26e
        0x0012aeec:    1f40        @.      SUBS     r0,r0,#5
        0x0012aeee:    4798        .G      BLX      r3
        0x0012aef0:    e0e5        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012aef2:    4877        wH      LDR      r0,[pc,#476] ; [0x12b0d0] = 0x12b67c
        0x0012aef4:    7840        @x      LDRB     r0,[r0,#1]
        0x0012aef6:    08c0        ..      LSRS     r0,r0,#3
        0x0012aef8:    2805        .(      CMP      r0,#5
        0x0012aefa:    d115        ..      BNE      0x12af28 ; uart_rx_payload_finish + 384
        0x0012aefc:    4874        tH      LDR      r0,[pc,#464] ; [0x12b0d0] = 0x12b67c
        0x0012aefe:    8840        @.      LDRH     r0,[r0,#2]
        0x0012af00:    2802        .(      CMP      r0,#2
        0x0012af02:    d111        ..      BNE      0x12af28 ; uart_rx_payload_finish + 384
        0x0012af04:    2000        .       MOVS     r0,#0
        0x0012af06:    9001        ..      STR      r0,[sp,#4]
        0x0012af08:    4871        qH      LDR      r0,[pc,#452] ; [0x12b0d0] = 0x12b67c
        0x0012af0a:    7a07        .z      LDRB     r7,[r0,#8]
        0x0012af0c:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012af0e:    9000        ..      STR      r0,[sp,#0]
        0x0012af10:    4638        8F      MOV      r0,r7
        0x0012af12:    9900        ..      LDR      r1,[sp,#0]
        0x0012af14:    f7fffb86    ....    BL       flash_status_write ; 0x12a624
        0x0012af18:    9001        ..      STR      r0,[sp,#4]
        0x0012af1a:    486e        nH      LDR      r0,[pc,#440] ; [0x12b0d4] = 0x12b25c
        0x0012af1c:    4a6e        nJ      LDR      r2,[pc,#440] ; [0x12b0d8] = 0x12b1b1
        0x0012af1e:    2101        .!      MOVS     r1,#1
        0x0012af20:    6803        .h      LDR      r3,[r0,#0]
        0x0012af22:    a801        ..      ADD      r0,sp,#4
        0x0012af24:    4798        .G      BLX      r3
        0x0012af26:    e0ca        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012af28:    4869        iH      LDR      r0,[pc,#420] ; [0x12b0d0] = 0x12b67c
        0x0012af2a:    7840        @x      LDRB     r0,[r0,#1]
        0x0012af2c:    08c0        ..      LSRS     r0,r0,#3
        0x0012af2e:    2806        .(      CMP      r0,#6
        0x0012af30:    d115        ..      BNE      0x12af5e ; uart_rx_payload_finish + 438
        0x0012af32:    4867        gH      LDR      r0,[pc,#412] ; [0x12b0d0] = 0x12b67c
        0x0012af34:    8840        @.      LDRH     r0,[r0,#2]
        0x0012af36:    2802        .(      CMP      r0,#2
        0x0012af38:    d111        ..      BNE      0x12af5e ; uart_rx_payload_finish + 438
        0x0012af3a:    2000        .       MOVS     r0,#0
        0x0012af3c:    9001        ..      STR      r0,[sp,#4]
        0x0012af3e:    4864        dH      LDR      r0,[pc,#400] ; [0x12b0d0] = 0x12b67c
        0x0012af40:    7a07        .z      LDRB     r7,[r0,#8]
        0x0012af42:    7a40        @z      LDRB     r0,[r0,#9]
        0x0012af44:    9000        ..      STR      r0,[sp,#0]
        0x0012af46:    4638        8F      MOV      r0,r7
        0x0012af48:    9900        ..      LDR      r1,[sp,#0]
        0x0012af4a:    f7fffbb1    ....    BL       flash_status_write_2bytes ; 0x12a6b0
        0x0012af4e:    9001        ..      STR      r0,[sp,#4]
        0x0012af50:    4860        `H      LDR      r0,[pc,#384] ; [0x12b0d4] = 0x12b25c
        0x0012af52:    4a61        aJ      LDR      r2,[pc,#388] ; [0x12b0d8] = 0x12b1b1
        0x0012af54:    2102        .!      MOVS     r1,#2
        0x0012af56:    6803        .h      LDR      r3,[r0,#0]
        0x0012af58:    a801        ..      ADD      r0,sp,#4
        0x0012af5a:    4798        .G      BLX      r3
        0x0012af5c:    e0af        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012af5e:    485c        \H      LDR      r0,[pc,#368] ; [0x12b0d0] = 0x12b67c
        0x0012af60:    7840        @x      LDRB     r0,[r0,#1]
        0x0012af62:    08c0        ..      LSRS     r0,r0,#3
        0x0012af64:    2807        .(      CMP      r0,#7
        0x0012af66:    d10e        ..      BNE      0x12af86 ; uart_rx_payload_finish + 478
        0x0012af68:    4859        YH      LDR      r0,[pc,#356] ; [0x12b0d0] = 0x12b67c
        0x0012af6a:    8840        @.      LDRH     r0,[r0,#2]
        0x0012af6c:    2802        .(      CMP      r0,#2
        0x0012af6e:    d10a        ..      BNE      0x12af86 ; uart_rx_payload_finish + 478
        0x0012af70:    2001        .       MOVS     r0,#1
        0x0012af72:    495f        _I      LDR      r1,[pc,#380] ; [0x12b0f0] = 0x12b273
        0x0012af74:    7008        .p      STRB     r0,[r1,#0]
        0x0012af76:    4857        WH      LDR      r0,[pc,#348] ; [0x12b0d4] = 0x12b25c
        0x0012af78:    4a57        WJ      LDR      r2,[pc,#348] ; [0x12b0d8] = 0x12b1b1
        0x0012af7a:    2105        .!      MOVS     r1,#5
        0x0012af7c:    6803        .h      LDR      r3,[r0,#0]
        0x0012af7e:    4857        WH      LDR      r0,[pc,#348] ; [0x12b0dc] = 0x12b26e
        0x0012af80:    1f40        @.      SUBS     r0,r0,#5
        0x0012af82:    4798        .G      BLX      r3
        0x0012af84:    e09b        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012af86:    4852        RH      LDR      r0,[pc,#328] ; [0x12b0d0] = 0x12b67c
        0x0012af88:    7840        @x      LDRB     r0,[r0,#1]
        0x0012af8a:    08c0        ..      LSRS     r0,r0,#3
        0x0012af8c:    2808        .(      CMP      r0,#8
        0x0012af8e:    d10e        ..      BNE      0x12afae ; uart_rx_payload_finish + 518
        0x0012af90:    484f        OH      LDR      r0,[pc,#316] ; [0x12b0d0] = 0x12b67c
        0x0012af92:    8840        @.      LDRH     r0,[r0,#2]
        0x0012af94:    2802        .(      CMP      r0,#2
        0x0012af96:    d10a        ..      BNE      0x12afae ; uart_rx_payload_finish + 518
        0x0012af98:    2001        .       MOVS     r0,#1
        0x0012af9a:    4956        VI      LDR      r1,[pc,#344] ; [0x12b0f4] = 0x12b257
        0x0012af9c:    7008        .p      STRB     r0,[r1,#0]
        0x0012af9e:    484d        MH      LDR      r0,[pc,#308] ; [0x12b0d4] = 0x12b25c
        0x0012afa0:    4a4d        MJ      LDR      r2,[pc,#308] ; [0x12b0d8] = 0x12b1b1
        0x0012afa2:    2105        .!      MOVS     r1,#5
        0x0012afa4:    6803        .h      LDR      r3,[r0,#0]
        0x0012afa6:    484d        MH      LDR      r0,[pc,#308] ; [0x12b0dc] = 0x12b26e
        0x0012afa8:    1f40        @.      SUBS     r0,r0,#5
        0x0012afaa:    4798        .G      BLX      r3
        0x0012afac:    e087        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012afae:    4849        IH      LDR      r0,[pc,#292] ; [0x12b0d4] = 0x12b25c
        0x0012afb0:    4a49        IJ      LDR      r2,[pc,#292] ; [0x12b0d8] = 0x12b1b1
        0x0012afb2:    2105        .!      MOVS     r1,#5
        0x0012afb4:    6803        .h      LDR      r3,[r0,#0]
        0x0012afb6:    4849        IH      LDR      r0,[pc,#292] ; [0x12b0dc] = 0x12b26e
        0x0012afb8:    4798        .G      BLX      r3
        0x0012afba:    e080        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012afbc:    4844        DH      LDR      r0,[pc,#272] ; [0x12b0d0] = 0x12b67c
        0x0012afbe:    7840        @x      LDRB     r0,[r0,#1]
        0x0012afc0:    0740        @.      LSLS     r0,r0,#29
        0x0012afc2:    0fc0        ..      LSRS     r0,r0,#31
        0x0012afc4:    2800        .(      CMP      r0,#0
        0x0012afc6:    d07a        z.      BEQ      0x12b0be ; uart_rx_payload_finish + 790
        0x0012afc8:    4841        AH      LDR      r0,[pc,#260] ; [0x12b0d0] = 0x12b67c
        0x0012afca:    7840        @x      LDRB     r0,[r0,#1]
        0x0012afcc:    0780        ..      LSLS     r0,r0,#30
        0x0012afce:    0f80        ..      LSRS     r0,r0,#30
        0x0012afd0:    2800        .(      CMP      r0,#0
        0x0012afd2:    d004        ..      BEQ      0x12afde ; uart_rx_payload_finish + 566
        0x0012afd4:    2801        .(      CMP      r0,#1
        0x0012afd6:    d017        ..      BEQ      0x12b008 ; uart_rx_payload_finish + 608
        0x0012afd8:    2802        .(      CMP      r0,#2
        0x0012afda:    d168        h.      BNE      0x12b0ae ; uart_rx_payload_finish + 774
        0x0012afdc:    e054        T.      B        0x12b088 ; uart_rx_payload_finish + 736
        0x0012afde:    483c        <H      LDR      r0,[pc,#240] ; [0x12b0d0] = 0x12b67c
        0x0012afe0:    7a04        .z      LDRB     r4,[r0,#8]
        0x0012afe2:    4601        .F      MOV      r1,r0
        0x0012afe4:    7a49        Iz      LDRB     r1,[r1,#9]
        0x0012afe6:    0209        ..      LSLS     r1,r1,#8
        0x0012afe8:    430c        .C      ORRS     r4,r4,r1
        0x0012afea:    4601        .F      MOV      r1,r0
        0x0012afec:    460a        .F      MOV      r2,r1
        0x0012afee:    3208        .2      ADDS     r2,r2,#8
        0x0012aff0:    6848        Hh      LDR      r0,[r1,#4]
        0x0012aff2:    4621        !F      MOV      r1,r4
        0x0012aff4:    f7fffa9e    ....    BL       flash_read ; 0x12a534
        0x0012aff8:    4836        6H      LDR      r0,[pc,#216] ; [0x12b0d4] = 0x12b25c
        0x0012affa:    4a37        7J      LDR      r2,[pc,#220] ; [0x12b0d8] = 0x12b1b1
        0x0012affc:    4621        !F      MOV      r1,r4
        0x0012affe:    6803        .h      LDR      r3,[r0,#0]
        0x0012b000:    4833        3H      LDR      r0,[pc,#204] ; [0x12b0d0] = 0x12b67c
        0x0012b002:    3008        .0      ADDS     r0,r0,#8
        0x0012b004:    4798        .G      BLX      r3
        0x0012b006:    e059        Y.      B        0x12b0bc ; uart_rx_payload_finish + 788
        0x0012b008:    4839        9H      LDR      r0,[pc,#228] ; [0x12b0f0] = 0x12b273
        0x0012b00a:    7800        .x      LDRB     r0,[r0,#0]
        0x0012b00c:    2800        .(      CMP      r0,#0
        0x0012b00e:    d124        $.      BNE      0x12b05a ; uart_rx_payload_finish + 690
        0x0012b010:    2600        .&      MOVS     r6,#0
        0x0012b012:    e01d        ..      B        0x12b050 ; uart_rx_payload_finish + 680
        0x0012b014:    482e        .H      LDR      r0,[pc,#184] ; [0x12b0d0] = 0x12b67c
        0x0012b016:    8840        @.      LDRH     r0,[r0,#2]
        0x0012b018:    21ff        .!      MOVS     r1,#0xff
        0x0012b01a:    3101        .1      ADDS     r1,#1
        0x0012b01c:    4288        .B      CMP      r0,r1
        0x0012b01e:    dd01        ..      BLE      0x12b024 ; uart_rx_payload_finish + 636
        0x0012b020:    4608        .F      MOV      r0,r1
        0x0012b022:    e001        ..      B        0x12b028 ; uart_rx_payload_finish + 640
        0x0012b024:    482a        *H      LDR      r0,[pc,#168] ; [0x12b0d0] = 0x12b67c
        0x0012b026:    8840        @.      LDRH     r0,[r0,#2]
        0x0012b028:    4605        .F      MOV      r5,r0
        0x0012b02a:    4929        )I      LDR      r1,[pc,#164] ; [0x12b0d0] = 0x12b67c
        0x0012b02c:    3108        .1      ADDS     r1,r1,#8
        0x0012b02e:    198a        ..      ADDS     r2,r1,r6
        0x0012b030:    3908        .9      SUBS     r1,r1,#8
        0x0012b032:    6848        Hh      LDR      r0,[r1,#4]
        0x0012b034:    4629        )F      MOV      r1,r5
        0x0012b036:    f7fffa28    ..(.    BL       flash_program ; 0x12a48a
        0x0012b03a:    1970        p.      ADDS     r0,r6,r5
        0x0012b03c:    b286        ..      UXTH     r6,r0
        0x0012b03e:    4824        $H      LDR      r0,[pc,#144] ; [0x12b0d0] = 0x12b67c
        0x0012b040:    8840        @.      LDRH     r0,[r0,#2]
        0x0012b042:    1b40        @.      SUBS     r0,r0,r5
        0x0012b044:    4922        "I      LDR      r1,[pc,#136] ; [0x12b0d0] = 0x12b67c
        0x0012b046:    8048        H.      STRH     r0,[r1,#2]
        0x0012b048:    4608        .F      MOV      r0,r1
        0x0012b04a:    6840        @h      LDR      r0,[r0,#4]
        0x0012b04c:    1940        @.      ADDS     r0,r0,r5
        0x0012b04e:    6048        H`      STR      r0,[r1,#4]
        0x0012b050:    481f        .H      LDR      r0,[pc,#124] ; [0x12b0d0] = 0x12b67c
        0x0012b052:    8840        @.      LDRH     r0,[r0,#2]
        0x0012b054:    2800        .(      CMP      r0,#0
        0x0012b056:    d1dd        ..      BNE      0x12b014 ; uart_rx_payload_finish + 620
        0x0012b058:    e00e        ..      B        0x12b078 ; uart_rx_payload_finish + 720
        0x0012b05a:    481d        .H      LDR      r0,[pc,#116] ; [0x12b0d0] = 0x12b67c
        0x0012b05c:    8842        B.      LDRH     r2,[r0,#2]
        0x0012b05e:    4601        .F      MOV      r1,r0
        0x0012b060:    3108        .1      ADDS     r1,r1,#8
        0x0012b062:    4825        %H      LDR      r0,[pc,#148] ; [0x12b0f8] = 0x12b274
        0x0012b064:    6800        .h      LDR      r0,[r0,#0]
        0x0012b066:    f7fff837    ..7.    BL       __aeabi_memcpy ; 0x12a0d8
        0x0012b06a:    4819        .H      LDR      r0,[pc,#100] ; [0x12b0d0] = 0x12b67c
        0x0012b06c:    8840        @.      LDRH     r0,[r0,#2]
        0x0012b06e:    4922        "I      LDR      r1,[pc,#136] ; [0x12b0f8] = 0x12b274
        0x0012b070:    6809        .h      LDR      r1,[r1,#0]
        0x0012b072:    1840        @.      ADDS     r0,r0,r1
        0x0012b074:    4920         I      LDR      r1,[pc,#128] ; [0x12b0f8] = 0x12b274
        0x0012b076:    6008        .`      STR      r0,[r1,#0]
        0x0012b078:    4816        .H      LDR      r0,[pc,#88] ; [0x12b0d4] = 0x12b25c
        0x0012b07a:    4a17        .J      LDR      r2,[pc,#92] ; [0x12b0d8] = 0x12b1b1
        0x0012b07c:    2105        .!      MOVS     r1,#5
        0x0012b07e:    6803        .h      LDR      r3,[r0,#0]
        0x0012b080:    4816        .H      LDR      r0,[pc,#88] ; [0x12b0dc] = 0x12b26e
        0x0012b082:    1f40        @.      SUBS     r0,r0,#5
        0x0012b084:    4798        .G      BLX      r3
        0x0012b086:    e019        ..      B        0x12b0bc ; uart_rx_payload_finish + 788
        0x0012b088:    4811        .H      LDR      r0,[pc,#68] ; [0x12b0d0] = 0x12b67c
        0x0012b08a:    7a00        .z      LDRB     r0,[r0,#8]
        0x0012b08c:    9001        ..      STR      r0,[sp,#4]
        0x0012b08e:    4810        .H      LDR      r0,[pc,#64] ; [0x12b0d0] = 0x12b67c
        0x0012b090:    6847        Gh      LDR      r7,[r0,#4]
        0x0012b092:    4638        8F      MOV      r0,r7
        0x0012b094:    9901        ..      LDR      r1,[sp,#4]
        0x0012b096:    f7fff99b    ....    BL       flash_erase ; 0x12a3d0
        0x0012b09a:    480e        .H      LDR      r0,[pc,#56] ; [0x12b0d4] = 0x12b25c
        0x0012b09c:    4a0e        .J      LDR      r2,[pc,#56] ; [0x12b0d8] = 0x12b1b1
        0x0012b09e:    2105        .!      MOVS     r1,#5
        0x0012b0a0:    6803        .h      LDR      r3,[r0,#0]
        0x0012b0a2:    480e        .H      LDR      r0,[pc,#56] ; [0x12b0dc] = 0x12b26e
        0x0012b0a4:    1f40        @.      SUBS     r0,r0,#5
        0x0012b0a6:    4798        .G      BLX      r3
        0x0012b0a8:    e008        ..      B        0x12b0bc ; uart_rx_payload_finish + 788
        0x0012b0aa:    e008        ..      B        0x12b0be ; uart_rx_payload_finish + 790
        0x0012b0ac:    e7ff        ..      B        0x12b0ae ; uart_rx_payload_finish + 774
        0x0012b0ae:    4809        .H      LDR      r0,[pc,#36] ; [0x12b0d4] = 0x12b25c
        0x0012b0b0:    4a09        .J      LDR      r2,[pc,#36] ; [0x12b0d8] = 0x12b1b1
        0x0012b0b2:    2105        .!      MOVS     r1,#5
        0x0012b0b4:    6803        .h      LDR      r3,[r0,#0]
        0x0012b0b6:    4809        .H      LDR      r0,[pc,#36] ; [0x12b0dc] = 0x12b26e
        0x0012b0b8:    4798        .G      BLX      r3
        0x0012b0ba:    bf00        ..      NOP      
        0x0012b0bc:    bf00        ..      NOP      
        0x0012b0be:    4805        .H      LDR      r0,[pc,#20] ; [0x12b0d4] = 0x12b25c
        0x0012b0c0:    4a0e        .J      LDR      r2,[pc,#56] ; [0x12b0fc] = 0x12ad89
        0x0012b0c2:    2108        .!      MOVS     r1,#8
        0x0012b0c4:    6843        Ch      LDR      r3,[r0,#4]
        0x0012b0c6:    4802        .H      LDR      r0,[pc,#8] ; [0x12b0d0] = 0x12b67c
        0x0012b0c8:    4798        .G      BLX      r3
        0x0012b0ca:    bf00        ..      NOP      
        0x0012b0cc:    e69e        ..      B        0x12ae0c ; uart_rx_payload_finish + 100
    $d
        0x0012b0ce:    0000        ..      DCW    0
        0x0012b0d0:    0012b67c    |...    DCD    1226364
        0x0012b0d4:    0012b25c    \...    DCD    1225308
        0x0012b0d8:    0012b1b1    ....    DCD    1225137
        0x0012b0dc:    0012b26e    n...    DCD    1225326
        0x0012b0e0:    0012b254    T...    DCD    1225300
        0x0012b0e4:    0012b255    U...    DCD    1225301
        0x0012b0e8:    0012b256    V...    DCD    1225302
        0x0012b0ec:    0012b27c    |...    DCD    1225340
        0x0012b0f0:    0012b273    s...    DCD    1225331
        0x0012b0f4:    0012b257    W...    DCD    1225303
        0x0012b0f8:    0012b274    t...    DCD    1225332
        0x0012b0fc:    0012ad89    ....    DCD    1224073
    $t
    i.uart_set_baudrate
    uart_set_baudrate
        0x0012b100:    b510        ..      PUSH     {r4,lr}
        0x0012b102:    4604        .F      MOV      r4,r0
        0x0012b104:    2001        .       MOVS     r0,#1
        0x0012b106:    f7fffca7    ....    BL       uart_dlab_setf ; 0x12aa58
        0x0012b10a:    b2e0        ..      UXTB     r0,r4
        0x0012b10c:    4904        .I      LDR      r1,[pc,#16] ; [0x12b120] = 0x20143000
        0x0012b10e:    6008        .`      STR      r0,[r1,#0]
        0x0012b110:    bf00        ..      NOP      
        0x0012b112:    1220         .      ASRS     r0,r4,#8
        0x0012b114:    6048        H`      STR      r0,[r1,#4]
        0x0012b116:    bf00        ..      NOP      
        0x0012b118:    2000        .       MOVS     r0,#0
        0x0012b11a:    f7fffc9d    ....    BL       uart_dlab_setf ; 0x12aa58
        0x0012b11e:    bd10        ..      POP      {r4,pc}
    $d
        0x0012b120:    20143000    .0.     DCD    538193920
    $t
    i.uart_sync_to_pc
    uart_sync_to_pc
        0x0012b124:    b510        ..      PUSH     {r4,lr}
        0x0012b126:    4803        .H      LDR      r0,[pc,#12] ; [0x12b134] = 0x12b25c
        0x0012b128:    4a03        .J      LDR      r2,[pc,#12] ; [0x12b138] = 0x12ad89
        0x0012b12a:    2108        .!      MOVS     r1,#8
        0x0012b12c:    6843        Ch      LDR      r3,[r0,#4]
        0x0012b12e:    4803        .H      LDR      r0,[pc,#12] ; [0x12b13c] = 0x12b67c
        0x0012b130:    4798        .G      BLX      r3
        0x0012b132:    bd10        ..      POP      {r4,pc}
    $d
        0x0012b134:    0012b25c    \...    DCD    1225308
        0x0012b138:    0012ad89    ....    DCD    1224073
        0x0012b13c:    0012b67c    |...    DCD    1226364
    $t
    i.uart_thr_empty_isr
    uart_thr_empty_isr
        0x0012b140:    b570        p.      PUSH     {r4-r6,lr}
        0x0012b142:    2400        .$      MOVS     r4,#0
        0x0012b144:    2500        .%      MOVS     r5,#0
        0x0012b146:    e024        $.      B        0x12b192 ; uart_thr_empty_isr + 82
        0x0012b148:    4917        .I      LDR      r1,[pc,#92] ; [0x12b1a8] = 0x12c684
        0x0012b14a:    6849        Ih      LDR      r1,[r1,#4]
        0x0012b14c:    7808        .x      LDRB     r0,[r1,#0]
        0x0012b14e:    4917        .I      LDR      r1,[pc,#92] ; [0x12b1ac] = 0x20143000
        0x0012b150:    6008        .`      STR      r0,[r1,#0]
        0x0012b152:    bf00        ..      NOP      
        0x0012b154:    4814        .H      LDR      r0,[pc,#80] ; [0x12b1a8] = 0x12c684
        0x0012b156:    6800        .h      LDR      r0,[r0,#0]
        0x0012b158:    1e40        @.      SUBS     r0,r0,#1
        0x0012b15a:    4913        .I      LDR      r1,[pc,#76] ; [0x12b1a8] = 0x12c684
        0x0012b15c:    6008        .`      STR      r0,[r1,#0]
        0x0012b15e:    4608        .F      MOV      r0,r1
        0x0012b160:    6840        @h      LDR      r0,[r0,#4]
        0x0012b162:    1c40        @.      ADDS     r0,r0,#1
        0x0012b164:    6048        H`      STR      r0,[r1,#4]
        0x0012b166:    4608        .F      MOV      r0,r1
        0x0012b168:    6800        .h      LDR      r0,[r0,#0]
        0x0012b16a:    2800        .(      CMP      r0,#0
        0x0012b16c:    d111        ..      BNE      0x12b192 ; uart_thr_empty_isr + 82
        0x0012b16e:    6048        H`      STR      r0,[r1,#4]
        0x0012b170:    f7fffcd6    ....    BL       uart_etbei_setf ; 0x12ab20
        0x0012b174:    480c        .H      LDR      r0,[pc,#48] ; [0x12b1a8] = 0x12c684
        0x0012b176:    6884        .h      LDR      r4,[r0,#8]
        0x0012b178:    68c5        .h      LDR      r5,[r0,#0xc]
        0x0012b17a:    2c00        .,      CMP      r4,#0
        0x0012b17c:    d007        ..      BEQ      0x12b18e ; uart_thr_empty_isr + 78
        0x0012b17e:    2000        .       MOVS     r0,#0
        0x0012b180:    4909        .I      LDR      r1,[pc,#36] ; [0x12b1a8] = 0x12c684
        0x0012b182:    6088        .`      STR      r0,[r1,#8]
        0x0012b184:    60c8        .`      STR      r0,[r1,#0xc]
        0x0012b186:    2100        .!      MOVS     r1,#0
        0x0012b188:    4628        (F      MOV      r0,r5
        0x0012b18a:    47a0        .G      BLX      r4
        0x0012b18c:    e000        ..      B        0x12b190 ; uart_thr_empty_isr + 80
        0x0012b18e:    bf00        ..      NOP      
        0x0012b190:    e007        ..      B        0x12b1a2 ; uart_thr_empty_isr + 98
        0x0012b192:    bf00        ..      NOP      
        0x0012b194:    4805        .H      LDR      r0,[pc,#20] ; [0x12b1ac] = 0x20143000
        0x0012b196:    3040        @0      ADDS     r0,r0,#0x40
        0x0012b198:    6bc0        .k      LDR      r0,[r0,#0x3c]
        0x0012b19a:    0780        ..      LSLS     r0,r0,#30
        0x0012b19c:    0fc0        ..      LSRS     r0,r0,#31
        0x0012b19e:    2800        .(      CMP      r0,#0
        0x0012b1a0:    d1d2        ..      BNE      0x12b148 ; uart_thr_empty_isr + 8
        0x0012b1a2:    bf00        ..      NOP      
        0x0012b1a4:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012b1a6:    0000        ..      DCW    0
        0x0012b1a8:    0012c684    ....    DCD    1230468
        0x0012b1ac:    20143000    .0.     DCD    538193920
    $t
    i.uart_tx_finish
    uart_tx_finish
        0x0012b1b0:    4770        pG      BX       lr
        0x0012b1b2:    0000        ..      MOVS     r0,r0
    i.uart_uninit
    uart_uninit
        0x0012b1b4:    b500        ..      PUSH     {lr}
        0x0012b1b6:    2005        .       MOVS     r0,#5
        0x0012b1b8:    0240        @.      LSLS     r0,r0,#9
        0x0012b1ba:    f7feffdd    ....    BL       clk_gate_per_g0 ; 0x12a178
        0x0012b1be:    200b        .       MOVS     r0,#0xb
        0x0012b1c0:    2101        .!      MOVS     r1,#1
        0x0012b1c2:    4081        .@      LSLS     r1,r1,r0
        0x0012b1c4:    4a01        .J      LDR      r2,[pc,#4] ; [0x12b1cc] = 0xe000e180
        0x0012b1c6:    6011        .`      STR      r1,[r2,#0]
        0x0012b1c8:    bf00        ..      NOP      
        0x0012b1ca:    bd00        ..      POP      {pc}
    $d
        0x0012b1cc:    e000e180    ....    DCD    3758154112
    $t
    i.uart_write
    uart_write
        0x0012b1d0:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012b1d2:    4604        .F      MOV      r4,r0
        0x0012b1d4:    460d        .F      MOV      r5,r1
        0x0012b1d6:    4616        .F      MOV      r6,r2
        0x0012b1d8:    461f        .F      MOV      r7,r3
        0x0012b1da:    4804        .H      LDR      r0,[pc,#16] ; [0x12b1ec] = 0x12c684
        0x0012b1dc:    6005        .`      STR      r5,[r0,#0]
        0x0012b1de:    6044        D`      STR      r4,[r0,#4]
        0x0012b1e0:    6086        .`      STR      r6,[r0,#8]
        0x0012b1e2:    60c7        .`      STR      r7,[r0,#0xc]
        0x0012b1e4:    2001        .       MOVS     r0,#1
        0x0012b1e6:    f7fffc9b    ....    BL       uart_etbei_setf ; 0x12ab20
        0x0012b1ea:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012b1ec:    0012c684    ....    DCD    1230468
    $t
    i.uart_write_register
    uart_write_register
        0x0012b1f0:    6001        .`      STR      r1,[r0,#0]
        0x0012b1f2:    4770        pG      BX       lr
    $d.realdata
    .constdata
        0x0012b1f4:    00000000    ....    DCD    0
        0x0012b1f8:    00000000    ....    DCD    0
        0x0012b1fc:    00000000    ....    DCD    0
        0x0012b200:    00009240    @...    DCD    37440
        0x0012b204:    20300060    `.0     DCD    540016736
        0x0012b208:    00000000    ....    DCD    0
        0x0012b20c:    00000000    ....    DCD    0
        0x0012b210:    0000a480    ....    DCD    42112
        0x0012b214:    203000ec    ..0     DCD    540016876
        0x0012b218:    00000000    ....    DCD    0
        0x0012b21c:    00000000    ....    DCD    0
        0x0012b220:    0000a482    ....    DCD    42114
        0x0012b224:    00000000    ....    DCD    0
        0x0012b228:    00000000    ....    DCD    0
        0x0012b22c:    00000080    ....    DCD    128
        0x0012b230:    024600eb    ..F.    DCD    38142187
        0x0012b234:    00000000    ....    DCD    0
        0x0012b238:    00000000    ....    DCD    0
        0x0012b23c:    00000080    ....    DCD    128
        0x0012b240:    010600bb    ....    DCD    17170619
    Region$$Table$$Base
        0x0012b244:    0012b27c    |...    DCD    1225340
        0x0012b248:    0012b27c    |...    DCD    1225340
        0x0012b24c:    0000342c    ,4..    DCD    13356
        0x0012b250:    0012a168    h...    DCD    1220968
    Region$$Table$$Limit

====================================

** Section #3

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x0012b254
    File Offset : 4760 (0x1298)
    Size        : 40 bytes (0x28)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    0x12b254:   00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
    0x12b264:   42 58 53 0d 0a 42 58 41 0d 0a 42 58 4e 0d 0a 00    BXS..BXA..BXN...
    0x12b274:   00 00 10 00 00 00 00 00                            ........


====================================

** Section #4

    Name        : EXEC_SECTION
    Type        : SHT_NOBITS (0x00000008)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x0012b27c
    File Offset : 4800 (0x12c0)
    Size        : 13356 bytes (0x342c)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 8
    Entry Size  : 0


====================================

** Section #5

    Name        : .debug_abbrev
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 4800 (0x12c0)
    Size        : 1476 bytes (0x5c4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #6

    Name        : .debug_frame
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 6276 (0x1884)
    Size        : 2012 bytes (0x7dc)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #7

    Name        : .debug_info
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 8288 (0x2060)
    Size        : 98028 bytes (0x17eec)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #8

    Name        : .debug_line
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 106316 (0x19f4c)
    Size        : 10596 bytes (0x2964)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #9

    Name        : .debug_loc
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 116912 (0x1c8b0)
    Size        : 5148 bytes (0x141c)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #10

    Name        : .debug_macinfo
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 122060 (0x1dccc)
    Size        : 14440 bytes (0x3868)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #11

    Name        : .debug_pubnames
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 136500 (0x21534)
    Size        : 1416 bytes (0x588)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #12

    Name        : .symtab
    Type        : SHT_SYMTAB (0x00000002)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 137916 (0x21abc)
    Size        : 5216 bytes (0x1460)
    Link        : Section 13 (.strtab)
    Info        : Last local symbol no = 235
    Alignment   : 4
    Entry Size  : 16


====================================

** Section #13

    Name        : .strtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 143132 (0x22f1c)
    Size        : 3956 bytes (0xf74)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #14

    Name        : .note
    Type        : SHT_NOTE (0x00000007)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 147088 (0x23e90)
    Size        : 32 bytes (0x20)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

        Section     Segment
    ====================================

              1           0
              2           0
              3           0
              4           0

====================================

** Section #15

    Name        : .comment
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 147120 (0x23eb0)
    Size        : 14408 bytes (0x3848)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0

    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    ArmLink --strict --library_type=microlib --callgraph --load_addr_map_info --map --symbols --xref --diag_suppress=9931 --cpu=Cortex-M0+ --list=.\Listings\boot_ram_download.map --output=.\Objects\boot_ram_download.axf --scatter=.\boot_ram_scatter.txt --info=summarysizes,sizes,totals,unused,veneers

    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\h_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\m_ps.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\mc_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\mf_p.l
    E:\installsoft\Keil\mdk525\ARM\ARMCC\Bin\..\lib\armlib\vfpsupport.l
    Input Comments:
    
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\boot_ram.o --vfemode=force

    Input Comments:
    
    pf4c-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\boot_ram.o --depend=.\objects\boot_ram.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I.\uart -I.\qspi -I.\flash -I.\dmac -I.\uart_awo -I.\clkg -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\boot_ram.crf boot_ram.c
    
    
    
    uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\uart_download.o --vfemode=force

    Input Comments:
    
    p2974-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\uart_download.o --depend=.\objects\uart_download.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\uart_download.crf uart_download.c
    
    
    
    startup_uart_download.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    ArmAsm --debug --xref --diag_suppress=9931 --cpu=Cortex-M0+ --apcs=interwork --depend=.\objects\startup_uart_download.d  -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:
    
    
    uart.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\uart.o --vfemode=force

    Input Comments:
    
    p984-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    uart.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\uart.o --depend=.\objects\uart.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\uart.crf uart\uart.c
    
    
    
    
    
    pshare_rom.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\pshare_rom.o --vfemode=force

    Input Comments:
    
    pb84-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    pshare_rom.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\pshare_rom.o --depend=.\objects\pshare_rom.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\pshare_rom.crf pshare_rom.c
    
    
    segger_rtt.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\segger_rtt.o --depend=.\objects\segger_rtt.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\segger_rtt.crf log\RTT\SEGGER_RTT.c
    
    segger_rtt_printf.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\segger_rtt_printf.o --depend=.\objects\segger_rtt_printf.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\segger_rtt_printf.crf log\RTT\SEGGER_RTT_printf.c
    
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\dma_for_qspi.o --vfemode=force

    Input Comments:
    
    p1fb8-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\dma_for_qspi.o --depend=.\objects\dma_for_qspi.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\dma_for_qspi.crf ..\boot_ram\dma_for_qspi.c
    
    
    
    
    
    flash.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\flash.o --depend=.\objects\flash.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\flash.crf ..\boot_ram\flash.c
    
    qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armlink [4d35ed]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=9931,9931,6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=.\objects\qspi.o --vfemode=force

    Input Comments:
    
    p1304-3
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: armasm [4d35ec]
    armasm --debug --diag_suppress=9931,9931,1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork/interwork --no_divide 
    qspi.o
    
    Component: ARM Compiler 5.06 update 6 (build 750) Tool: ArmCC [4d3637]
    ArmCC --c99 --split_sections --debug -c -o.\objects\qspi.o --depend=.\objects\qspi.d --cpu=Cortex-M0+ --apcs=interwork -O0 -Otime --diag_suppress=9931 -I..\..\bootloader -IC:\Keil_v5\ARM\Pack\ARM\CMSIS\4.5.0\CMSIS\Include -I.\clkg -I.\dmac -I.\flash -I.\qspi -I.\uart -I.\uart_awo -I.\user_file -I.\log -I.\log\RTT -I..\..\..\..\ip\ble\ll -I.\arch -I.\arch\boot -I.\arch\boot\armcc -I.\arch\boot\armgcc -I.\arch\compiler -I.\arch\compiler\armcc -I.\arch\compiler\armgcc -I.\arch\compiler\iar -I.\arch\ll -I.\arch\ll\armcc -I.\arch\ll\armgcc -I.\rwip\api -I.\sleep -I.\nvds\api -I.\reg\fw -I.\common\api -I..\boot_ram -I.\ -I..\..\..\..\ -I..\..\..\..\plf -I..\..\..\peripheral\flash_cache -I..\..\..\peripheral -I..\..\..\..\ip -I..\..\..\apollo_00 -I..\boot_ram_download -I.\RTE\_Boot_ram -IE:\installsoft\Keil\mdk525\ARM\PACK\ARM\CMSIS\5.3.0\Device\ARM\ARMCM0plus\Include -IE:\installsoft\Keil\mdk525\ARM\CMSIS\Include -D__MICROLIB -D__UVISION_VERSION=525 -DARMCM0P -DNDEBUG -DHW_BX_VERSION=2400 --omf_browse=.\objects\qspi.crf ..\boot_ram\qspi.c
    
    
    
    
    
    
    

====================================

** Section #16

    Name        : .shstrtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 161528 (0x276f8)
    Size        : 168 bytes (0xa8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

address     size       variable name                            type
0x0012c684  0x24       uart_env                                 uart_env_tag
0x0012c684  0x10       uart_env.tx                              uart_txrxchannel
0x0012c684  0x4        uart_env.tx.size                         uint32_t
0x0012c688  0x4        uart_env.tx.bufptr                       pointer to uint8_t
0x0012c68c  0x4        uart_env.tx.callback                     pointer to function 
0x0012c690  0x4        uart_env.tx.dummy                        pointer to unknown Type 
0x0012c694  0x10       uart_env.rx                              uart_txrxchannel
0x0012c694  0x4        uart_env.rx.size                         uint32_t
0x0012c698  0x4        uart_env.rx.bufptr                       pointer to uint8_t
0x0012c69c  0x4        uart_env.rx.callback                     pointer to function 
0x0012c6a0  0x4        uart_env.rx.dummy                        pointer to unknown Type 
0x0012c6a4  0x1        uart_env.errordetect                     _Bool

address     size       variable name                            type
0x0012b27c  0x400      apollo_00_reg_val                        array[256] of uint32_t

address     size       variable name                            type
0x0012b274  0x4        ram_run_current_location                 uint32_t

address     size       variable name                            type
0x0012b264  0xf        uart2pc                                  array[3] of array[5] of uint8_t

address     size       variable name                            type
0x0012b67c  0x1008     uart_buff                                Data_From_Host_t
0x0012b67c  0x8        uart_buff.header                         Uart_Ramrun_Header_t
0x0012b67c  0x1        uart_buff.header.header_flag             uint8_t
0x0012b67d  0x1        uart_buff.header.cmd                     anonymous
0x0012b67d  0x1(6:2)   uart_buff.header.cmd.r_w                 uint8_t
0x0012b67d  0x1(5:1)   uart_buff.header.cmd.section             uint8_t
0x0012b67d  0x1(0:5)   uart_buff.header.cmd.op                  uint8_t
0x0012b67e  0x2        uart_buff.header.length                  uint16_t
0x0012b680  0x4        uart_buff.header.addr                    uint32_t
0x0012b684  0x1000     uart_buff.data                           array[4096] of uint8_t

address     size       variable name                            type
0x0012b25c  0x8        uart_itf                                 uart_itf_t
0x0012b25c  0x4        uart_itf.tx_func                         write_fn_t
0x0012b260  0x4        uart_itf.rx_func                         read_fn_t

address     size       variable name                            type
0x0012b278  0x4        vec_int_base                             pointer to uint32_t

address     size       variable name                            type
0x0012b273  0x1        write_to_ram_run                         volatile uint8_t

address     size       variable name                            type
0x00129ff0  0x10       boot_header                              const boot_header_t
0x00129ff0  0x4        boot_header.bx_flag                      array[4] of uint8_t
0x00129ff4  0x4        boot_header.base_addr                    pointer to uint8_t
0x00129ff8  0x4        boot_header.length                       uint32_t
0x00129ffc  0x4        boot_header.entry_point                  entry_point_t

address     size       variable name                            type
0x0012b255  0x1        check_result                             uint8_t

address     size       variable name                            type
0x0012b258  0x1        fix_warning                              volatile uint8_t

address     size       variable name                            type
0x0012b257  0x1        start_ram_run                            volatile uint8_t

address     size       variable name                            type
0x0012b256  0x1        uart_baud_changed                        uint8_t

address     size       variable name                            type
0x0012b254  0x1        wait_download_finish                     uint8_t

