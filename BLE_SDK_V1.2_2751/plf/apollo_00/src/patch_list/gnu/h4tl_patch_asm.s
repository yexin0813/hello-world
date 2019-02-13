    .syntax	unified
	.arch	armv6-m
    .text
    .thumb
	.thumb_func
    .align	1    
    .globl H4TL_PATCH
    .type H4TL_PATCH,%function
H4TL_PATCH:
    MOV r0,r4
    MOV r1,lr
    BL h4tl_patch_c
    MOV r1,r0
    LDRB r0,[r4,#0x16]
    BX r1
    
    .end
    
    
    
    
    
    
