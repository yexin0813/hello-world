#ifndef FIELD_MANIPULATE_H_
#define FIELD_MANIPULATE_H_
#include <stdint.h>

#define REG_FIELD_WR(reg,field,val)\
        do{                                 \
        uint32_t old_val = (reg);\
        uint32_t new_val = ((val)<<(field##_POS)&(field##_MASK))|(old_val &~(field##_MASK));\
        (reg)= new_val;\
    }while(0)

#define REG_FIELD_RD(reg,field)\
    (((reg)&(field##_MASK))>>(field##_POS))
    
#define FIELD_WR(base,member,field,val) \
    do{                                 \
        uint32_t old_val = (base)->member;\
        uint32_t new_val = ((val)<<(field##_POS)&(field##_MASK))|(old_val &~(field##_MASK));\
        (base)->member = new_val;\
    }while(0)
    
#define FIELD_RD(base,member,field) \
    ((((base)->member)&(field##_MASK))>>(field##_POS))
    
#define FIELD_BUILD(field,val) \
    ((val)<<(field##_POS)&(field##_MASK))

#endif
