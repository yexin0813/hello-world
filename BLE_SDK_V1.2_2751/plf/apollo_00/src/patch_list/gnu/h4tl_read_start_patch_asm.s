    .syntax    unified
    .arch    armv6-m
    .text
    .thumb
    .thumb_func
    .align    1    
    .globl H4TL_READ_START_PATCH
    .type H4TL_READ_START_PATCH,%function
H4TL_READ_START_PATCH:
    BL h4tl_read_start_patch
    POP      {r4,pc}

    .end
    