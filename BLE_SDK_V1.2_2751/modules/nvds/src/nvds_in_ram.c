/**
 ****************************************************************************************
 *
 * @file nvds_in_ram.c
 *
 * @brief Non Volatile Data Stored in RAM
 *
 * Copyright (C) Apollo 2015-2016
 *
 *
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @addtogroup NVDS_IN_RAM
 * @{
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 #include "bx_config.h"
 #include <stdint.h>
 #include <stddef.h>
 #include <string.h>
 #include <stdbool.h>
 #include <limits.h>
 #include "nvds_in_ram.h"
 #include "bx_dbg.h"
 #include "log.h"
 #include "flash.h"
 #include "boot.h"
 #include "co_math.h"
 #include "dw_apb_ssi_typedef.h"
 #include "nvds_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#define NVDS_TAG_TABLE_MAX 31     // must be a prime number
#define HASH1_R 7 // must be a prime number
#if HASH1_R >= NVDS_TAG_TABLE_MAX
#error HASH1_R should be smaller than NVDS_TAG_TABLE_MAX
#endif
#define NVDS_CONSTANT "NVDS"
#define NVDS_HASH0(x) (x)
#define NVDS_HASH1(x) (HASH1_R - ((x)%HASH1_R))
#define NVDS_ERASE_SIZE 0x1000

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

typedef struct
{
    uint8_t struct_size;
    uint8_t name;
    uint16_t offset;
}nvds_tag_hash_table_t;
typedef struct{
    nvds_tag_hash_table_t tag_table[NVDS_TAG_TABLE_MAX];
    uint8_t (*read)(uint32_t src,uint32_t length,uint8_t *dst);
    uint8_t (*write)(uint32_t dst,uint32_t length,uint8_t *src);
    uint8_t (*erase)(uint32_t addr);
    uint8_t *base;
    uint8_t *available;
    uint32_t ver;
    uint8_t current_blk;
}nvds_env_t;
nvds_env_t nvds_env;
#if LOCAL_NVDS == 0
static uint8_t nvds_ram_buf[NVDS_DATA_SECTION_MAX_SIZE];
#endif

/*
 * FUNCTION DECLARE
 ****************************************************************************************
 */
static uint8_t *nvds_index_build(uint8_t *base,uint8_t *start);

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#if LOCAL_NVDS == 0

static uint8_t memread_wrapper(uint32_t src,uint32_t length,uint8_t *dst)
{
    memcpy(dst,(uint8_t *)src,length);
    return 0;
}

static uint8_t memwrite_wrapper(uint32_t dst,uint32_t length,uint8_t *src)
{
    memcpy((uint8_t *)dst,src,length);
    return 0;
}

static uint8_t memerase_wrapper(uint32_t addr)
{
    memset((uint8_t *)addr,0,NVDS_ERASE_SIZE);
    return 0;
}

void nvds_ram_buf_init(uint8_t const*data,uint16_t length)
{
    memcpy(nvds_ram_buf,data,length);
    nvds_env.base = nvds_ram_buf;
    nvds_env.available = nvds_index_build(nvds_env.base,nvds_env.base+sizeof(nvds_data_head_t));
}

#else

static uint8_t flash_erase_wrapper(uint32_t addr)
{
    return flash_erase(addr,Sector_Erase);
}

static uint32_t nvds_data_ver_get(uint8_t *base)
{
    nvds_data_head_t head;
    nvds_env.read((uint32_t)base,sizeof(nvds_data_head_t),(uint8_t *)&head);
    if(memcmp(head.magic_flag,NVDS_CONSTANT,NVDS_MAGIC_FLAG_SIZE))
    {
        return 0;
    }else
    {
        return head.ver;
    }
}

static void nvds_move_to_another_blk()
{
    uint8_t *new_base;
    if(nvds_env.current_blk == 0)
    {
        new_base = nvds_env.base + NVDS_DATA_SECTION_MAX_SIZE;
    }else
    {
        new_base = nvds_env.base - NVDS_DATA_SECTION_MAX_SIZE;
    }
    nvds_env.erase((uint32_t)new_base);
    uint8_t *new_available = new_base + sizeof(nvds_data_head_t);
    uint8_t tmp[UCHAR_MAX];
    uint8_t i;
    for(i=0;i<NVDS_TAG_TABLE_MAX;++i)
    {
        if(nvds_env.tag_table[i].struct_size && nvds_env.tag_table[i].offset)
        {
            nvds_env.read((uint32_t)nvds_env.base + nvds_env.tag_table[i].offset,nvds_env.tag_table[i].struct_size,tmp);
            nvds_env.write((uint32_t)new_available,nvds_env.tag_table[i].struct_size,tmp);
            nvds_env.tag_table[i].offset = new_available - new_base;
            new_available += nvds_env.tag_table[i].struct_size;
        }
    }
    nvds_data_head_t new_head = {
        .magic_flag = {'N','V','D','S'},
        .ver = nvds_env.ver + 1,
    };
    nvds_env.write((uint32_t)new_base,sizeof(new_head),(uint8_t *)&new_head);
    
    nvds_env.base = new_base;
    nvds_env.available = new_available;
    nvds_env.ver += 1;
    nvds_env.current_blk = !nvds_env.current_blk;
}

#endif



static bool nvds_chk_idx_available(uint8_t idx,uint8_t name)
{
    return nvds_env.tag_table[idx].struct_size == 0 || nvds_env.tag_table[idx].name == name;
}

static nvds_tag_hash_table_t *nvds_get_available_idx(uint8_t tag_name)
{
    uint8_t hash = NVDS_HASH0(tag_name);
    uint8_t step = NVDS_HASH1(tag_name);
    hash %= NVDS_TAG_TABLE_MAX;
    uint8_t initial = hash;
    while(nvds_chk_idx_available(hash,tag_name)==false)
    {
        hash = (hash + step)%NVDS_TAG_TABLE_MAX;
        if(hash == initial)
        {
            return NULL;
        }
    }
    return &nvds_env.tag_table[hash];
}

static nvds_tag_hash_table_t *nvds_tag_search(uint8_t tag_name)
{
    uint8_t hash = NVDS_HASH0(tag_name);
    uint8_t step = NVDS_HASH1(tag_name);
    hash %= NVDS_TAG_TABLE_MAX;
    if(nvds_env.tag_table[hash].struct_size)
    {
        if(nvds_env.tag_table[hash].name==tag_name)
        {
            return &nvds_env.tag_table[hash];
        }else
        {
            uint8_t initial = hash;
            while(1)
            {
                hash = (hash + step) % NVDS_TAG_TABLE_MAX;
                if(hash == initial)
                {
                    return NULL;
                }
                if(nvds_env.tag_table[hash].name == tag_name)
                {
                    return &nvds_env.tag_table[hash];
                }
            }
        }
    }else
    {
        return NULL;
    }
}

static void nvds_write_new_item(nvds_tag_hash_table_t *ptr,uint8_t name,uint32_t length,uint8_t *buf)
{
    nvds_tag_head_t tag_head;
    TAG_HEAD_INIT(&tag_head,name,length);
    nvds_env.write((uint32_t)nvds_env.available + sizeof(tag_head),length,buf);
    nvds_env.write((uint32_t)nvds_env.available,sizeof(tag_head),(uint8_t *)&tag_head);
    ptr->name = name;
    ptr->struct_size = sizeof(tag_head) + length;
    ptr->offset = nvds_env.available - nvds_env.base;
    nvds_env.available += ptr->struct_size;
    #if LOCAL_NVDS == 0
    nvds_data_put_ind_send(name,buf,length);
    #endif
}

static void nvds_invalid_item(uint16_t offset)
{
    tag_label_t label = {
        .valid = 0,
        .reserved = 0x7f,
    };
    nvds_env.write((uint32_t)nvds_env.base+offset+offsetof(nvds_tag_head_t,label),sizeof(label),(uint8_t *)&label);
}

static uint8_t *nvds_index_build(uint8_t *base,uint8_t *start)
{
    nvds_tag_head_t tag_head;
    do{
        nvds_env.read((uint32_t)start,sizeof(tag_head),(uint8_t *)&tag_head);
        uint8_t size = NVDS_TAG_STRUCT_SIZE(&tag_head);
        if(size)
        {
            if(tag_head.label.valid)
            {
                nvds_tag_hash_table_t *ptr = nvds_get_available_idx(tag_head.name);
                if(ptr)
                {
                    if(ptr->name == tag_head.name)
                    {
                        LOG(LOG_LVL_WARN,"unexpected multiple valid tag\n");
                        nvds_invalid_item(ptr->offset);
                    }
                    ptr->name = tag_head.name;
                    ptr->struct_size = size;
                    ptr->offset = start-base;
                }else
                {
                    LOG(LOG_LVL_ERROR,"no space for nvds index\n");
                    BX_ASSERT(0);
                }
            }
            start += size;
        }else
        {
            break;
        }
    }while(start - base < NVDS_DATA_SECTION_MAX_SIZE);
    return start;
}




/*
 * EXPORTED FUNCTION DEFINITIONS
 ****************************************************************************************
 */


uint8_t nvds_init(uint8_t *base, uint32_t len)
{
    #if LOCAL_NVDS == 1
    BX_ASSERT(((uint32_t)base&0xfff)==0);
    nvds_env.read = flash_read_func_ptr_get();
    nvds_env.write = flash_program;
    nvds_env.erase = flash_erase_wrapper;
    uint32_t nvds_ver[2] = 
    {
        [0] = nvds_data_ver_get(base),
        [1] = nvds_data_ver_get(base + NVDS_DATA_SECTION_MAX_SIZE),
    };
    if(nvds_ver[1] > nvds_ver[0])
    {
        nvds_env.base = base + NVDS_DATA_SECTION_MAX_SIZE;
        nvds_env.ver = nvds_ver[1];
        nvds_env.current_blk = 1;
    }else
    {
        nvds_env.base = base;
        nvds_env.ver = nvds_ver[0];
        nvds_env.current_blk = 0; 
    }
    nvds_env.available = nvds_index_build(nvds_env.base,nvds_env.base+sizeof(nvds_data_head_t));
    #else
    nvds_env.read= memread_wrapper;
    nvds_env.write = memwrite_wrapper;
    nvds_env.erase = memerase_wrapper;
    #endif
    return NVDS_OK;
}

uint8_t nvds_get(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf)
{
    nvds_tag_hash_table_t *ptr = nvds_tag_search(tag);
    if(ptr)
    {
        uint8_t data_length = ptr->struct_size - sizeof(nvds_tag_head_t);
        if(data_length > *lengthPtr)
        {
            return NVDS_LENGTH_OUT_OF_RANGE;
        }else
        {
            *lengthPtr = data_length;
            nvds_env.read((uint32_t)nvds_env.base + ptr->offset + sizeof(nvds_tag_head_t),data_length,buf);
            return NVDS_OK;
        }
    }else
    {
        return NVDS_TAG_NOT_DEFINED;
    }
}

uint8_t nvds_del(uint8_t tag)
{
    #if LOCAL_NVDS == 1
    nvds_tag_hash_table_t *ptr = nvds_tag_search(tag);
    if(ptr)
    {
        nvds_invalid_item(ptr->offset);
        ptr->struct_size = 0;
        ptr->name = 0;
        ptr->offset = 0;
        return NVDS_OK;
    }else
    {
        return NVDS_TAG_NOT_DEFINED;
    }
    #else
    return NVDS_FAIL;
    #endif
}

uint8_t nvds_put(uint8_t tag, nvds_tag_len_t length, uint8_t *buf)
{
    #if LOCAL_NVDS == 0
    if(nvds_env.base ==NULL)
    {
        return NVDS_OK;
    }
    #endif
    if (nvds_env.available+ length + sizeof(nvds_tag_head_t)>nvds_env.base+NVDS_DATA_SECTION_MAX_SIZE)
    {
        #if LOCAL_NVDS == 1
        nvds_move_to_another_blk();
        #else
        return NVDS_NO_SPACE_AVAILABLE;
        #endif
    }
    nvds_tag_hash_table_t *ptr = nvds_tag_search(tag);
    if(ptr)
    {
        uint16_t old_offset = ptr->offset;
        nvds_write_new_item(ptr,tag,length,buf);
        if(old_offset)
        {
            nvds_invalid_item(old_offset);
        }
        return NVDS_OK;
    }else
    {
        nvds_tag_hash_table_t *item = nvds_get_available_idx(tag);
        if(item)
        {
            nvds_write_new_item(item,tag,length,buf);
            return NVDS_OK;
        }else
        {
            return NVDS_NO_SPACE_AVAILABLE;
        }
    }
}

