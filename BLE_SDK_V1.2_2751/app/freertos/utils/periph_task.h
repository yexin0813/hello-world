#ifndef PERIPH_TASK_H_
#define PERIPH_TASK_H_


#include "FreeRTOS.h"
#include "task.h"
extern TaskHandle_t handler_dma_m2m_task;
extern TaskHandle_t handler_division_task;
extern TaskHandle_t handler_uart_tx_task;
extern TaskHandle_t handler_flash_write_task;
extern TaskHandle_t handler_iic_task;
void iic_task(void * params);
void flash_write_task(void *params);
void dma_m2m_task(void *params);
void division_task(void *params);
void uart_tx_task(void *params);
void dma_isr(void);

#endif
