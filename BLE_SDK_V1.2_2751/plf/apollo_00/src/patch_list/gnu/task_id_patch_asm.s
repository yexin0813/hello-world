    .syntax    unified
    .arch    armv6-m
    .text
    .thumb
    .thumb_func
    .align    1    
    .globl GET_TASK_FROM_ID_PATCH
    .type GET_TASK_FROM_ID_PATCH,%function
GET_TASK_FROM_ID_PATCH:
    BL gapm_get_task_from_id_patch
    POP      {r4,pc}
    
    .globl GET_ID_FROM_TASK_PATCH
    .type GET_ID_FROM_TASK_PATCH,%function
GET_ID_FROM_TASK_PATCH:
    BL gapm_get_id_from_task_patch
    POP      {r4,pc}
    
    
    .end
