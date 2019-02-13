/**
 ****************************************************************************************
 *
 * @file bxota.c
 *
 * @brief Apollo BLE OTA Implementation.
 *
 * Copyright (C) Apollo 2015-2016
 *
 *
 ****************************************************************************************
 */

 /**
 ****************************************************************************************
 * @addtogroup BXOTAS
 * @{
 ****************************************************************************************
 */

 /*
 * INCLUDE FILES
 ****************************************************************************************
 */
 #include "rwip_config.h"
 #if (BLE_BXOTA_SERVER)
 #include <stddef.h>
 #include "attm.h"
 #include "prf_utils.h"
 #include "prf.h"
 #include "plf.h"
 #include "flash.h"
 #include "bxotas.h"
 #include "ke_mem.h"
 #include "log.h"
 #include "bxotas_task.h"
 #include "co_math.h"


 /*
 * MACROS
 ****************************************************************************************
 */
#define IMAGE_OFFSET_BASE 				0x1000  //boot ram size
#define IMAGE_LIMIT_OFFSET_IN_BOOT_RAM  0x18
#define FEATURE_OFFSET_IN_IMAGE_HEADER	0x50
#define VALID_BIT_OFFSET_IN_FEATURE		0x00
#define OBSOLETE_BIT_OFFSET_IN_FEATURE      0x01


 /*
 * DECLARATIONS
 ****************************************************************************************
 */
extern uint32_t current_flash_offset;

/*
 * BXOTA ATTRIBUTES
 ****************************************************************************************
 */
 const struct attm_desc bxotas_att_db[BXOTAS_IDX_NB] =
 {
    [BXOTAS_IDX_SVC] = {ATT_DECL_PRIMARY_SERVICE,PERM(RD,ENABLE),0,0},
    [BXOTAS_IDX_CTRL_CHAR] = {ATT_DECL_CHARACTERISTIC,PERM(RD,ENABLE),0,0},
    [BXOTAS_IDX_CTRL_VAL] = {ATT_CHAR_BXOTA_CTRL,PERM(WRITE_REQ,ENABLE)|PERM(IND,ENABLE),PERM(RI,ENABLE),sizeof(bxota_ctrl_t)},
    [BXOTAS_IDX_CTRL_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,PERM(RD,ENABLE)|PERM(WRITE_REQ,ENABLE),0,0},
    [BXOTAS_IDX_DATA_CHAR] = {ATT_DECL_CHARACTERISTIC,PERM(RD,ENABLE),0,0},
    [BXOTAS_IDX_DATA_VAL] = {ATT_CHAR_BXOTA_DATA,PERM(WRITE_COMMAND,ENABLE)|PERM(RD,ENABLE),PERM(RI,ENABLE),BXOTA_ATT_DATA_MAX_LENGTH},
};

 /*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static uint8_t bxotas_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  void *params)
{
    uint8_t status = attm_svc_create_db(start_hdl, ATT_SVC_BXOTA, NULL, BXOTAS_IDX_NB, NULL, env->task, bxotas_att_db, sec_lvl);
    if(status == ATT_ERR_NO_ERROR)
    {
        struct bxotas_env_tag *bxotas_env = (struct bxotas_env_tag *)ke_malloc(sizeof(struct bxotas_env_tag),KE_MEM_ATT_DB);
        env->env = &bxotas_env->prf_env;
        bxotas_env->start_hdl = *start_hdl;
        bxotas_env->prf_env.app_task = app_task | PERM(PRF_MI, DISABLE);
        bxotas_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);
        env->id = TASK_ID_BXOTAS;
        env->desc.idx_max = BXOTAS_IDX_MAX;
        env->desc.state = bxotas_env->state;
        env->desc.default_handler = &bxotas_default_handler;

        ke_state_set(env->task,BXOTAS_IDLE);
    }
    return status;
}
static void bxotas_destroy(struct prf_task_env* env)
{
    struct bxotas_env_tag *bxotas_env =(struct bxotas_env_tag *)env->env;
    env->env = NULL;
    ke_free(bxotas_env);
}

static void bxotas_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Nothing to do */
}

static void bxotas_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    uint8_t status = ke_state_get(env->task);
    struct bxotas_env_tag *bxotas_env =(struct bxotas_env_tag *)env->env;
    if(status == BXOTAS_BUSY && conidx == bxotas_env->conidx)
    {
        ke_state_set(env->task,BXOTAS_IDLE);
    }
}

 /*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const struct prf_task_cbs bxotas_itf =
{
    bxotas_init,
    bxotas_destroy,
    bxotas_create,
    bxotas_cleanup
};
 
 /*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
const struct prf_task_cbs *bxotas_prf_itf_get()
{
    return &bxotas_itf;
}

static void bxotas_obsolete_image(uint32_t image_base)
{
    uint8_t obsolete = ~(1<<OBSOLETE_BIT_OFFSET_IN_FEATURE);
    flash_program(image_base + FEATURE_OFFSET_IN_IMAGE_HEADER,sizeof(obsolete),&obsolete);
}

static void bxotas_validate_image(uint32_t image_base)
{
    uint8_t valid = ~(1 << VALID_BIT_OFFSET_IN_FEATURE);
    flash_program(image_base + FEATURE_OFFSET_IN_IMAGE_HEADER,sizeof(valid),&valid);
}

void bxotas_image_validation(struct bxotas_env_tag *bxotas_env)
{
    uint32_t old_image_base = current_flash_offset + IMAGE_OFFSET_BASE;
    bxotas_obsolete_image(old_image_base);
    bxotas_validate_image(bxotas_env->ota_image_base);
}

void bxotas_erase_flash(uint32_t offset,uint32_t image_limit)
{
    uint8_t sector_num = CEILING(image_limit,BLOCK_4K_SECTOR);
    uint8_t i;
    for(i=0;i<sector_num;++i)
    {
        flash_erase(offset,Sector_Erase);//must be sector erase ,cannot erase boot_ram.
        offset += BLOCK_4K_SECTOR;
    }
}

void bxotas_flash_cleanup_for_ota(struct bxotas_env_tag *bxotas_env)
{
    uint32_t image_size_limit;
    flash_read(IMAGE_LIMIT_OFFSET_IN_BOOT_RAM , sizeof(image_size_limit) , (uint8_t*)&image_size_limit);
    LOG(LOG_LVL_INFO,"image_size_limit = 0x%x\n" , image_size_limit);
    bxotas_env->ota_image_base = (current_flash_offset ? 0 : image_size_limit) + IMAGE_OFFSET_BASE;
    bxotas_erase_flash(bxotas_env->ota_image_base,image_size_limit);

}

void bxotas_program_flash(struct bxotas_env_tag *bxotas_env,uint16_t segment_id,uint8_t const *data,uint16_t length)
{
    flash_program(bxotas_env->ota_image_base + (bxotas_env->current_block *(BXOTA_ACK_MAP_SIZE * 8) + segment_id) * bxotas_env->segment_data_length,length,(void *)data);
}

#endif //BLE_BXOTA_SERVER
 /// @} BXOTAS
 
