
========================================================================

** ELF Header Information

    File Name: boot_ram.elf

    Machine class: ELFCLASS32 (32-bit)
    Data encoding: ELFDATA2LSB (Little endian)
    Header version: EV_CURRENT (Current version)
    Operating System ABI: none
    ABI Version: 0
    File Type: ET_EXEC (Executable) (2)
    Machine: EM_ARM (ARM)

    Image Entry point: 0x0012e00d
    Flags: EF_ARM_HASENTRY + EF_ARM_ABI_FLOAT_SOFT (0x05000202)

    ARM ELF revision: 5 (ABI version 2)

    Conforms to Soft float procedure-call standard

    Built with
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]

    Header size: 52 bytes (0x34)
    Program header entry size: 32 bytes (0x20)
    Section header entry size: 40 bytes (0x28)

    Program header entries: 1
    Section header entries: 15

    Program header offset: 96820 (0x00017a34)
    Section header offset: 96852 (0x00017a54)

    Section header string table index: 14

========================================================================

** Program header #0

    Type          : PT_LOAD (1)
    File Offset   : 52 (0x34)
    Virtual Addr  : 0x0012dff0
    Physical Addr : 0x0012dff0
    Size in file  : 1692 bytes (0x69c)
    Size in memory: 1884 bytes (0x75c)
    Flags         : PF_X + PF_W + PF_R + PF_ARM_ENTRY (0x80000007)
    Alignment     : 4


========================================================================

** Section #1

    Name        : EXEC_SECTION
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : SHF_ALLOC + SHF_EXECINSTR (0x00000006)
    Addr        : 0x0012dff0
    File Offset : 52 (0x34)
    Size        : 1692 bytes (0x69c)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

    $d.realdata
    boot_header_area
    boot_ram_header
        0x0012dff0:    34325842    BX24    DCD    875714626
        0x0012dff4:    0012e000    ....    DCD    1236992
        0x0012dff8:    ffffffff    ....    DCD    4294967295
        0x0012dffc:    0012e00d    ....    DCD    1237005
        0x0012e000:    ffffffff    ....    DCD    4294967295
        0x0012e004:    ffffffff    ....    DCD    4294967295
        0x0012e008:    ffffffff    ....    DCD    4294967295
    $t
    i.boot_ram_entry
    boot_ram_entry
        0x0012e00c:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012e00e:    f000f8a1    ....    BL       dmac_init ; 0x12e154
        0x0012e012:    2101        .!      MOVS     r1,#1
        0x0012e014:    2002        .       MOVS     r0,#2
        0x0012e016:    f000f9e5    ....    BL       qspi_init ; 0x12e3e4
        0x0012e01a:    4d3c        <M      LDR      r5,[pc,#240] ; [0x12e10c] = 0x12e68c
        0x0012e01c:    462f        /F      MOV      r7,r5
        0x0012e01e:    3780        .7      ADDS     r7,r7,#0x80
        0x0012e020:    462e        .F      MOV      r6,r5
        0x0012e022:    3660        `6      ADDS     r6,r6,#0x60
        0x0012e024:    2401        .$      MOVS     r4,#1
        0x0012e026:    0324        $.      LSLS     r4,r4,#12
        0x0012e028:    4a38        8J      LDR      r2,[pc,#224] ; [0x12e10c] = 0x12e68c
        0x0012e02a:    2160        `!      MOVS     r1,#0x60
        0x0012e02c:    4620         F      MOV      r0,r4
        0x0012e02e:    f000f96b    ..k.    BL       flash_read ; 0x12e308
        0x0012e032:    4837        7H      LDR      r0,[pc,#220] ; [0x12e110] = 0x12dff0
        0x0012e034:    6980        .i      LDR      r0,[r0,#0x18]
        0x0012e036:    4632        2F      MOV      r2,r6
        0x0012e038:    1900        ..      ADDS     r0,r0,r4
        0x0012e03a:    2160        `!      MOVS     r1,#0x60
        0x0012e03c:    f000f964    ..d.    BL       flash_read ; 0x12e308
        0x0012e040:    6d28        (m      LDR      r0,[r5,#0x50]
        0x0012e042:    07c1        ..      LSLS     r1,r0,#31
        0x0012e044:    d103        ..      BNE      0x12e04e ; boot_ram_entry + 66
        0x0012e046:    0780        ..      LSLS     r0,r0,#30
        0x0012e048:    d501        ..      BPL      0x12e04e ; boot_ram_entry + 66
        0x0012e04a:    4c30        0L      LDR      r4,[pc,#192] ; [0x12e10c] = 0x12e68c
        0x0012e04c:    e007        ..      B        0x12e05e ; boot_ram_entry + 82
        0x0012e04e:    6b38        8k      LDR      r0,[r7,#0x30]
        0x0012e050:    07c1        ..      LSLS     r1,r0,#31
        0x0012e052:    d103        ..      BNE      0x12e05c ; boot_ram_entry + 80
        0x0012e054:    0780        ..      LSLS     r0,r0,#30
        0x0012e056:    d501        ..      BPL      0x12e05c ; boot_ram_entry + 80
        0x0012e058:    4634        4F      MOV      r4,r6
        0x0012e05a:    e000        ..      B        0x12e05e ; boot_ram_entry + 82
        0x0012e05c:    2400        .$      MOVS     r4,#0
        0x0012e05e:    2c00        .,      CMP      r4,#0
        0x0012e060:    d0e0        ..      BEQ      0x12e024 ; boot_ram_entry + 24
        0x0012e062:    42ac        .B      CMP      r4,r5
        0x0012e064:    d101        ..      BNE      0x12e06a ; boot_ram_entry + 94
        0x0012e066:    2500        .%      MOVS     r5,#0
        0x0012e068:    e001        ..      B        0x12e06e ; boot_ram_entry + 98
        0x0012e06a:    4829        )H      LDR      r0,[pc,#164] ; [0x12e110] = 0x12dff0
        0x0012e06c:    6985        .i      LDR      r5,[r0,#0x18]
        0x0012e06e:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e070:    6de0        .m      LDR      r0,[r4,#0x5c]
        0x0012e072:    1a40        @.      SUBS     r0,r0,r1
        0x0012e074:    2121        !!      MOVS     r1,#0x21
        0x0012e076:    1828        (.      ADDS     r0,r5,r0
        0x0012e078:    01c9        ..      LSLS     r1,r1,#7
        0x0012e07a:    1847        G.      ADDS     r7,r0,r1
        0x0012e07c:    6d20         m      LDR      r0,[r4,#0x50]
        0x0012e07e:    06c0        ..      LSLS     r0,r0,#27
        0x0012e080:    0f40        @.      LSRS     r0,r0,#29
        0x0012e082:    2802        .(      CMP      r0,#2
        0x0012e084:    d018        ..      BEQ      0x12e0b8 ; boot_ram_entry + 172
        0x0012e086:    2804        .(      CMP      r0,#4
        0x0012e088:    d016        ..      BEQ      0x12e0b8 ; boot_ram_entry + 172
        0x0012e08a:    2003        .       MOVS     r0,#3
        0x0012e08c:    f000fad2    ....    BL       sysc_cpu_qspi_en_setf ; 0x12e634
        0x0012e090:    6d20         m      LDR      r0,[r4,#0x50]
        0x0012e092:    4e20         N      LDR      r6,[pc,#128] ; [0x12e114] = 0x100000
        0x0012e094:    06c0        ..      LSLS     r0,r0,#27
        0x0012e096:    0f41        A.      LSRS     r1,r0,#29
        0x0012e098:    d018        ..      BEQ      0x12e0cc ; boot_ram_entry + 192
        0x0012e09a:    0f43        C.      LSRS     r3,r0,#29
        0x0012e09c:    4632        2F      MOV      r2,r6
        0x0012e09e:    4638        8F      MOV      r0,r7
        0x0012e0a0:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e0a2:    f000f8d5    ....    BL       flash_multi_read ; 0x12e250
        0x0012e0a6:    481c        .H      LDR      r0,[pc,#112] ; [0x12e118] = 0x20201080
        0x0012e0a8:    6b00        .k      LDR      r0,[r0,#0x30]
        0x0012e0aa:    06c0        ..      LSLS     r0,r0,#27
        0x0012e0ac:    0ec0        ..      LSRS     r0,r0,#27
        0x0012e0ae:    2814        .(      CMP      r0,#0x14
        0x0012e0b0:    d312        ..      BCC      0x12e0d8 ; boot_ram_entry + 204
        0x0012e0b2:    21ff        .!      MOVS     r1,#0xff
        0x0012e0b4:    318e        .1      ADDS     r1,r1,#0x8e
        0x0012e0b6:    e011        ..      B        0x12e0dc ; boot_ram_entry + 208
        0x0012e0b8:    200f        .       MOVS     r0,#0xf
        0x0012e0ba:    f000fabb    ....    BL       sysc_cpu_qspi_en_setf ; 0x12e634
        0x0012e0be:    f000f919    ....    BL       flash_quad_enable_check ; 0x12e2f4
        0x0012e0c2:    2800        .(      CMP      r0,#0
        0x0012e0c4:    d1e4        ..      BNE      0x12e090 ; boot_ram_entry + 132
        0x0012e0c6:    f000f90f    ....    BL       flash_quad_enable_01 ; 0x12e2e8
        0x0012e0ca:    e7e1        ..      B        0x12e090 ; boot_ram_entry + 132
        0x0012e0cc:    4632        2F      MOV      r2,r6
        0x0012e0ce:    4638        8F      MOV      r0,r7
        0x0012e0d0:    6da1        .m      LDR      r1,[r4,#0x58]
        0x0012e0d2:    f000f919    ....    BL       flash_read ; 0x12e308
        0x0012e0d6:    e7e6        ..      B        0x12e0a6 ; boot_ram_entry + 154
        0x0012e0d8:    21ff        .!      MOVS     r1,#0xff
        0x0012e0da:    31b2        .1      ADDS     r1,r1,#0xb2
        0x0012e0dc:    4620         F      MOV      r0,r4
        0x0012e0de:    f000fa7d    ..}.    BL       ram_code_crc_check ; 0x12e5dc
        0x0012e0e2:    2800        .(      CMP      r0,#0
        0x0012e0e4:    d103        ..      BNE      0x12e0ee ; boot_ram_entry + 226
        0x0012e0e6:    490c        .I      LDR      r1,[pc,#48] ; [0x12e118] = 0x20201080
        0x0012e0e8:    2002        .       MOVS     r0,#2
        0x0012e0ea:    3940        @9      SUBS     r1,r1,#0x40
        0x0012e0ec:    6008        .`      STR      r0,[r1,#0]
        0x0012e0ee:    6830        0h      LDR      r0,[r6,#0]
        0x0012e0f0:    f3808808    ....    MSR      MSP,r0
        0x0012e0f4:    4807        .H      LDR      r0,[pc,#28] ; [0x12e114] = 0x100000
        0x0012e0f6:    6d21        !m      LDR      r1,[r4,#0x50]
        0x0012e0f8:    1d00        ..      ADDS     r0,r0,#4
        0x0012e0fa:    06c9        ..      LSLS     r1,r1,#27
        0x0012e0fc:    0f4a        J.      LSRS     r2,r1,#29
        0x0012e0fe:    4904        .I      LDR      r1,[pc,#16] ; [0x12e110] = 0x12dff0
        0x0012e100:    6949        Ii      LDR      r1,[r1,#0x14]
        0x0012e102:    6804        .h      LDR      r4,[r0,#0]
        0x0012e104:    4b05        .K      LDR      r3,[pc,#20] ; [0x12e11c] = 0x3399
        0x0012e106:    4628        (F      MOV      r0,r5
        0x0012e108:    47a0        .G      BLX      r4
        0x0012e10a:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012e10c:    0012e68c    ....    DCD    1238668
        0x0012e110:    0012dff0    ....    DCD    1236976
        0x0012e114:    00100000    ....    DCD    1048576
        0x0012e118:    20201080    ..      DCD    538972288
        0x0012e11c:    00003399    .3..    DCD    13209
    $t
    i.dmac_block_ts_config
    dmac_block_ts_config
        0x0012e120:    b510        ..      PUSH     {r4,lr}
        0x0012e122:    4a0b        .J      LDR      r2,[pc,#44] ; [0x12e150] = 0x20120000
        0x0012e124:    2458        X$      MOVS     r4,#0x58
        0x0012e126:    4361        aC      MULS     r1,r4,r1
        0x0012e128:    4b08        .K      LDR      r3,[pc,#32] ; [0x12e14c] = 0xfff
        0x0012e12a:    1889        ..      ADDS     r1,r1,r2
        0x0012e12c:    69ca        .i      LDR      r2,[r1,#0x1c]
        0x0012e12e:    4298        .B      CMP      r0,r3
        0x0012e130:    d904        ..      BLS      0x12e13c ; dmac_block_ts_config + 28
        0x0012e132:    431a        .C      ORRS     r2,r2,r3
        0x0012e134:    61ca        .a      STR      r2,[r1,#0x1c]
        0x0012e136:    4259        YB      RSBS     r1,r3,#0
        0x0012e138:    1840        @.      ADDS     r0,r0,r1
        0x0012e13a:    bd10        ..      POP      {r4,pc}
        0x0012e13c:    0500        ..      LSLS     r0,r0,#20
        0x0012e13e:    0d00        ..      LSRS     r0,r0,#20
        0x0012e140:    0b12        ..      LSRS     r2,r2,#12
        0x0012e142:    0312        ..      LSLS     r2,r2,#12
        0x0012e144:    4310        .C      ORRS     r0,r0,r2
        0x0012e146:    61c8        .a      STR      r0,[r1,#0x1c]
        0x0012e148:    2000        .       MOVS     r0,#0
        0x0012e14a:    bd10        ..      POP      {r4,pc}
    $d
        0x0012e14c:    00000fff    ....    DCD    4095
        0x0012e150:    20120000    ...     DCD    538050560
    $t
    i.dmac_init
    dmac_init
        0x0012e154:    4903        .I      LDR      r1,[pc,#12] ; [0x12e164] = 0x20120380
        0x0012e156:    2001        .       MOVS     r0,#1
        0x0012e158:    6188        .a      STR      r0,[r1,#0x18]
        0x0012e15a:    4902        .I      LDR      r1,[pc,#8] ; [0x12e164] = 0x20120380
        0x0012e15c:    20ff        .       MOVS     r0,#0xff
        0x0012e15e:    3980        .9      SUBS     r1,r1,#0x80
        0x0012e160:    6388        .c      STR      r0,[r1,#0x38]
        0x0012e162:    4770        pG      BX       lr
    $d
        0x0012e164:    20120380    ...     DCD    538051456
    $t
    i.dmac_start
    dmac_start
        0x0012e168:    b5f8        ..      PUSH     {r3-r7,lr}
        0x0012e16a:    4604        .F      MOV      r4,r0
        0x0012e16c:    460f        .F      MOV      r7,r1
        0x0012e16e:    6880        .h      LDR      r0,[r0,#8]
        0x0012e170:    f7ffffd6    ....    BL       dmac_block_ts_config ; 0x12e120
        0x0012e174:    4602        .F      MOV      r2,r0
        0x0012e176:    68e0        .h      LDR      r0,[r4,#0xc]
        0x0012e178:    0441        A.      LSLS     r1,r0,#17
        0x0012e17a:    0f49        I.      LSRS     r1,r1,#29
        0x0012e17c:    d008        ..      BEQ      0x12e190 ; dmac_start + 40
        0x0012e17e:    2901        .)      CMP      r1,#1
        0x0012e180:    d003        ..      BEQ      0x12e18a ; dmac_start + 34
        0x0012e182:    2902        .)      CMP      r1,#2
        0x0012e184:    d106        ..      BNE      0x12e194 ; dmac_start + 44
        0x0012e186:    2602        .&      MOVS     r6,#2
        0x0012e188:    e003        ..      B        0x12e192 ; dmac_start + 42
        0x0012e18a:    2600        .&      MOVS     r6,#0
        0x0012e18c:    2502        .%      MOVS     r5,#2
        0x0012e18e:    e001        ..      B        0x12e194 ; dmac_start + 44
        0x0012e190:    2600        .&      MOVS     r6,#0
        0x0012e192:    2500        .%      MOVS     r5,#0
        0x0012e194:    0481        ..      LSLS     r1,r0,#18
        0x0012e196:    05c3        ..      LSLS     r3,r0,#23
        0x0012e198:    0f89        ..      LSRS     r1,r1,#30
        0x0012e19a:    0f5b        [.      LSRS     r3,r3,#29
        0x0012e19c:    0509        ..      LSLS     r1,r1,#20
        0x0012e19e:    039b        ..      LSLS     r3,r3,#14
        0x0012e1a0:    4319        .C      ORRS     r1,r1,r3
        0x0012e1a2:    0503        ..      LSLS     r3,r0,#20
        0x0012e1a4:    0f5b        [.      LSRS     r3,r3,#29
        0x0012e1a6:    02db        ..      LSLS     r3,r3,#11
        0x0012e1a8:    4319        .C      ORRS     r1,r1,r3
        0x0012e1aa:    07b3        ..      LSLS     r3,r6,#30
        0x0012e1ac:    0d5b        [.      LSRS     r3,r3,#21
        0x0012e1ae:    4319        .C      ORRS     r1,r1,r3
        0x0012e1b0:    07ab        ..      LSLS     r3,r5,#30
        0x0012e1b2:    0ddb        ..      LSRS     r3,r3,#23
        0x0012e1b4:    4319        .C      ORRS     r1,r1,r3
        0x0012e1b6:    0743        C.      LSLS     r3,r0,#29
        0x0012e1b8:    0e5b        [.      LSRS     r3,r3,#25
        0x0012e1ba:    0680        ..      LSLS     r0,r0,#26
        0x0012e1bc:    4319        .C      ORRS     r1,r1,r3
        0x0012e1be:    0f40        @.      LSRS     r0,r0,#29
        0x0012e1c0:    0040        @.      LSLS     r0,r0,#1
        0x0012e1c2:    4301        .C      ORRS     r1,r1,r0
        0x0012e1c4:    2058        X       MOVS     r0,#0x58
        0x0012e1c6:    4b0f        .K      LDR      r3,[pc,#60] ; [0x12e204] = 0x20120000
        0x0012e1c8:    4378        xC      MULS     r0,r7,r0
        0x0012e1ca:    18c0        ..      ADDS     r0,r0,r3
        0x0012e1cc:    6181        .a      STR      r1,[r0,#0x18]
        0x0012e1ce:    2100        .!      MOVS     r1,#0
        0x0012e1d0:    6401        .d      STR      r1,[r0,#0x40]
        0x0012e1d2:    68e1        .h      LDR      r1,[r4,#0xc]
        0x0012e1d4:    024b        K.      LSLS     r3,r1,#9
        0x0012e1d6:    0349        I.      LSLS     r1,r1,#13
        0x0012e1d8:    0f1b        ..      LSRS     r3,r3,#28
        0x0012e1da:    0f09        ..      LSRS     r1,r1,#28
        0x0012e1dc:    02db        ..      LSLS     r3,r3,#11
        0x0012e1de:    01c9        ..      LSLS     r1,r1,#7
        0x0012e1e0:    430b        .C      ORRS     r3,r3,r1
        0x0012e1e2:    1c9b        ..      ADDS     r3,r3,#2
        0x0012e1e4:    6443        Cd      STR      r3,[r0,#0x44]
        0x0012e1e6:    6821        !h      LDR      r1,[r4,#0]
        0x0012e1e8:    6001        .`      STR      r1,[r0,#0]
        0x0012e1ea:    6861        ah      LDR      r1,[r4,#4]
        0x0012e1ec:    6081        .`      STR      r1,[r0,#8]
        0x0012e1ee:    2001        .       MOVS     r0,#1
        0x0012e1f0:    40b8        .@      LSLS     r0,r0,r7
        0x0012e1f2:    0681        ..      LSLS     r1,r0,#26
        0x0012e1f4:    0e89        ..      LSRS     r1,r1,#26
        0x0012e1f6:    0680        ..      LSLS     r0,r0,#26
        0x0012e1f8:    0c80        ..      LSRS     r0,r0,#18
        0x0012e1fa:    4301        .C      ORRS     r1,r1,r0
        0x0012e1fc:    4802        .H      LDR      r0,[pc,#8] ; [0x12e208] = 0x20120380
        0x0012e1fe:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e200:    4610        .F      MOV      r0,r2
        0x0012e202:    bdf8        ..      POP      {r3-r7,pc}
    $d
        0x0012e204:    20120000    ...     DCD    538050560
        0x0012e208:    20120380    ...     DCD    538051456
    $t
    i.dmac_transfer_wait
    dmac_transfer_wait
        0x0012e20c:    b5f3        ..      PUSH     {r0,r1,r4-r7,lr}
        0x0012e20e:    b081        ..      SUB      sp,sp,#4
        0x0012e210:    9902        ..      LDR      r1,[sp,#8]
        0x0012e212:    4e0e        .N      LDR      r6,[pc,#56] ; [0x12e24c] = 0x201202c0
        0x0012e214:    2401        .$      MOVS     r4,#1
        0x0012e216:    408c        .@      LSLS     r4,r4,r1
        0x0012e218:    6831        1h      LDR      r1,[r6,#0]
        0x0012e21a:    4221        !B      TST      r1,r4
        0x0012e21c:    d0fc        ..      BEQ      0x12e218 ; dmac_transfer_wait + 12
        0x0012e21e:    4f0b        .O      LDR      r7,[pc,#44] ; [0x12e24c] = 0x201202c0
        0x0012e220:    3740        @7      ADDS     r7,r7,#0x40
        0x0012e222:    63bc        .c      STR      r4,[r7,#0x38]
        0x0012e224:    06a5        ..      LSLS     r5,r4,#26
        0x0012e226:    0ead        ..      LSRS     r5,r5,#26
        0x0012e228:    06a1        ..      LSLS     r1,r4,#26
        0x0012e22a:    0c89        ..      LSRS     r1,r1,#18
        0x0012e22c:    430d        .C      ORRS     r5,r5,r1
        0x0012e22e:    e009        ..      B        0x12e244 ; dmac_transfer_wait + 56
        0x0012e230:    9902        ..      LDR      r1,[sp,#8]
        0x0012e232:    f7ffff75    ..u.    BL       dmac_block_ts_config ; 0x12e120
        0x0012e236:    4905        .I      LDR      r1,[pc,#20] ; [0x12e24c] = 0x201202c0
        0x0012e238:    31c0        .1      ADDS     r1,r1,#0xc0
        0x0012e23a:    620d        .b      STR      r5,[r1,#0x20]
        0x0012e23c:    6831        1h      LDR      r1,[r6,#0]
        0x0012e23e:    4221        !B      TST      r1,r4
        0x0012e240:    d0fc        ..      BEQ      0x12e23c ; dmac_transfer_wait + 48
        0x0012e242:    63bc        .c      STR      r4,[r7,#0x38]
        0x0012e244:    2800        .(      CMP      r0,#0
        0x0012e246:    d1f3        ..      BNE      0x12e230 ; dmac_transfer_wait + 36
        0x0012e248:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x0012e24a:    0000        ..      DCW    0
        0x0012e24c:    201202c0    ...     DCD    538051264
    $t
    i.flash_multi_read
    flash_multi_read
        0x0012e250:    b57f        ..      PUSH     {r0-r6,lr}
        0x0012e252:    4c21        !L      LDR      r4,[pc,#132] ; [0x12e2d8] = 0x12e64c
        0x0012e254:    0200        ..      LSLS     r0,r0,#8
        0x0012e256:    68a5        .h      LDR      r5,[r4,#8]
        0x0012e258:    68e4        .h      LDR      r4,[r4,#0xc]
        0x0012e25a:    9101        ..      STR      r1,[sp,#4]
        0x0012e25c:    b2e9        ..      UXTB     r1,r5
        0x0012e25e:    4301        .C      ORRS     r1,r1,r0
        0x0012e260:    9200        ..      STR      r2,[sp,#0]
        0x0012e262:    220c        ."      MOVS     r2,#0xc
        0x0012e264:    9102        ..      STR      r1,[sp,#8]
        0x0012e266:    20f0        .       MOVS     r0,#0xf0
        0x0012e268:    0191        ..      LSLS     r1,r2,#6
        0x0012e26a:    9403        ..      STR      r4,[sp,#0xc]
        0x0012e26c:    2b01        .+      CMP      r3,#1
        0x0012e26e:    d006        ..      BEQ      0x12e27e ; flash_multi_read + 46
        0x0012e270:    2b02        .+      CMP      r3,#2
        0x0012e272:    d00a        ..      BEQ      0x12e28a ; flash_multi_read + 58
        0x0012e274:    2b03        .+      CMP      r3,#3
        0x0012e276:    d016        ..      BEQ      0x12e2a6 ; flash_multi_read + 86
        0x0012e278:    2b04        .+      CMP      r3,#4
        0x0012e27a:    d10e        ..      BNE      0x12e29a ; flash_multi_read + 74
        0x0012e27c:    e01e        ..      B        0x12e2bc ; flash_multi_read + 108
        0x0012e27e:    233b        ;#      MOVS     r3,#0x3b
        0x0012e280:    466c        lF      MOV      r4,sp
        0x0012e282:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e284:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e286:    4393        .C      BICS     r3,r3,r2
        0x0012e288:    e013        ..      B        0x12e2b2 ; flash_multi_read + 98
        0x0012e28a:    236b        k#      MOVS     r3,#0x6b
        0x0012e28c:    466c        lF      MOV      r4,sp
        0x0012e28e:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e290:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e292:    4393        .C      BICS     r3,r3,r2
        0x0012e294:    4383        .C      BICS     r3,r3,r0
        0x0012e296:    e019        ..      B        0x12e2cc ; flash_multi_read + 124
        0x0012e298:    81e3        ..      STRH     r3,[r4,#0xe]
        0x0012e29a:    4668        hF      MOV      r0,sp
        0x0012e29c:    f000f8ae    ....    BL       qspi_multi_read ; 0x12e3fc
        0x0012e2a0:    2000        .       MOVS     r0,#0
        0x0012e2a2:    b004        ..      ADD      sp,sp,#0x10
        0x0012e2a4:    bd70        p.      POP      {r4-r6,pc}
        0x0012e2a6:    23bb        .#      MOVS     r3,#0xbb
        0x0012e2a8:    466c        lF      MOV      r4,sp
        0x0012e2aa:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e2ac:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e2ae:    4393        .C      BICS     r3,r3,r2
        0x0012e2b0:    1d1b        ..      ADDS     r3,r3,#4
        0x0012e2b2:    4383        .C      BICS     r3,r3,r0
        0x0012e2b4:    438b        .C      BICS     r3,r3,r1
        0x0012e2b6:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2b8:    3301        .3      ADDS     r3,#1
        0x0012e2ba:    e7ed        ..      B        0x12e298 ; flash_multi_read + 72
        0x0012e2bc:    23eb        .#      MOVS     r3,#0xeb
        0x0012e2be:    466c        lF      MOV      r4,sp
        0x0012e2c0:    81a3        ..      STRH     r3,[r4,#0xc]
        0x0012e2c2:    89e3        ..      LDRH     r3,[r4,#0xe]
        0x0012e2c4:    4393        .C      BICS     r3,r3,r2
        0x0012e2c6:    1d1b        ..      ADDS     r3,r3,#4
        0x0012e2c8:    4383        .C      BICS     r3,r3,r0
        0x0012e2ca:    3340        @3      ADDS     r3,r3,#0x40
        0x0012e2cc:    438b        .C      BICS     r3,r3,r1
        0x0012e2ce:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2d0:    33ff        .3      ADDS     r3,r3,#0xff
        0x0012e2d2:    3302        .3      ADDS     r3,#2
        0x0012e2d4:    e7e0        ..      B        0x12e298 ; flash_multi_read + 72
    $d
        0x0012e2d6:    0000        ..      DCW    0
        0x0012e2d8:    0012e64c    L...    DCD    1238604
    $t
    i.flash_operation_wait
    flash_operation_wait
        0x0012e2dc:    b510        ..      PUSH     {r4,lr}
        0x0012e2de:    f000f84b    ..K.    BL       flash_read_status_reg_1 ; 0x12e378
        0x0012e2e2:    07c1        ..      LSLS     r1,r0,#31
        0x0012e2e4:    d1fb        ..      BNE      0x12e2de ; flash_operation_wait + 2
        0x0012e2e6:    bd10        ..      POP      {r4,pc}
    i.flash_quad_enable_01
    flash_quad_enable_01
        0x0012e2e8:    b510        ..      PUSH     {r4,lr}
        0x0012e2ea:    2102        .!      MOVS     r1,#2
        0x0012e2ec:    2000        .       MOVS     r0,#0
        0x0012e2ee:    f000f867    ..g.    BL       flash_write_status_reg_2bytes ; 0x12e3c0
        0x0012e2f2:    bd10        ..      POP      {r4,pc}
    i.flash_quad_enable_check
    flash_quad_enable_check
        0x0012e2f4:    b510        ..      PUSH     {r4,lr}
        0x0012e2f6:    f000f84c    ..L.    BL       flash_read_status_reg_2 ; 0x12e392
        0x0012e2fa:    0780        ..      LSLS     r0,r0,#30
        0x0012e2fc:    d501        ..      BPL      0x12e302 ; flash_quad_enable_check + 14
        0x0012e2fe:    2001        .       MOVS     r0,#1
        0x0012e300:    bd10        ..      POP      {r4,pc}
        0x0012e302:    2000        .       MOVS     r0,#0
        0x0012e304:    bd10        ..      POP      {r4,pc}
        0x0012e306:    0000        ..      MOVS     r0,r0
    i.flash_read
    flash_read
        0x0012e308:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012e30a:    4604        .F      MOV      r4,r0
        0x0012e30c:    0c08        ..      LSRS     r0,r1,#16
        0x0012e30e:    9002        ..      STR      r0,[sp,#8]
        0x0012e310:    b288        ..      UXTH     r0,r1
        0x0012e312:    2500        .%      MOVS     r5,#0
        0x0012e314:    2701        .'      MOVS     r7,#1
        0x0012e316:    4616        .F      MOV      r6,r2
        0x0012e318:    043f        ?.      LSLS     r7,r7,#16
        0x0012e31a:    9001        ..      STR      r0,[sp,#4]
        0x0012e31c:    e012        ..      B        0x12e344 ; flash_read + 60
        0x0012e31e:    a015        ..      ADR      r0,{pc}+0x56 ; 0x12e374
        0x0012e320:    6800        .h      LDR      r0,[r0,#0]
        0x0012e322:    9000        ..      STR      r0,[sp,#0]
        0x0012e324:    0c20         .      LSRS     r0,r4,#16
        0x0012e326:    4669        iF      MOV      r1,sp
        0x0012e328:    7048        Hp      STRB     r0,[r1,#1]
        0x0012e32a:    0a20         .      LSRS     r0,r4,#8
        0x0012e32c:    7088        .p      STRB     r0,[r1,#2]
        0x0012e32e:    70cc        .p      STRB     r4,[r1,#3]
        0x0012e330:    463b        ;F      MOV      r3,r7
        0x0012e332:    4632        2F      MOV      r2,r6
        0x0012e334:    2104        .!      MOVS     r1,#4
        0x0012e336:    4668        hF      MOV      r0,sp
        0x0012e338:    f000f90a    ....    BL       qspi_std_read ; 0x12e550
        0x0012e33c:    19e4        ..      ADDS     r4,r4,r7
        0x0012e33e:    19f6        ..      ADDS     r6,r6,r7
        0x0012e340:    1c6d        m.      ADDS     r5,r5,#1
        0x0012e342:    b2ad        ..      UXTH     r5,r5
        0x0012e344:    9802        ..      LDR      r0,[sp,#8]
        0x0012e346:    4285        .B      CMP      r5,r0
        0x0012e348:    d3e9        ..      BCC      0x12e31e ; flash_read + 22
        0x0012e34a:    9801        ..      LDR      r0,[sp,#4]
        0x0012e34c:    2800        .(      CMP      r0,#0
        0x0012e34e:    d00e        ..      BEQ      0x12e36e ; flash_read + 102
        0x0012e350:    a008        ..      ADR      r0,{pc}+0x24 ; 0x12e374
        0x0012e352:    6800        .h      LDR      r0,[r0,#0]
        0x0012e354:    9000        ..      STR      r0,[sp,#0]
        0x0012e356:    0c20         .      LSRS     r0,r4,#16
        0x0012e358:    4669        iF      MOV      r1,sp
        0x0012e35a:    7048        Hp      STRB     r0,[r1,#1]
        0x0012e35c:    0a20         .      LSRS     r0,r4,#8
        0x0012e35e:    7088        .p      STRB     r0,[r1,#2]
        0x0012e360:    70cc        .p      STRB     r4,[r1,#3]
        0x0012e362:    4632        2F      MOV      r2,r6
        0x0012e364:    2104        .!      MOVS     r1,#4
        0x0012e366:    4668        hF      MOV      r0,sp
        0x0012e368:    9b01        ..      LDR      r3,[sp,#4]
        0x0012e36a:    f000f8f1    ....    BL       qspi_std_read ; 0x12e550
        0x0012e36e:    2000        .       MOVS     r0,#0
        0x0012e370:    bdfe        ..      POP      {r1-r7,pc}
    $d
        0x0012e372:    0000        ..      DCW    0
        0x0012e374:    00000003    ....    DCD    3
    $t
    i.flash_read_status_reg_1
    flash_read_status_reg_1
        0x0012e378:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012e37a:    2005        .       MOVS     r0,#5
        0x0012e37c:    4669        iF      MOV      r1,sp
        0x0012e37e:    7108        .q      STRB     r0,[r1,#4]
        0x0012e380:    2301        .#      MOVS     r3,#1
        0x0012e382:    466a        jF      MOV      r2,sp
        0x0012e384:    4619        .F      MOV      r1,r3
        0x0012e386:    a801        ..      ADD      r0,sp,#4
        0x0012e388:    f000f8e2    ....    BL       qspi_std_read ; 0x12e550
        0x0012e38c:    4669        iF      MOV      r1,sp
        0x0012e38e:    7808        .x      LDRB     r0,[r1,#0]
        0x0012e390:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_read_status_reg_2
    flash_read_status_reg_2
        0x0012e392:    b51c        ..      PUSH     {r2-r4,lr}
        0x0012e394:    2035        5       MOVS     r0,#0x35
        0x0012e396:    4669        iF      MOV      r1,sp
        0x0012e398:    7108        .q      STRB     r0,[r1,#4]
        0x0012e39a:    2301        .#      MOVS     r3,#1
        0x0012e39c:    466a        jF      MOV      r2,sp
        0x0012e39e:    4619        .F      MOV      r1,r3
        0x0012e3a0:    a801        ..      ADD      r0,sp,#4
        0x0012e3a2:    f000f8d5    ....    BL       qspi_std_read ; 0x12e550
        0x0012e3a6:    4669        iF      MOV      r1,sp
        0x0012e3a8:    7808        .x      LDRB     r0,[r1,#0]
        0x0012e3aa:    bd1c        ..      POP      {r2-r4,pc}
    i.flash_write_enable
    flash_write_enable
        0x0012e3ac:    b508        ..      PUSH     {r3,lr}
        0x0012e3ae:    2006        .       MOVS     r0,#6
        0x0012e3b0:    4669        iF      MOV      r1,sp
        0x0012e3b2:    7008        .p      STRB     r0,[r1,#0]
        0x0012e3b4:    2101        .!      MOVS     r1,#1
        0x0012e3b6:    4668        hF      MOV      r0,sp
        0x0012e3b8:    f000f8fe    ....    BL       qspi_std_write ; 0x12e5b8
        0x0012e3bc:    bd08        ..      POP      {r3,pc}
        0x0012e3be:    0000        ..      MOVS     r0,r0
    i.flash_write_status_reg_2bytes
    flash_write_status_reg_2bytes
        0x0012e3c0:    b508        ..      PUSH     {r3,lr}
        0x0012e3c2:    a207        ..      ADR      r2,{pc}+0x1e ; 0x12e3e0
        0x0012e3c4:    6812        .h      LDR      r2,[r2,#0]
        0x0012e3c6:    9200        ..      STR      r2,[sp,#0]
        0x0012e3c8:    466a        jF      MOV      r2,sp
        0x0012e3ca:    7050        Pp      STRB     r0,[r2,#1]
        0x0012e3cc:    7091        .p      STRB     r1,[r2,#2]
        0x0012e3ce:    f7ffffed    ....    BL       flash_write_enable ; 0x12e3ac
        0x0012e3d2:    2103        .!      MOVS     r1,#3
        0x0012e3d4:    4668        hF      MOV      r0,sp
        0x0012e3d6:    f000f8ef    ....    BL       qspi_std_write ; 0x12e5b8
        0x0012e3da:    f7ffff7f    ....    BL       flash_operation_wait ; 0x12e2dc
        0x0012e3de:    bd08        ..      POP      {r3,pc}
    $d
        0x0012e3e0:    00000001    ....    DCD    1
    $t
    i.qspi_init
    qspi_init
        0x0012e3e4:    4a04        .J      LDR      r2,[pc,#16] ; [0x12e3f8] = 0x20300000
        0x0012e3e6:    2300        .#      MOVS     r3,#0
        0x0012e3e8:    6093        .`      STR      r3,[r2,#8]
        0x0012e3ea:    6150        Pa      STR      r0,[r2,#0x14]
        0x0012e3ec:    4802        .H      LDR      r0,[pc,#8] ; [0x12e3f8] = 0x20300000
        0x0012e3ee:    30c0        .0      ADDS     r0,r0,#0xc0
        0x0012e3f0:    6301        .c      STR      r1,[r0,#0x30]
        0x0012e3f2:    6113        .a      STR      r3,[r2,#0x10]
        0x0012e3f4:    4770        pG      BX       lr
    $d
        0x0012e3f6:    0000        ..      DCW    0
        0x0012e3f8:    20300000    ..0     DCD    540016640
    $t
    i.qspi_multi_read
    qspi_multi_read
        0x0012e3fc:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012e3fe:    4604        .F      MOV      r4,r0
        0x0012e400:    6840        @h      LDR      r0,[r0,#4]
        0x0012e402:    b085        ..      SUB      sp,sp,#0x14
        0x0012e404:    0885        ..      LSRS     r5,r0,#2
        0x0012e406:    0786        ..      LSLS     r6,r0,#30
        0x0012e408:    7ba0        .{      LDRB     r0,[r4,#0xe]
        0x0012e40a:    0fb6        ..      LSRS     r6,r6,#30
        0x0012e40c:    0601        ..      LSLS     r1,r0,#24
        0x0012e40e:    0782        ..      LSLS     r2,r0,#30
        0x0012e410:    0f09        ..      LSRS     r1,r1,#28
        0x0012e412:    02c9        ..      LSLS     r1,r1,#11
        0x0012e414:    0d92        ..      LSRS     r2,r2,#22
        0x0012e416:    0700        ..      LSLS     r0,r0,#28
        0x0012e418:    4311        .C      ORRS     r1,r1,r2
        0x0012e41a:    0f80        ..      LSRS     r0,r0,#30
        0x0012e41c:    4301        .C      ORRS     r1,r1,r0
        0x0012e41e:    2020                MOVS     r0,#0x20
        0x0012e420:    4301        .C      ORRS     r1,r1,r0
        0x0012e422:    4831        1H      LDR      r0,[pc,#196] ; [0x12e4e8] = 0x203000c0
        0x0012e424:    6341        Ac      STR      r1,[r0,#0x34]
        0x0012e426:    4f30        0O      LDR      r7,[pc,#192] ; [0x12e4e8] = 0x203000c0
        0x0012e428:    3fc0        .?      SUBS     r7,r7,#0xc0
        0x0012e42a:    2d00        .-      CMP      r5,#0
        0x0012e42c:    d029        ).      BEQ      0x12e482 ; qspi_multi_read + 134
        0x0012e42e:    89e0        ..      LDRH     r0,[r4,#0xe]
        0x0012e430:    492e        .I      LDR      r1,[pc,#184] ; [0x12e4ec] = 0x1f0200
        0x0012e432:    0580        ..      LSLS     r0,r0,#22
        0x0012e434:    0f80        ..      LSRS     r0,r0,#30
        0x0012e436:    0540        @.      LSLS     r0,r0,#21
        0x0012e438:    1840        @.      ADDS     r0,r0,r1
        0x0012e43a:    6038        8`      STR      r0,[r7,#0]
        0x0012e43c:    1e68        h.      SUBS     r0,r5,#1
        0x0012e43e:    6078        x`      STR      r0,[r7,#4]
        0x0012e440:    4829        )H      LDR      r0,[pc,#164] ; [0x12e4e8] = 0x203000c0
        0x0012e442:    2107        .!      MOVS     r1,#7
        0x0012e444:    3880        .8      SUBS     r0,r0,#0x80
        0x0012e446:    6141        Aa      STR      r1,[r0,#0x14]
        0x0012e448:    2100        .!      MOVS     r1,#0
        0x0012e44a:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e44c:    2101        .!      MOVS     r1,#1
        0x0012e44e:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e450:    60b9        .`      STR      r1,[r7,#8]
        0x0012e452:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012e454:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e456:    68a1        .h      LDR      r1,[r4,#8]
        0x0012e458:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e45a:    4825        %H      LDR      r0,[pc,#148] ; [0x12e4f0] = 0x12e67c
        0x0012e45c:    c80f        ..      LDM      r0,{r0-r3}
        0x0012e45e:    9000        ..      STR      r0,[sp,#0]
        0x0012e460:    a801        ..      ADD      r0,sp,#4
        0x0012e462:    c00e        ..      STM      r0!,{r1-r3}
        0x0012e464:    6820         h      LDR      r0,[r4,#0]
        0x0012e466:    9001        ..      STR      r0,[sp,#4]
        0x0012e468:    2100        .!      MOVS     r1,#0
        0x0012e46a:    9502        ..      STR      r5,[sp,#8]
        0x0012e46c:    4668        hF      MOV      r0,sp
        0x0012e46e:    f7fffe7b    ..{.    BL       dmac_start ; 0x12e168
        0x0012e472:    2101        .!      MOVS     r1,#1
        0x0012e474:    6139        9a      STR      r1,[r7,#0x10]
        0x0012e476:    2100        .!      MOVS     r1,#0
        0x0012e478:    f7fffec8    ....    BL       dmac_transfer_wait ; 0x12e20c
        0x0012e47c:    2000        .       MOVS     r0,#0
        0x0012e47e:    60b8        .`      STR      r0,[r7,#8]
        0x0012e480:    6138        8a      STR      r0,[r7,#0x10]
        0x0012e482:    2e00        ..      CMP      r6,#0
        0x0012e484:    d02d        -.      BEQ      0x12e4e2 ; qspi_multi_read + 230
        0x0012e486:    89e0        ..      LDRH     r0,[r4,#0xe]
        0x0012e488:    491a        .I      LDR      r1,[pc,#104] ; [0x12e4f4] = 0x70200
        0x0012e48a:    0580        ..      LSLS     r0,r0,#22
        0x0012e48c:    0f80        ..      LSRS     r0,r0,#30
        0x0012e48e:    0540        @.      LSLS     r0,r0,#21
        0x0012e490:    1840        @.      ADDS     r0,r0,r1
        0x0012e492:    6038        8`      STR      r0,[r7,#0]
        0x0012e494:    1e70        p.      SUBS     r0,r6,#1
        0x0012e496:    6078        x`      STR      r0,[r7,#4]
        0x0012e498:    4813        .H      LDR      r0,[pc,#76] ; [0x12e4e8] = 0x203000c0
        0x0012e49a:    2100        .!      MOVS     r1,#0
        0x0012e49c:    3880        .8      SUBS     r0,r0,#0x80
        0x0012e49e:    60c1        .`      STR      r1,[r0,#0xc]
        0x0012e4a0:    2101        .!      MOVS     r1,#1
        0x0012e4a2:    60b9        .`      STR      r1,[r7,#8]
        0x0012e4a4:    89a1        ..      LDRH     r1,[r4,#0xc]
        0x0012e4a6:    6201        .b      STR      r1,[r0,#0x20]
        0x0012e4a8:    68a1        .h      LDR      r1,[r4,#8]
        0x0012e4aa:    00ab        ..      LSLS     r3,r5,#2
        0x0012e4ac:    0a0a        ..      LSRS     r2,r1,#8
        0x0012e4ae:    18d2        ..      ADDS     r2,r2,r3
        0x0012e4b0:    0212        ..      LSLS     r2,r2,#8
        0x0012e4b2:    b2c9        ..      UXTB     r1,r1
        0x0012e4b4:    430a        .C      ORRS     r2,r2,r1
        0x0012e4b6:    6202        .b      STR      r2,[r0,#0x20]
        0x0012e4b8:    2101        .!      MOVS     r1,#1
        0x0012e4ba:    6139        9a      STR      r1,[r7,#0x10]
        0x0012e4bc:    6ab9        .j      LDR      r1,[r7,#0x28]
        0x0012e4be:    0749        I.      LSLS     r1,r1,#29
        0x0012e4c0:    d5fc        ..      BPL      0x12e4bc ; qspi_multi_read + 192
        0x0012e4c2:    6ab9        .j      LDR      r1,[r7,#0x28]
        0x0012e4c4:    07c9        ..      LSLS     r1,r1,#31
        0x0012e4c6:    d1fc        ..      BNE      0x12e4c2 ; qspi_multi_read + 198
        0x0012e4c8:    6821        !h      LDR      r1,[r4,#0]
        0x0012e4ca:    18cb        ..      ADDS     r3,r1,r3
        0x0012e4cc:    2100        .!      MOVS     r1,#0
        0x0012e4ce:    e003        ..      B        0x12e4d8 ; qspi_multi_read + 220
        0x0012e4d0:    6a02        .j      LDR      r2,[r0,#0x20]
        0x0012e4d2:    545a        ZT      STRB     r2,[r3,r1]
        0x0012e4d4:    1c49        I.      ADDS     r1,r1,#1
        0x0012e4d6:    b2c9        ..      UXTB     r1,r1
        0x0012e4d8:    42b1        .B      CMP      r1,r6
        0x0012e4da:    d3f9        ..      BCC      0x12e4d0 ; qspi_multi_read + 212
        0x0012e4dc:    2000        .       MOVS     r0,#0
        0x0012e4de:    60b8        .`      STR      r0,[r7,#8]
        0x0012e4e0:    6138        8a      STR      r0,[r7,#0x10]
        0x0012e4e2:    b005        ..      ADD      sp,sp,#0x14
        0x0012e4e4:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012e4e6:    0000        ..      DCW    0
        0x0012e4e8:    203000c0    ..0     DCD    540016832
        0x0012e4ec:    001f0200    ....    DCD    2032128
        0x0012e4f0:    0012e67c    |...    DCD    1238652
        0x0012e4f4:    00070200    ....    DCD    459264
    $t
    i.qspi_std_byte_write_dma_config
    qspi_std_byte_write_dma_config
        0x0012e4f8:    4a05        .J      LDR      r2,[pc,#20] ; [0x12e510] = 0x70100
        0x0012e4fa:    6002        .`      STR      r2,[r0,#0]
        0x0012e4fc:    1f09        ..      SUBS     r1,r1,#4
        0x0012e4fe:    6501        .e      STR      r1,[r0,#0x50]
        0x0012e500:    2100        .!      MOVS     r1,#0
        0x0012e502:    64c1        .d      STR      r1,[r0,#0x4c]
        0x0012e504:    2102        .!      MOVS     r1,#2
        0x0012e506:    64c1        .d      STR      r1,[r0,#0x4c]
        0x0012e508:    2101        .!      MOVS     r1,#1
        0x0012e50a:    6081        .`      STR      r1,[r0,#8]
        0x0012e50c:    4770        pG      BX       lr
    $d
        0x0012e50e:    0000        ..      DCW    0
        0x0012e510:    00070100    ....    DCD    459008
    $t
    i.qspi_std_byte_write_dma_start
    qspi_std_byte_write_dma_start
        0x0012e514:    b57f        ..      PUSH     {r0-r6,lr}
        0x0012e516:    4604        .F      MOV      r4,r0
        0x0012e518:    480c        .H      LDR      r0,[pc,#48] ; [0x12e54c] = 0x12e65c
        0x0012e51a:    460d        .F      MOV      r5,r1
        0x0012e51c:    68c0        .h      LDR      r0,[r0,#0xc]
        0x0012e51e:    9003        ..      STR      r0,[sp,#0xc]
        0x0012e520:    4620         F      MOV      r0,r4
        0x0012e522:    3060        `0      ADDS     r0,r0,#0x60
        0x0012e524:    9001        ..      STR      r0,[sp,#4]
        0x0012e526:    9200        ..      STR      r2,[sp,#0]
        0x0012e528:    2100        .!      MOVS     r1,#0
        0x0012e52a:    9302        ..      STR      r3,[sp,#8]
        0x0012e52c:    4668        hF      MOV      r0,sp
        0x0012e52e:    f7fffe1b    ....    BL       dmac_start ; 0x12e168
        0x0012e532:    6125        %a      STR      r5,[r4,#0x10]
        0x0012e534:    2100        .!      MOVS     r1,#0
        0x0012e536:    f7fffe69    ..i.    BL       dmac_transfer_wait ; 0x12e20c
        0x0012e53a:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012e53c:    0740        @.      LSLS     r0,r0,#29
        0x0012e53e:    d5fc        ..      BPL      0x12e53a ; qspi_std_byte_write_dma_start + 38
        0x0012e540:    6aa0        .j      LDR      r0,[r4,#0x28]
        0x0012e542:    07c0        ..      LSLS     r0,r0,#31
        0x0012e544:    d1fc        ..      BNE      0x12e540 ; qspi_std_byte_write_dma_start + 44
        0x0012e546:    60a0        .`      STR      r0,[r4,#8]
        0x0012e548:    6120         a      STR      r0,[r4,#0x10]
        0x0012e54a:    bd7f        ..      POP      {r0-r6,pc}
    $d
        0x0012e54c:    0012e65c    \...    DCD    1238620
    $t
    i.qspi_std_read
    qspi_std_read
        0x0012e550:    b5f0        ..      PUSH     {r4-r7,lr}
        0x0012e552:    4694        .F      MOV      r12,r2
        0x0012e554:    4c16        .L      LDR      r4,[pc,#88] ; [0x12e5b0] = 0x20300000
        0x0012e556:    b085        ..      SUB      sp,sp,#0x14
        0x0012e558:    4a14        .J      LDR      r2,[pc,#80] ; [0x12e5ac] = 0x70300
        0x0012e55a:    6022        "`      STR      r2,[r4,#0]
        0x0012e55c:    1e5a        Z.      SUBS     r2,r3,#1
        0x0012e55e:    6062        b`      STR      r2,[r4,#4]
        0x0012e560:    4d13        .M      LDR      r5,[pc,#76] ; [0x12e5b0] = 0x20300000
        0x0012e562:    2607        .&      MOVS     r6,#7
        0x0012e564:    3540        @5      ADDS     r5,r5,#0x40
        0x0012e566:    616e        na      STR      r6,[r5,#0x14]
        0x0012e568:    2600        .&      MOVS     r6,#0
        0x0012e56a:    60ee        .`      STR      r6,[r5,#0xc]
        0x0012e56c:    2601        .&      MOVS     r6,#1
        0x0012e56e:    60ee        .`      STR      r6,[r5,#0xc]
        0x0012e570:    60a6        .`      STR      r6,[r4,#8]
        0x0012e572:    2700        .'      MOVS     r7,#0
        0x0012e574:    e003        ..      B        0x12e57e ; qspi_std_read + 46
        0x0012e576:    5dc2        .]      LDRB     r2,[r0,r7]
        0x0012e578:    622a        *b      STR      r2,[r5,#0x20]
        0x0012e57a:    1c7f        ..      ADDS     r7,r7,#1
        0x0012e57c:    b2ff        ..      UXTB     r7,r7
        0x0012e57e:    428f        .B      CMP      r7,r1
        0x0012e580:    d3f9        ..      BCC      0x12e576 ; qspi_std_read + 38
        0x0012e582:    480c        .H      LDR      r0,[pc,#48] ; [0x12e5b4] = 0x12e65c
        0x0012e584:    6901        .i      LDR      r1,[r0,#0x10]
        0x0012e586:    69c0        .i      LDR      r0,[r0,#0x1c]
        0x0012e588:    9003        ..      STR      r0,[sp,#0xc]
        0x0012e58a:    4660        `F      MOV      r0,r12
        0x0012e58c:    9100        ..      STR      r1,[sp,#0]
        0x0012e58e:    9001        ..      STR      r0,[sp,#4]
        0x0012e590:    2100        .!      MOVS     r1,#0
        0x0012e592:    9302        ..      STR      r3,[sp,#8]
        0x0012e594:    4668        hF      MOV      r0,sp
        0x0012e596:    f7fffde7    ....    BL       dmac_start ; 0x12e168
        0x0012e59a:    6126        &a      STR      r6,[r4,#0x10]
        0x0012e59c:    2100        .!      MOVS     r1,#0
        0x0012e59e:    f7fffe35    ..5.    BL       dmac_transfer_wait ; 0x12e20c
        0x0012e5a2:    2000        .       MOVS     r0,#0
        0x0012e5a4:    60a0        .`      STR      r0,[r4,#8]
        0x0012e5a6:    6120         a      STR      r0,[r4,#0x10]
        0x0012e5a8:    b005        ..      ADD      sp,sp,#0x14
        0x0012e5aa:    bdf0        ..      POP      {r4-r7,pc}
    $d
        0x0012e5ac:    00070300    ....    DCD    459520
        0x0012e5b0:    20300000    ..0     DCD    540016640
        0x0012e5b4:    0012e65c    \...    DCD    1238620
    $t
    i.qspi_std_write
    qspi_std_write
        0x0012e5b8:    b570        p.      PUSH     {r4-r6,lr}
        0x0012e5ba:    4c07        .L      LDR      r4,[pc,#28] ; [0x12e5d8] = 0x20300000
        0x0012e5bc:    460d        .F      MOV      r5,r1
        0x0012e5be:    4606        .F      MOV      r6,r0
        0x0012e5c0:    2140        @!      MOVS     r1,#0x40
        0x0012e5c2:    4620         F      MOV      r0,r4
        0x0012e5c4:    f7ffff98    ....    BL       qspi_std_byte_write_dma_config ; 0x12e4f8
        0x0012e5c8:    462b        +F      MOV      r3,r5
        0x0012e5ca:    4632        2F      MOV      r2,r6
        0x0012e5cc:    2101        .!      MOVS     r1,#1
        0x0012e5ce:    4620         F      MOV      r0,r4
        0x0012e5d0:    f7ffffa0    ....    BL       qspi_std_byte_write_dma_start ; 0x12e514
        0x0012e5d4:    bd70        p.      POP      {r4-r6,pc}
    $d
        0x0012e5d6:    0000        ..      DCW    0
        0x0012e5d8:    20300000    ..0     DCD    540016640
    $t
    i.ram_code_crc_check
    ram_code_crc_check
        0x0012e5dc:    b5fe        ..      PUSH     {r1-r7,lr}
        0x0012e5de:    4606        .F      MOV      r6,r0
        0x0012e5e0:    6d80        .m      LDR      r0,[r0,#0x58]
        0x0012e5e2:    460f        .F      MOV      r7,r1
        0x0012e5e4:    0101        ..      LSLS     r1,r0,#4
        0x0012e5e6:    0500        ..      LSLS     r0,r0,#20
        0x0012e5e8:    0c09        ..      LSRS     r1,r1,#16
        0x0012e5ea:    0d00        ..      LSRS     r0,r0,#20
        0x0012e5ec:    2501        .%      MOVS     r5,#1
        0x0012e5ee:    052d        -.      LSLS     r5,r5,#20
        0x0012e5f0:    2400        .$      MOVS     r4,#0
        0x0012e5f2:    9101        ..      STR      r1,[sp,#4]
        0x0012e5f4:    9000        ..      STR      r0,[sp,#0]
        0x0012e5f6:    e00d        ..      B        0x12e614 ; ram_code_crc_check + 56
        0x0012e5f8:    2201        ."      MOVS     r2,#1
        0x0012e5fa:    0312        ..      LSLS     r2,r2,#12
        0x0012e5fc:    4629        )F      MOV      r1,r5
        0x0012e5fe:    2000        .       MOVS     r0,#0
        0x0012e600:    47b8        .G      BLX      r7
        0x0012e602:    0061        a.      LSLS     r1,r4,#1
        0x0012e604:    5a71        qZ      LDRH     r1,[r6,r1]
        0x0012e606:    4281        .B      CMP      r1,r0
        0x0012e608:    d10f        ..      BNE      0x12e62a ; ram_code_crc_check + 78
        0x0012e60a:    2001        .       MOVS     r0,#1
        0x0012e60c:    0300        ..      LSLS     r0,r0,#12
        0x0012e60e:    182d        -.      ADDS     r5,r5,r0
        0x0012e610:    1c64        d.      ADDS     r4,r4,#1
        0x0012e612:    b2a4        ..      UXTH     r4,r4
        0x0012e614:    9801        ..      LDR      r0,[sp,#4]
        0x0012e616:    4284        .B      CMP      r4,r0
        0x0012e618:    d3ee        ..      BCC      0x12e5f8 ; ram_code_crc_check + 28
        0x0012e61a:    4629        )F      MOV      r1,r5
        0x0012e61c:    2000        .       MOVS     r0,#0
        0x0012e61e:    9a00        ..      LDR      r2,[sp,#0]
        0x0012e620:    47b8        .G      BLX      r7
        0x0012e622:    0061        a.      LSLS     r1,r4,#1
        0x0012e624:    5a71        qZ      LDRH     r1,[r6,r1]
        0x0012e626:    4281        .B      CMP      r1,r0
        0x0012e628:    d001        ..      BEQ      0x12e62e ; ram_code_crc_check + 82
        0x0012e62a:    2000        .       MOVS     r0,#0
        0x0012e62c:    bdfe        ..      POP      {r1-r7,pc}
        0x0012e62e:    2001        .       MOVS     r0,#1
        0x0012e630:    bdfe        ..      POP      {r1-r7,pc}
        0x0012e632:    0000        ..      MOVS     r0,r0
    i.sysc_cpu_qspi_en_setf
    sysc_cpu_qspi_en_setf
        0x0012e634:    4904        .I      LDR      r1,[pc,#16] ; [0x12e648] = 0x20132000
        0x0012e636:    6a0a        .j      LDR      r2,[r1,#0x20]
        0x0012e638:    0700        ..      LSLS     r0,r0,#28
        0x0012e63a:    0912        ..      LSRS     r2,r2,#4
        0x0012e63c:    0112        ..      LSLS     r2,r2,#4
        0x0012e63e:    0f00        ..      LSRS     r0,r0,#28
        0x0012e640:    4302        .C      ORRS     r2,r2,r0
        0x0012e642:    620a        .b      STR      r2,[r1,#0x20]
        0x0012e644:    4770        pG      BX       lr
    $d
        0x0012e646:    0000        ..      DCW    0
        0x0012e648:    20132000    . .     DCD    538124288
    $d.realdata
    .constdata
        0x0012e64c:    00000000    ....    DCD    0
        0x0012e650:    00000000    ....    DCD    0
        0x0012e654:    00000000    ....    DCD    0
        0x0012e658:    00020000    ....    DCD    131072
    .constdata
        0x0012e65c:    00000000    ....    DCD    0
        0x0012e660:    00000000    ....    DCD    0
        0x0012e664:    00000000    ....    DCD    0
        0x0012e668:    00009240    @...    DCD    37440
        0x0012e66c:    20300060    `.0     DCD    540016736
        0x0012e670:    00000000    ....    DCD    0
        0x0012e674:    00000000    ....    DCD    0
        0x0012e678:    0000a480    ....    DCD    42112
        0x0012e67c:    203000ec    ..0     DCD    540016876
        0x0012e680:    00000000    ....    DCD    0
        0x0012e684:    00000000    ....    DCD    0
        0x0012e688:    0000a482    ....    DCD    42114

====================================

** Section #2

    Name        : EXEC_SECTION
    Type        : SHT_NOBITS (0x00000008)
    Flags       : SHF_ALLOC + SHF_WRITE (0x00000003)
    Addr        : 0x0012e68c
    File Offset : 1744 (0x6d0)
    Size        : 192 bytes (0xc0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0


====================================

** Section #3

    Name        : .debug_abbrev
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 1744 (0x6d0)
    Size        : 1444 bytes (0x5a4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #4

    Name        : .debug_frame
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 3188 (0xc74)
    Size        : 712 bytes (0x2c8)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #5

    Name        : .debug_info
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 3900 (0xf3c)
    Size        : 70912 bytes (0x11500)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #6

    Name        : .debug_line
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 74812 (0x1243c)
    Size        : 7224 bytes (0x1c38)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #7

    Name        : .debug_loc
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 82036 (0x14074)
    Size        : 2016 bytes (0x7e0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #8

    Name        : .debug_macinfo
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 84052 (0x14854)
    Size        : 4032 bytes (0xfc0)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #9

    Name        : .debug_pubnames
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 88084 (0x15814)
    Size        : 676 bytes (0x2a4)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #10

    Name        : .symtab
    Type        : SHT_SYMTAB (0x00000002)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 88760 (0x15ab8)
    Size        : 1456 bytes (0x5b0)
    Link        : Section 11 (.strtab)
    Info        : Last local symbol no = 71
    Alignment   : 4
    Entry Size  : 16


====================================

** Section #11

    Name        : .strtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 90216 (0x16068)
    Size        : 1552 bytes (0x610)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

** Section #12

    Name        : .note
    Type        : SHT_NOTE (0x00000007)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 91768 (0x16678)
    Size        : 24 bytes (0x18)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 4
    Entry Size  : 0

        Section     Segment
    ====================================

              1           0
              2           0

====================================

** Section #13

    Name        : .comment
    Type        : SHT_PROGBITS (0x00000001)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 91792 (0x16690)
    Size        : 4880 bytes (0x1310)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0

    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --verbose --library_type=microlib --callgraph --map --symbols --xref --cpu=Cortex-M0+ --datacompressor=off --list=../../../../output/boot_ram/boot_ram.map --output=../../../../output/boot_ram/boot_ram.elf --scatter=D:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/ARMCC/boot_ram_link.txt --entry=boot_ram_entry --info=any,architecture,exceptions,inline,inputs,libraries,merge,pltgot,sizes,stack,summarysizes,summarystack,tailreorder,totals,unused,unusedsymbols,veneers,veneercallers,veneerpools,visibility,weakrefs

    Input Comments:
    
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/boot_ram.o --vfemode=force

    Input Comments:
    
    p30ec-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    boot_ram.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    
    
    
    flash.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/dma_for_qspi.o --vfemode=force

    Input Comments:
    
    p3c24-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    dma_for_qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    
    
    
    qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armlink [4d35a8]
    armlink --partial --no_add_relocs_to_undefined --no_generate_mapping_symbols --diag_suppress=6642 --cpu=Cortex-M0+ --fpu=SoftVFP --output=CMakeFiles/boot_ram.dir/qspi.o --vfemode=force

    Input Comments:
    
    p31cc-3
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armasm [4d35a5]
    armasm --debug --diag_suppress=1602,1073 --cpu=Cortex-M0+ --fpu=SoftVFP --apcs=/interwork --no_divide 
    qspi.o
    
    Component: ARM Compiler 5.06 update 1 (build 61) Tool: armcc [4d35ad]
    armcc --c99 --bss_threshold=0 --split_sections --debug -c  --cpu=Cortex-M0+ -ID:/svn/apollo/Trunk/. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../app/config -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../../ip -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../../inc/cmsis -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/../.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/.. -ID:/svn/apollo/Trunk/plf/apollo_00/bootloader/boot_ram/. -DHW_BX_VERSION=00 -DNDEBUG -D__BOOT_RAM__ -D__MICROLIB
    
    
    
    
    
    
    

====================================

** Section #14

    Name        : .shstrtab
    Type        : SHT_STRTAB (0x00000003)
    Flags       : None (0x00000000)
    Addr        : 0x00000000
    File Offset : 96672 (0x179a0)
    Size        : 148 bytes (0x94)
    Link        : SHN_UNDEF
    Info        : 0
    Alignment   : 1
    Entry Size  : 0


====================================

address     size       variable name                            type
0x0012dff0  0x1c       boot_ram_header                          const boot_ram_header_t
0x0012dff0  0x10       boot_ram_header.header                   boot_header_t
0x0012dff0  0x4        boot_ram_header.header.bx_flag           array[4] of uint8_t
0x0012dff4  0x4        boot_ram_header.header.base_addr         pointer to uint8_t
0x0012dff8  0x4        boot_ram_header.header.length            uint32_t
0x0012dffc  0x4        boot_ram_header.header.entry_point       entry_point_t
0x0012e000  0x4        boot_ram_header.flash_total_size         volatile uint32_t
0x0012e004  0x4        boot_ram_header.nvds_base                volatile uint32_t
0x0012e008  0x4        boot_ram_header.image_limit              volatile uint32_t

address     size       variable name                            type
0x0012e68c  0xc0       image_header                             array[2] of image_header_t

