// ----------------------------------------------------------------------------
// FILE NAME : c200.h
// DESCRIPTION: Header file for Embedded Interrupt Controller, general register 
//              and some TBox Registers for generating interrupt 
// ----------------------------------------------------------------------------
#ifndef _C200_H_
#define _C200_H_

#include "global.h"
// ----- Basic Operation ----//
#define bit_read(r,b) ((r>>b) & 0x1 )
#define bits_read(r,b,m) ((r >> b) & m)

#define bit_write(r,b) (r |= (0x1 << b)  )
#define bits_write(r,b,m,v) ( r = (( r & ~(m <<b)) | (v << b)) )

#define bit_clear(r,b) (r &= ~(1<<b))
#define bits_clear(r,b,m)  (r &= ~(m<<b))

//---------------------------------------------------------------------------- 
// General register read/write(read from a g_reg; write to a g_reg)
// r0 r/w
#define G_R0_write(var)                                 __asm__  __volatile__("MOV r0, %[value]"::[value]"r"(var));
#define G_R0_read(var)                                  __asm__  __volatile__("MOV %[value],r0":[value]"=r"(var));
// r1 r/w
#define G_R1_write(var)                                 __asm__  __volatile__("MOV r1, %[value]"::[value]"r"(var));
#define G_R1_read(var)                                  __asm__  __volatile__("MOV %[value],r1":[value]"=r"(var));
// r2 r/w
#define G_R2_write(var)                                 __asm__  __volatile__("MOV r2, %[value]"::[value]"r"(var));
#define G_R2_read(var)                                  __asm__  __volatile__("MOV %[value],r2":[value]"=r"(var));
// r3 r/w
#define G_R3_write(var)                                 __asm__  __volatile__("MOV r3, %[value]"::[value]"r"(var));
#define G_R3_read(var)                                  __asm__  __volatile__("MOV %[value],r3":[value]"=r"(var));
// r4 r/w
#define G_R4_write(var)                                 __asm__  __volatile__("MOV r4, %[value]"::[value]"r"(var));
#define G_R4_read(var)                                  __asm__  __volatile__("MOV %[value],r4":[value]"=r"(var));
// r5 r/w
#define G_R5_write(var)                                 __asm__  __volatile__("MOV r5, %[value]"::[value]"r"(var));
#define G_R5_read(var)                                  __asm__  __volatile__("MOV %[value],r5":[value]"=r"(var));
// r6 r/w
#define G_R6_write(var)                                 __asm__  __volatile__("MOV r6, %[value]"::[value]"r"(var));
#define G_R6_read(var)                                  __asm__  __volatile__("MOV %[value],r6":[value]"=r"(var));
// r7 r/w
#define G_R7_write(var)                                 __asm__  __volatile__("MOV r7, %[value]"::[value]"r"(var));
#define G_R7_read(var)                                  __asm__  __volatile__("MOV %[value],r7":[value]"=r"(var));
// r8 r/w
#define G_R8_write(var)                                 __asm__  __volatile__("MOV r8, %[value]"::[value]"r"(var));
#define G_R8_read(var)                                  __asm__  __volatile__("MOV %[value],r8":[value]"=r"(var));
// r9 r/w
#define G_R9_write(var)                                 __asm__  __volatile__("MOV r9, %[value]"::[value]"r"(var));
#define G_R9_read(var)                                  __asm__  __volatile__("MOV %[value],r9":[value]"=r"(var));
// r10 r/w
#define G_R10_write(var)                                 __asm__  __volatile__("MOV r10, %[value]"::[value]"r"(var));
#define G_R10_read(var)                                  __asm__  __volatile__("MOV %[value],r10":[value]"=r"(var));
// r11 r/w
#define G_R11_write(var)                                 __asm__  __volatile__("MOV r11, %[value]"::[value]"r"(var));
#define G_R11_read(var)                                  __asm__  __volatile__("MOV %[value],r11":[value]"=r"(var));
// r12 r/w
#define G_R12_write(var)                                 __asm__  __volatile__("MOV r12, %[value]"::[value]"r"(var));
#define G_R12_read(var)                                  __asm__  __volatile__("MOV %[value],r12":[value]"=r"(var));


//--------------------Embedded Interrupt Controller-----------------------------------------------------

// Interrupt Control Status Register
#define ICSR_READ(icsr)				     icsr = (*(volatile UINT32*)(0x00c40000))
#define ICSR_WRITE(icsr)			      (*(volatile UINT32*)(0x00c40000)) = icsr
// Interrupt Enable Register
#define IER_READ(ier)                                ier = *(volatile UINT32*)(0x00c40010) 
#define IER_WRITE(ier)                                *(volatile UINT32*)(0x00c40010) = ier 
// Interrupt Pending Set Register
#define IPSR_READ(ipsr)                                ipsr = *(volatile UINT32*)(0x00c40018) 
#define IPSR_WRITE(ipsr)                                *(volatile UINT32*)(0x00c40018) = ipsr 
// Interrupt Pending Clear Register 
#define IPCR_READ(ipcr)                                ipcr = *(volatile UINT32*)(0x00c4001c) 
#define IPCR_WRITE(ipcr)                                *(volatile UINT32*)(0x00c4001c) = ipcr 
// Interrupt Priority Level Select group Registers 
// PLSGRG0
#define PLSGR0_READ(plsgr0)                        plsgr0 = *(volatile UINT32*)(0x00c40040)
#define PLSGR0_WRITE(plsgr0)                        *(volatile UINT32*)(0x00c40040) = plsgr0
// PLSGRG1
#define PLSGR1_READ(plsgr1)                        plsgr1 = *(volatile UINT32*)(0x00c40044)
#define PLSGR1_WRITE(plsgr1)                        *(volatile UINT32*)(0x00c40044) = plsgr1
// PLSGRG2
#define PLSGR2_READ(plsgr2)                        plsgr2 = *(volatile UINT32*)(0x00c40048)
#define PLSGR2_WRITE(plsgr2)                        *(volatile UINT32*)(0x00c40048) = plsgr2
// PLSGRG3
#define PLSGR3_READ(plsgr3)                        plsgr3 = *(volatile UINT32*)(0x00c4004c)
#define PLSGR3_WRITE(plsgr3)                        *(volatile UINT32*)(0x00c4004c) = plsgr3
// PLSGRG4
#define PLSGR4_READ(plsgr4)                        plsgr4 = *(volatile UINT32*)(0x00c40050)
#define PLSGR4_WRITE(plsgr4)                        *(volatile UINT32*)(0x00c40050) = plsgr4
// PLSGR5
#define PLSGR5_READ(plsgr5)                        plsgr5 = *(volatile UINT32*)(0x00c40054)
#define PLSGR5_WRITE(plsgr5)                        *(volatile UINT32*)(0x00c40054) = plsgr5
// PLSGR6
#define PLSGR6_READ(plsgr6)                        plsgr6 = *(volatile UINT32*)(0x00c40058)
#define PLSGR6_WRITE(plsgr6)                        *(volatile UINT32*)(0x00c40058) = plsgr6
// PLSGR7
#define PLSGR7_READ(plsgr7)                        plsgr7 = *(volatile UINT32*)(0x00c4005c)
#define PLSGR7_WRITE(plsgr7)                        *(volatile UINT32*)(0x00c4005c) = plsgr7

#define PLSRV0  0x00
#define PLSRV1  0x40
#define PLSRV2  0x80
#define PLSRV3  0xC0
//--------------------Debug----------------------------------------------------

//-------------------TBox Register Definition----------------------------------
//----------------irq 0 -----------------------------
// config irq0, bit 31, interrupt type, 1: pulse; 0: level(default)
//              bit[4:0] interrupt number 
// init interrupt must config this field
#define CNF_IRQ0(number)  (*(volatile UINT32*)(0x130000C0)) = number  // config enable interrupt's number and type
#define STR_IRQ0(cyclenum)     (*(volatile UINT32*)(0x1300000C)) = cyclenum // interrupt will start up after cyclenum cycle
#define CLR_IRQ0            (*(volatile UINT32*)(0x13000014)) = 0
//---------------irq 1------------------------------
// config irq1, bit 31, interrupt type, 1: pulse; 0: level(default)
//              bit[4:0] interrupt number
#define CNF_IRQ1(number)  (*(volatile UINT32*)(0x130000D4)) = number  // config enable interrupt's number and type
#define STR_IRQ1(cyclenum)     (*(volatile UINT32*)(0x130000CC)) = cyclenum // interrupt will start up after cyclenum cycle
#define CLR_IRQ1            (*(volatile UINT32*)(0x130000D0)) = 0

// NMI
#define STR_NMI(cyclenum)   (*(volatile UINT32*)(0x130000C4)) = cyclenum // NMI will start up after cyclenum cycle
#define CLR_NMI               (*(volatile UINT32*)(0x130000C8)) = 0


#endif
