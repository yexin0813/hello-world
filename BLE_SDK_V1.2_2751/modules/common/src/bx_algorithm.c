#include <stdint.h>
#define GET_ARRAY_ELMT_PTR(array_base,type_size,idx) ((uint8_t *)(array_base) + (type_size)*(idx))

void bx_swap(void *a,void *b,uint32_t type_size)
{
    uint32_t quotient = type_size / 4;
    uint8_t remainder = type_size % 4;
    uint8_t p16 = remainder / 2;
    uint8_t p8 = remainder % 2;
    uint32_t *p32_a;
    uint32_t *p32_b;
    uint32_t i;
    for(i=0,p32_a = a,p32_b = b;i<quotient;++i,++p32_a,++p32_b)
    {
        *p32_a = *p32_a ^ *p32_b;
        *p32_b = *p32_a ^ *p32_b;
        *p32_a = *p32_a ^ *p32_b;
    }
    uint16_t *p16_a = p32_a;
    uint16_t *p16_b = p32_b;
    if(p16)
    {
        *p16_a = *p16_a ^ *p16_b;
        *p16_b = *p16_a ^ *p16_b;
        *p16_a = *p16_a ^ *p16_b;
        ++p16_a;
        ++p16_b;
    }
    uint8_t *p8_a = p16_a;
    uint8_t *p8_b = p16_b;
    if(p8)
    {
        *p8_a = *p8_a ^ *p8_b;
        *p8_b = *p8_a ^ *p8_b;
        *p8_a = *p8_a ^ *p8_b;
    }
}

static void *qsort_median3(void *array,uint32_t type_size,int (*cmp)(void *,void *),uint32_t left,uint32_t right)
{
    uint32_t center = (left+right)/2;
    if(cmp(GET_ARRAY_ELMT_PTR(array,type_size,left),GET_ARRAY_ELMT_PTR(array,type_size,center))>0)
    {
        swap(GET_ARRAY_ELMT_PTR(array,type_size,left),GET_ARRAY_ELMT_PTR(array,type_size,center),type_size);
    }
    if(cmp(GET_ARRAY_ELMT_PTR(array,type_size,left),GET_ARRAY_ELMT_PTR(array,type_size,right))>0)
    {
        swap(GET_ARRAY_ELMT_PTR(array,type_size,left),GET_ARRAY_ELMT_PTR(array,type_size,right),type_size);
    }
    if(cmp(GET_ARRAY_ELMT_PTR(array,type_size,center),GET_ARRAY_ELMT_PTR(array,type_size,right))>0)
    {
        swap(GET_ARRAY_ELMT_PTR(array,type_size,center),GET_ARRAY_ELMT_PTR(array,type_size,right),type_size);
    }
    swap(GET_ARRAY_ELMT_PTR(array,type_size,center),GET_ARRAY_ELMT_PTR(array,type_size,right-1)); //hide pivot
    return GET_ARRAY_ELMT_PTR(array,type_size,right-1);
}

void bx_qsort(void *array,uint32_t type_size,int (*cmp)(void *,void *),uint32_t left,uint32_t right)
{
    uint32_t i,j;
    if(left<right)
    {
        void *pivot = qsort_median3(array,type_size,cmp,left,right);
        i = left;
        j = right -1;
        while(1)
        {
            while(cmp(GET_ARRAY_ELMT_PTR(array,type_size,++i),pivot)<0);
            while(cmp(GET_ARRAY_ELMT_PTR(array,type_size,--j),pivot)>0);
            if(i<j)
            {
                swap(GET_ARRAY_ELMT_PTR(array,type_size,i),GET_ARRAY_ELMT_PTR(array,type_size,j));
            }else{
                break;
            }
            bx_swap(GET_ARRAY_ELMT_PTR(array,type_size,i),GET_ARRAY_ELMT_PTR(array,type_size,right - 1));
            qsort(array,type_size,cmp,left, i-1);
            qsort(array,type_size,cmp, i+1,right);
        }
    }
}
