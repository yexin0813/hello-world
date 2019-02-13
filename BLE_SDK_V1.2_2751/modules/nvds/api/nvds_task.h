#ifndef NVDS_TASK_H_
#define NVDS_TASK_H_
#include <stdint.h>
#include "rwip_config.h"
#include "rwip_task.h"
#include "task_id_patch.h"

#define NVDS_IDX_MAX 1

enum nvds_state_id
{
    NVDS_EMPTY,
    NVDS_READY,
    NVDS_STATE_MAX,
};

enum nvds_msg_id
{
    NVDS_SET_DATA_CMD = TASK_FIRST_MSG(TASK_ID_NVDS),
    NVDS_DATA_PUT_IND ,
};

struct nvds_set_data_cmd
{
    uint16_t length;
    uint8_t nvds[];
};

struct nvds_data_put_ind
{
    uint16_t length;
    uint8_t tag_name;
    uint8_t data[];
};

void nvds_task_init(void);

void nvds_data_put_ind_send(uint8_t name,uint8_t *data,uint16_t length);
#endif
