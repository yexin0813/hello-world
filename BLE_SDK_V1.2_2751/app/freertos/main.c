#include <stdint.h>
#include "arch_init.h"
#include "task_init.h"
int main()
{
    soc_initialize();
    rtos_task_init();
    while(1);    
}
