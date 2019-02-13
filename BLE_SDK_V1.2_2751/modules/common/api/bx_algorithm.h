#ifndef BX_ALGORITHM_H_
#define BX_ALGORITHM_H_
#include <stdint.h>

void bx_swap(void *a,void *b,uint32_t type_size);

void bx_qsort(void *array,uint32_t type_size,int (*cmp)(void *,void *),uint32_t left,uint32_t right);


#endif

