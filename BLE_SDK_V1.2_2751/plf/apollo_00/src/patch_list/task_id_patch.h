#ifndef TASK_ID_PATCH_H_
#define TASK_ID_PATCH_H_
#include "bx_config.h"
#include "rwip_config.h"
#if(LOCAL_NVDS==0 || MESH_SCHED_PATCH == 1)
enum extended_task_type
{
    #if(LOCAL_NVDS == 0)
    #if(MESH_SCHED_PATCH == 0)
    TASK_NVDS = (TASK_AHI - 1),
    #else
    TASK_NVDS = (TASK_AHI - 2),
    #endif
    #endif
    #if(MESH_SCHED_PATCH == 1)
    TASK_MESH_SCHED = (TASK_AHI - 1),
    #endif
};
#endif

#endif
