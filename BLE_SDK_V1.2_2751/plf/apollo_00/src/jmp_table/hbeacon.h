#ifndef HBEACON_H_
#define HBEACON_H_
#include <stdint.h>
#include "jump_table.h"

typedef enum
{
	HBEACON_START,
	HBEACON_STOP,
	HBEACON_DATA_BACKUP,
	HBEACON_SCHED,
}Hbeacon_entry_t;
#ifdef __RAM_CODE__
void Hbeacon_trap(uint32_t operation, void *p1,void *p2,void *p3);
#else
typedef void (*Hbeacon_trap_fn)(uint32_t,void *,void *,void *);
#define Hbeacon_trap(op,p1,p2,p3) ((Hbeacon_trap_fn)jump_table[HBEACON])((op),(p1),(p2),(p3))
#endif
#endif

