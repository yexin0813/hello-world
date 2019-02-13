#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "bxfs.h"
#include "dw_apb_ssi_typedef.h"
#include "log.h"
#include "flash.h"
#define INVALID_RECORD_OFFSET (0xffff)
#define INVALID_DIR_OFFSET (0)
typedef enum {
    NOT_TAIL_UPDTAE,
    HEAD_UPDATE,
}section_update_t;

typedef struct
{
    uint16_t node_offset: 12,
            reserved:1,
            used:1,
            head:1,
            not_tail:1;
}bxfs_section_head_t;

typedef struct{
    uint16_t others:13,
                    used:1,
                    obsolete:1,
                    node_type:1;
}__attribute__((packed)) node_info_t;

typedef struct
{
    uint16_t data_length:12,
                        reserved:1,
                        common:3;
}__attribute__((packed)) record_info_t;

typedef struct
{
    uint16_t node_idx:8,
                    reserved:5,
                    common:3;
}__attribute__((packed)) dir_info_t;

typedef union
{
    node_info_t info;
    dir_info_t dir;
    record_info_t record;
}__attribute__((packed)) node_storage_u;

typedef struct{
    node_storage_u u;
    uint8_t parent_dir_idx;
    uint8_t key;
}__attribute__((packed)) bxfs_node_storage_t;

typedef struct bxfs_dir_node_s{
    uint16_t first_child_record;
    uint16_t offset:12,
                    not_linked:1;
    uint8_t section;
    uint8_t next_sibling;
    uint8_t parent_dir_offset;
    uint8_t first_child_dir;
    uint8_t idx;
    uint8_t key;
}bxfs_dir_node_t;

typedef struct{
    uint32_t data_length:12,
                offset:12,
                section:8;
    uint16_t next_sibling;
    uint8_t parent_dir_offset;
    uint8_t key;
}bxfs_record_node_t;

bxfs_dir_node_t bxfs_dir[BXFS_DIR_NODE_MAX+1];
bxfs_record_node_t bxfs_record[BXFS_RECORD_NODE_MAX];
void const *root_dir_ptr = bxfs_dir;
static struct{
    uint8_t (*read)(uint32_t src,uint32_t length,uint8_t *dst);
    uint8_t (*write)(uint32_t dst,uint32_t length,uint8_t *src);
    uint8_t (*erase)(uint32_t addr,erase_t type);
    uint32_t base;
    uint32_t free_bytes;
    uint16_t head_valid_data_size;
    uint16_t tail_available_offset;
    uint8_t section_num;
    uint8_t head_section;
    uint8_t tail_section;
    uint8_t dir_idx_max;
}bxfs_env;

static uint8_t tmp_data_buf[BXFS_TMP_BUF_SIZE];

#define GET_FLASH_ADDR(section,offset) (bxfs_env.base + (section)*BXFS_SECTION_SIZE + (offset))

static void raw_node_read(uint32_t section_base,uint16_t offset,bxfs_node_storage_t *ptr)
{
    if(offset+sizeof(bxfs_node_storage_t)>BXFS_SECTION_SIZE)
    {
        bxfs_env.read(section_base + offset,BXFS_SECTION_SIZE - offset,(uint8_t *)ptr);
        bxfs_env.read(section_base + BXFS_SECTION_SIZE + sizeof(bxfs_section_head_t),offset + sizeof(bxfs_node_storage_t) - BXFS_SECTION_SIZE,(uint8_t *)ptr + BXFS_SECTION_SIZE - offset);
    }else
    {
        bxfs_env.read(section_base + offset,sizeof(bxfs_node_storage_t),(uint8_t *)ptr);
    }
}

static void dir_siblings_build(bxfs_dir_node_t *dir_base,uint8_t *left,uint8_t *right,uint8_t num,bxfs_dir_node_t *parent_dir)
{
    uint8_t idx = *left;
    for(idx=*left;idx<num;++idx)
    {
        if(dir_base[idx].not_linked == 1 && dir_base[idx].parent_dir_offset == parent_dir->idx)
        {
            break;
        }
    }
    *left = idx;
    while(idx<num)
    {
        if(!(dir_base[idx].not_linked == 1 && dir_base[idx].parent_dir_offset == parent_dir->idx))
        {
            break;
        }
        ++idx;
    }
    *right = idx;
    for(idx = *left;idx<*right - 1;++idx)
    {
        dir_base[idx].not_linked = 0;
        dir_base[idx].next_sibling = idx+1;
        dir_base[idx].parent_dir_offset = parent_dir - dir_base;
    }
    if(idx == *right - 1)
    {
        dir_base[idx].not_linked = 0;
        dir_base[idx].next_sibling = INVALID_DIR_OFFSET;
        dir_base[idx].parent_dir_offset = parent_dir - dir_base;
    }
}

static uint8_t dir_tree_build(bxfs_dir_node_t *dir_base,uint8_t start,uint8_t num,bxfs_dir_node_t *parent_dir)
{
    if(start<num)
    {
        uint8_t left = start;
        uint8_t right;
        dir_siblings_build(dir_base,&left,&right,num,parent_dir);
        uint8_t parent_first_child = dir_base[left].parent_dir_offset == parent_dir - dir_base ? left : INVALID_DIR_OFFSET;
        while(left != right)
        {
            dir_base[left].first_child_dir = dir_tree_build(dir_base,right,num,&dir_base[left]);
            ++left;
        }
        return parent_first_child;
    }else
    {
        return 0;
    }
}

static void record_sibling_link(bxfs_record_node_t *record,uint16_t left,uint16_t right,uint8_t parent_dir_offset)
{
    uint16_t i;
    for(i=left;i<right;++i)
    {
        record[i].next_sibling = i+1;
        record[i].parent_dir_offset = parent_dir_offset;
    }
    record[right].next_sibling = INVALID_RECORD_OFFSET;
    record[right].parent_dir_offset = parent_dir_offset;
}


static uint8_t record_dir_link(bxfs_record_node_t *record,uint16_t idx,bxfs_dir_node_t *dir,uint8_t dir_num)
{
    uint8_t i = 0;
    while(i < dir_num &&dir[i].idx != record[idx].parent_dir_offset)
    {
       ++i;
    }
    BX_ASSERT(i < dir_num);
    dir[i].first_child_record = idx;
    return i;
}

static void record_mount(bxfs_record_node_t *record,uint16_t num,bxfs_dir_node_t *dir,uint8_t dir_num)
{
    uint16_t i = 0;
    uint16_t j = 1;
    while(i<num)
    {
        if(record[j].parent_dir_offset==record[i].parent_dir_offset)
        {
            ++j;
        }else
        {
            uint8_t parent_dir_offset = record_dir_link(record,i,dir,dir_num);
            record_sibling_link(record,i,j-1,parent_dir_offset);
            i=j++;
        }
    }
}

static int dir_parent_and_idx_cmp(const void *a,const void *b)
{
    const bxfs_dir_node_t *x = a;
    const bxfs_dir_node_t *y = b;
    if(x->parent_dir_offset>y->parent_dir_offset)
    {
        return 1;
    }else if(x->parent_dir_offset<y->parent_dir_offset)
    {
        return -1;
    }else
    {
        if(x->idx<y->idx)
        {
            return -1;
        }else if(x->idx>y->idx)
        {
            return 1;
        }else
        {
            return 0;
        }
    }
}

static int record_parent_idx_cmp(const void *a,const void *b)
{
    const bxfs_record_node_t *x = a;
    const bxfs_record_node_t *y = b;
    if(x->parent_dir_offset>y->parent_dir_offset)
    {
        return 1;
    }else if(x->parent_dir_offset<y->parent_dir_offset)
    {
        return -1;
    }else
    {
        return 0;
    }
}

static void section_head_program(uint8_t section,uint16_t node_offset,bool head,bool tail)
{
    bxfs_section_head_t section_head;
    *(uint16_t *)&section_head = 0xffff;
    section_head.node_offset = node_offset;
    section_head.used = 0;
    section_head.head = head? 0 : 1;
    section_head.not_tail = tail? 1 : 0;
    bxfs_env.write(bxfs_env.base+section*BXFS_SECTION_SIZE,sizeof(bxfs_section_head_t),(uint8_t *)&section_head);
}

static uint16_t section_valid_data_count(uint8_t section)
{
    uint16_t valid_data_size = 0;
    uint16_t i;
    for(i=1;i<BXFS_DIR_NODE_MAX+1;++i)
    {
        if(bxfs_dir[i].idx && bxfs_dir[i].section==section)
        {
            valid_data_size += sizeof(bxfs_node_storage_t);
        }
    }
    for(i=0;i<BXFS_RECORD_NODE_MAX;++i)
    {
        if(bxfs_record[i].data_length && bxfs_record[i].section == section)
        {
            valid_data_size += sizeof(bxfs_node_storage_t) + bxfs_record[i].data_length;
        }
    }
    return valid_data_size;
}

void bxfs_init(uint32_t base,uint16_t section_num)
{
    BX_ASSERT(((uint32_t)base&0xfff)==0);
    BX_ASSERT(section_num>=3);
    bxfs_env.read = flash_read_func_ptr_get();
    bxfs_env.write = flash_program;
    bxfs_env.erase = flash_erase;
    bxfs_env.base = base;
    bxfs_env.section_num = section_num;
    bxfs_env.free_bytes = section_num * (BXFS_SECTION_SIZE - sizeof(bxfs_section_head_t));
    bool all_section_empty = true;
    uint8_t dir_node = 1;
    uint16_t record_node = 0;
    uint16_t i;
    for(i=0;i<bxfs_env.section_num;++i)
    {
        bxfs_section_head_t section_head;
        bxfs_env.read(base+i*BXFS_SECTION_SIZE,sizeof(section_head),(uint8_t *)&section_head);
        if(section_head.used)
        {
            continue;
        }
        all_section_empty = false;
        if(section_head.head == 0)
        {
            bxfs_env.head_section = i;
        }

        uint16_t offset = section_head.node_offset;
        do{
            bxfs_node_storage_t raw_node;
            raw_node_read(base +i*BXFS_SECTION_SIZE,offset,&raw_node);
            if(raw_node.u.info.used != 0)
            {
                break;
            }
            uint16_t node_size = raw_node.u.info.node_type == DIRECTORY_NODE ?
                sizeof(bxfs_node_storage_t) : sizeof(bxfs_node_storage_t) + raw_node.u.record.data_length;
            bxfs_env.free_bytes -=   node_size;
            if(raw_node.u.info.obsolete != 0)
            {
                if(raw_node.u.info.node_type == DIRECTORY_NODE)
                {
                    BX_ASSERT(dir_node<BXFS_DIR_NODE_MAX+1);
                    bxfs_dir[dir_node].section = i;
                    bxfs_dir[dir_node].offset = offset;
                    bxfs_dir[dir_node].idx = raw_node.u.dir.node_idx;
                    bxfs_dir[dir_node].key = raw_node.key;
                    bxfs_dir[dir_node].parent_dir_offset = raw_node.parent_dir_idx;
                    bxfs_dir[dir_node].not_linked = 1;
                    bxfs_dir[dir_node].first_child_dir = INVALID_DIR_OFFSET;
                    bxfs_dir[dir_node].first_child_record = INVALID_RECORD_OFFSET;
                    if(bxfs_env.dir_idx_max< raw_node.u.dir.node_idx)
                    {
                        bxfs_env.dir_idx_max = raw_node.u.dir.node_idx;
                    }
                    ++dir_node;
                }else
                {
                    BX_ASSERT(record_node<BXFS_RECORD_NODE_MAX);
                    bxfs_record[record_node].data_length = raw_node.u.record.data_length;
                    bxfs_record[record_node].section = i;
                    bxfs_record[record_node].offset = offset;
                    bxfs_record[record_node].parent_dir_offset = raw_node.parent_dir_idx;
                    bxfs_record[record_node].key = raw_node.key;
                    ++record_node;
                }
            }
            offset += node_size;
        }while(offset < BXFS_SECTION_SIZE);
        if(section_head.not_tail)
        {
            bxfs_env.tail_section = i;
            bxfs_env.tail_available_offset = offset;
        }
    }

    bxfs_dir[0].first_child_record = INVALID_RECORD_OFFSET;
    if(all_section_empty)
    {
        section_head_program(0,sizeof(bxfs_section_head_t),true,true);
        bxfs_env.head_section = 0;
        bxfs_env.tail_section = 0;
        bxfs_env.head_valid_data_size = 0;
        bxfs_env.tail_available_offset = sizeof(bxfs_section_head_t);
        bxfs_env.dir_idx_max = 0;
    }else
    {
        bxfs_env.head_valid_data_size = section_valid_data_count(bxfs_env.head_section);
        if(dir_node > 2)
        {
            qsort(bxfs_dir+1,dir_node-1,sizeof(bxfs_dir_node_t),dir_parent_and_idx_cmp);
        }
        if(record_node > 1)
        {
            qsort(bxfs_record,record_node,sizeof(bxfs_record_node_t),record_parent_idx_cmp);
        }
        bxfs_dir[0].first_child_dir = dir_tree_build(bxfs_dir,1,dir_node,&bxfs_dir[0]);
        record_mount(bxfs_record, record_node, bxfs_dir,dir_node);
        bxfs_env.dir_idx_max = bxfs_dir[dir_node - 1].idx;
    }
}

static bool if_dir_exist(bxfs_dir_node_t *dir)
{
    if(((uint32_t)dir-(uint32_t)bxfs_dir)%sizeof(bxfs_dir_node_t))
    {
        return false;
    }
    if(dir < bxfs_dir || dir>= &bxfs_dir[BXFS_DIR_NODE_MAX+1])
    {
        return false;
    }
    if(dir == bxfs_dir)
    {
        return true;
    }
    if(dir->idx)
    {
        return true;
    }else
    {
        return false;
    }
}

static bxfs_dir_node_t *alloc_dir_node_buf()
{
    uint16_t i;
    for(i=1;i<BXFS_DIR_NODE_MAX+1;++i)
    {
        if(bxfs_dir[i].idx == 0)
        {
            return &bxfs_dir[i];
        }
    }
    return NULL;
}

static void section_head_update(uint8_t section,section_update_t type)
{
    bxfs_section_head_t section_head;
    *(uint16_t *)&section_head = 0xffff;
    if(type == NOT_TAIL_UPDTAE)
    {
        section_head.not_tail = 0;
    }else
    {
        section_head.head = 0;
    }
    bxfs_env.write(bxfs_env.base+section*BXFS_SECTION_SIZE,sizeof(bxfs_section_head_t),(uint8_t *)&section_head);
}

static uint8_t get_prev_section(uint8_t current)
{
    return current ? current - 1: bxfs_env.section_num - 1;
}

static void tail_section_move()
{
    section_head_program(bxfs_env.tail_section,bxfs_env.tail_available_offset,false, true);
    section_head_update(get_prev_section(bxfs_env.tail_section),NOT_TAIL_UPDTAE);
}

static uint8_t get_next_section(uint8_t current)
{
    return current + 1 == bxfs_env.section_num ? 0 : current + 1;
}

static bool data_program(uint8_t *data,uint16_t length)
{
    bxfs_env.free_bytes -= length;
    uint8_t section = bxfs_env.tail_section;
    uint16_t offset = bxfs_env.tail_available_offset;
    if(offset + length > BXFS_SECTION_SIZE)
    {
        bxfs_env.write(bxfs_env.base+section*BXFS_SECTION_SIZE + offset,BXFS_SECTION_SIZE-offset,data);
        uint8_t next_section = get_next_section(section);
        bxfs_env.write(bxfs_env.base+next_section*BXFS_SECTION_SIZE + sizeof(bxfs_section_head_t),length+offset-BXFS_SECTION_SIZE,&data[BXFS_SECTION_SIZE-offset]);
        bxfs_env.tail_section = next_section;
        bxfs_env.tail_available_offset = sizeof(bxfs_section_head_t)+length+offset-BXFS_SECTION_SIZE;
        return true;
    }else
    {
        bxfs_env.write(bxfs_env.base+section*BXFS_SECTION_SIZE + offset,length,data);
        bxfs_env.tail_available_offset += length;
        return false;
    }
}

static bool long_data_copy(uint32_t src,uint16_t length)
{
    uint16_t quotient = length / BXFS_TMP_BUF_SIZE;
    uint16_t remainder = length % BXFS_TMP_BUF_SIZE;
    uint16_t i;
    bool tail_move = false;
    for(i=0;i<quotient;++i)
    {
        bxfs_env.read(src,BXFS_TMP_BUF_SIZE,tmp_data_buf);
        if(data_program(tmp_data_buf,BXFS_TMP_BUF_SIZE))
        {
            tail_move = true;
        }
        src += BXFS_TMP_BUF_SIZE;
    }
    bxfs_env.read(src,remainder,tmp_data_buf);
    if(data_program(tmp_data_buf,remainder))
    {
        tail_move = true;
    }
    return tail_move;
}

static void node_copy(uint32_t src,uint8_t type)
{
    bxfs_node_storage_t node;
    bxfs_env.read(src,sizeof(bxfs_node_storage_t),(uint8_t *)&node);
    bool tail_move = false;
    if(data_program((uint8_t *)&node,sizeof(bxfs_node_storage_t)))
    {
        tail_move = true;
    }
    if(type == RECORD_NODE)
    {
        if(long_data_copy(src+sizeof(bxfs_node_storage_t),node.u.record.data_length))
        {
            tail_move = true;
        }
    }
    if(tail_move)
    {
        tail_section_move();
    }
}

static void bxfs_garbage_collection()
{
    BX_ASSERT(bxfs_env.head_section != bxfs_env.tail_section);
    uint16_t i;
    for(i=1;i<BXFS_DIR_NODE_MAX+1;++i)
    {
        if(bxfs_dir[i].idx && bxfs_dir[i].section==bxfs_env.head_section)
        {
            bxfs_dir[i].section = bxfs_env.tail_section;
            bxfs_dir[i].offset = bxfs_env.tail_available_offset;
            node_copy(GET_FLASH_ADDR(bxfs_dir[i].section,bxfs_dir[i].offset),DIRECTORY_NODE);
        }
    }
    for(i=0;i<BXFS_RECORD_NODE_MAX;++i)
    {
        if(bxfs_record[i].data_length && bxfs_record[i].section == bxfs_env.head_section)
        {
            bxfs_record[i].section = bxfs_env.tail_section;
            bxfs_record[i].offset = bxfs_env.tail_available_offset;
            node_copy(GET_FLASH_ADDR(bxfs_record[i].section,bxfs_record[i].offset),RECORD_NODE);
        }
    }
    bxfs_env.erase(GET_FLASH_ADDR(bxfs_env.head_section,0),Sector_Erase);
    bxfs_env.free_bytes += BXFS_ERASE_BLOCK_SIZE - sizeof(bxfs_section_head_t);
    bxfs_env.head_section = get_next_section(bxfs_env.head_section);
    bxfs_env.head_valid_data_size = section_valid_data_count(bxfs_env.head_section);
    section_head_update(bxfs_env.head_section,HEAD_UPDATE);
}

static void flash_data_update(bxfs_node_storage_t *node,uint8_t *data,uint16_t length)
{
    if(bxfs_env.free_bytes<bxfs_env.head_valid_data_size + length + sizeof(bxfs_node_storage_t))
    {
        bxfs_garbage_collection();
    }
    if(bxfs_env.tail_section == bxfs_env.head_section)
    {
        bxfs_env.head_valid_data_size += length + sizeof(bxfs_node_storage_t);
    }
    bool tail_move = false;
    if(data_program((uint8_t *)node, sizeof(bxfs_node_storage_t)))
    {
        tail_move = true;
    }
    if(length)
    {
        if(data_program(data,length))
        {
            tail_move = true;
        }
    }
    if(tail_move)
    {
        tail_section_move();
    }
}

static void new_node_info_set(node_storage_u *node,enum node_type_enum type)
{
    node->info.others = 0x1fff;
    node->info.used = 0;
    node->info.obsolete = 1;
    node->info.node_type = type;
}

static void dir_node_program(uint8_t node_idx,uint8_t node_key,uint8_t parent_dir_idx)
{
    bxfs_node_storage_t node_store;
    new_node_info_set(&node_store.u,DIRECTORY_NODE);
    node_store.key = node_key;
    node_store.parent_dir_idx = parent_dir_idx;
    node_store.u.dir.node_idx = node_idx;
    flash_data_update(&node_store,NULL,0);
}

static bxfs_dir_node_t *get_child_dir_by_key(bxfs_dir_node_t *parent,uint8_t dir_key)
{
    uint8_t idx;
    for(idx = parent->first_child_dir;idx!=INVALID_DIR_OFFSET;idx = bxfs_dir[idx].next_sibling)
    {
        if(bxfs_dir[idx].key == dir_key)
        {
            return &bxfs_dir[idx];
        }
    }
    return NULL;
}

uint8_t bxfs_mkdir(bxfs_dir_t *dir_to_make,bxfs_dir_t upper_dir,uint8_t dir_key)
{
    BX_ASSERT(upper_dir);
    if(if_dir_exist(upper_dir))
    {
        bxfs_dir_node_t *child = get_child_dir_by_key(upper_dir,dir_key);
        if(child)
        {
            *dir_to_make = child;
            return BXFS_DIR_KEY_ALREADY_EXISTED;
        }
        if(bxfs_env.dir_idx_max == 0xff)
        {
            return BXFS_DIR_IDX_OVERFLOW;
        }
        bxfs_dir_node_t *new_dir_node = alloc_dir_node_buf();
        if(new_dir_node == NULL)
        {
            return BXFS_INSUFFICIENT_NODE_BUF;
        }else
        {
            bxfs_dir_node_t *parent = upper_dir;
            new_dir_node->idx = ++bxfs_env.dir_idx_max;
            new_dir_node->key = dir_key;
            new_dir_node->next_sibling = parent->first_child_dir;
            parent->first_child_dir = new_dir_node - bxfs_dir;
            new_dir_node->first_child_dir = INVALID_DIR_OFFSET;
            new_dir_node->first_child_record = INVALID_RECORD_OFFSET;
            new_dir_node->section = bxfs_env.tail_section;
            new_dir_node->offset = bxfs_env.tail_available_offset;
            new_dir_node->parent_dir_offset = parent - bxfs_dir;
            dir_node_program(new_dir_node->idx,new_dir_node->key,parent->idx);
            *dir_to_make = new_dir_node;
            return BXFS_NO_ERROR;
        }  
    }else{
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

static bxfs_record_node_t *alloc_record_node_buf()
{
    uint16_t i;
    for(i=0;i<BXFS_RECORD_NODE_MAX;++i)
    {
        if(bxfs_record[i].data_length == 0)
        {
            return &bxfs_record[i];
        }
    }
    return NULL;
}

static void free_record_node_buf(bxfs_record_node_t *record)
{
    if(record)
    {
        record->data_length = 0;
        record->next_sibling = INVALID_RECORD_OFFSET;
    }
}

static bxfs_record_node_t *get_record_by_key(bxfs_dir_node_t *parent,uint8_t record_key,bxfs_record_node_t **prev_record)
{
    uint16_t record_buf_idx = parent->first_child_record;
    if(prev_record)
    {
        *prev_record = NULL;
    }
    while(record_buf_idx!=INVALID_RECORD_OFFSET)
    {
        if(bxfs_record[record_buf_idx].key == record_key)
        {
            return &bxfs_record[record_buf_idx];
        }
        if(prev_record)
        {
            *prev_record = &bxfs_record[record_buf_idx];
        }
        record_buf_idx = bxfs_record[record_buf_idx].next_sibling;
    }
    return NULL;
}

static void record_node_program(uint8_t record_key,uint8_t *data,uint16_t length,uint8_t parent_dir_idx)
{
    bxfs_node_storage_t node_store;
    new_node_info_set(&node_store.u,RECORD_NODE);
    node_store.key = record_key;
    node_store.parent_dir_idx = parent_dir_idx;
    node_store.u.record.data_length = length;
    flash_data_update(&node_store,data,length);
}

static void node_obsolete(uint8_t section,uint16_t offset)
{
    bxfs_node_storage_t node_store;
    *(uint16_t *)&node_store = 0xffff;
    node_store.u.info.obsolete = 0;
    bxfs_env.write(GET_FLASH_ADDR(section, offset),sizeof(node_store),(uint8_t *)&node_store);
}

static uint8_t bxfs_write_int(bxfs_dir_node_t *parent,uint8_t record_key,uint8_t *data,uint16_t length)
{
    bxfs_record_node_t *record = get_record_by_key(parent,record_key,NULL);
    if(record)
    {
        uint16_t old_offset = record->offset;
        uint8_t old_section =  record->section;
        record->data_length = length;
        record->offset = bxfs_env.tail_available_offset;
        record->section = bxfs_env.tail_section;
        record_node_program(record_key,data,length,parent->idx);
        node_obsolete(old_section,old_offset);
        return BXFS_NO_ERROR;
    }else
    {
        record = alloc_record_node_buf();
        if(record)
        {
            record->data_length = length;
            record->offset = bxfs_env.tail_available_offset;
            record->section = bxfs_env.tail_section;
            record->parent_dir_offset = parent - bxfs_dir;
            record->key = record_key;
            record->next_sibling = parent->first_child_record;
            parent->first_child_record = record - bxfs_record;
            record_node_program(record_key,data,length,parent->idx);
            return BXFS_NO_ERROR;
        }else
        {
            return BXFS_INSUFFICIENT_NODE_BUF;
        }
    }

}

uint8_t bxfs_write(bxfs_dir_t dir,uint8_t record_key,uint8_t *data,uint16_t length)
{
    BX_ASSERT(dir);
    BX_ASSERT(data);
    BX_ASSERT(length);
    if(if_dir_exist(dir))
    {
        return bxfs_write_int(dir,record_key,data,length);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

static void record_data_read(uint8_t section,uint16_t offset,uint8_t *data,uint16_t length)
{
    if(offset + sizeof(bxfs_node_storage_t) > BXFS_SECTION_SIZE)
    {
        bxfs_env.read(GET_FLASH_ADDR(get_next_section(section), offset + sizeof(bxfs_node_storage_t) - BXFS_SECTION_SIZE + sizeof(bxfs_section_head_t)),
            length,data);
    }else if(offset + sizeof(bxfs_node_storage_t) + length > BXFS_SECTION_SIZE)
    {
        uint16_t length1 = BXFS_SECTION_SIZE - offset - sizeof(bxfs_node_storage_t);
        bxfs_env.read(GET_FLASH_ADDR(section,offset + sizeof(bxfs_node_storage_t)),length1,data);
        bxfs_env.read(GET_FLASH_ADDR(get_next_section(section), sizeof(bxfs_section_head_t)),length - length1,&data[length1]);
    }else
    {
        bxfs_env.read(GET_FLASH_ADDR(section,offset + sizeof(bxfs_node_storage_t)),length,data);
    }
}

static uint8_t bxfs_read_int(bxfs_dir_node_t *parent,uint8_t record_key,uint8_t *data,uint16_t *length_ptr)
{
    bxfs_record_node_t *record = get_record_by_key(parent,record_key,NULL);
    if(record)
    {
        if(record->data_length<=*length_ptr)
        {
            record_data_read(record->section,record->offset,data,record->data_length);
            *length_ptr = record->data_length;
            return BXFS_NO_ERROR;
        }else
        {
            return BXFS_INSUFFICIENT_RECORD_DATA_BUF;
        }
    }else
    {
        return BXFS_RECORD_KEY_NOT_FOUND;
    }
}

uint8_t bxfs_read(bxfs_dir_t dir,uint8_t record_key,uint8_t *data,uint16_t *length_ptr)
{
    BX_ASSERT(dir);
    if(if_dir_exist(dir))
    {
        return bxfs_read_int(dir, record_key,data,length_ptr);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

static uint8_t bxfs_del_record_int(bxfs_dir_node_t *dir,uint8_t record_key)
{
    bxfs_record_node_t *prev_record;
    bxfs_record_node_t *record = get_record_by_key(dir,record_key,&prev_record);
    if(record)
    {
        node_obsolete(record->section,record->offset);
        if(prev_record)
        {
            prev_record->next_sibling = record->next_sibling;
        }else
        {
            bxfs_dir_node_t *parent = dir;
            parent->first_child_record = record->next_sibling;
        }
        free_record_node_buf(record);
        return BXFS_NO_ERROR;
    }else
    {
        return BXFS_RECORD_KEY_NOT_FOUND;
    }
}

uint8_t bxfs_del_record(bxfs_dir_t dir,uint8_t record_key)
{
    BX_ASSERT(dir);
    if(if_dir_exist(dir))
    {
        return bxfs_del_record_int(dir,record_key);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

static void free_dir_node_buf(bxfs_dir_node_t *dir)
{
    if(dir)
    {
        dir->idx = 0;
        dir->first_child_dir = INVALID_DIR_OFFSET;
        dir->first_child_record = INVALID_RECORD_OFFSET;
        dir->next_sibling = INVALID_DIR_OFFSET;
    }
}

static uint8_t bxfs_force_del_dir(bxfs_dir_node_t *dir)
{
    uint16_t record_buf_idx = dir->first_child_record;
    while(record_buf_idx!=INVALID_RECORD_OFFSET)
    {
        uint16_t next_sibling = bxfs_record[record_buf_idx].next_sibling;
        node_obsolete(bxfs_record[record_buf_idx].section,bxfs_record[record_buf_idx].offset);
        free_record_node_buf(&bxfs_record[record_buf_idx]);
        record_buf_idx = next_sibling;
    }
    uint8_t dir_buf_idx = dir->first_child_dir;
    while(dir_buf_idx!=INVALID_DIR_OFFSET)
    {
        dir_buf_idx = bxfs_force_del_dir(&bxfs_dir[dir_buf_idx]);
    }
    uint8_t next_sibling = dir->next_sibling;
    node_obsolete(dir->section,dir->offset);
    free_dir_node_buf(dir);
    return next_sibling;
}

static bxfs_dir_node_t *get_parent_dir_by_dir(bxfs_dir_node_t *dir)
{
    return &bxfs_dir[dir->parent_dir_offset];
}

static bxfs_dir_node_t * get_prev_dir_by_dir(bxfs_dir_node_t *parent,bxfs_dir_node_t *dir)
{
    uint8_t idx;
    bxfs_dir_node_t *prev = NULL;
    if(NULL == parent)
    {
        return NULL;
    }
    for(idx = parent->first_child_dir;idx!=INVALID_DIR_OFFSET;idx = bxfs_dir[idx].next_sibling)
    {
        if((&bxfs_dir[idx]) == dir)
        {
           return prev;
        }
        prev = &bxfs_dir[idx];
    }
    return NULL;
}


static bool if_dir_empty(bxfs_dir_node_t *dir)
{
    return dir->first_child_dir!=INVALID_DIR_OFFSET || dir->first_child_record!=INVALID_RECORD_OFFSET ? false : true;
}



uint8_t bxfs_del_dir(bxfs_dir_t dir,bool force)
{
    BX_ASSERT(dir);
    if(if_dir_exist(dir))
    {
        bxfs_dir_node_t *parent = get_parent_dir_by_dir(dir);
        bxfs_dir_node_t *prev =  get_prev_dir_by_dir(parent, dir);
        if(force)
        {
            uint8_t next_sibling = bxfs_force_del_dir(dir);
            if(prev)
            {
                prev->next_sibling = next_sibling;
            }else
            {
                parent->first_child_dir = next_sibling;
            }
            return BXFS_NO_ERROR;
        }else
        {
            if(if_dir_empty(dir) == false)
            {
                return BXFS_DIR_NOT_EMPTY;
            }else
            {
                free_dir_node_buf(dir);
                return BXFS_NO_ERROR;
            }
        }
    }else
    {
        return BXFS_DIR_NOT_EXISTED;
    }
}

bxfs_dir_node_t *hierarchy_dir_get(va_list *key_list,uint8_t depth)
{
    uint8_t i;
    bxfs_dir_node_t *dir = bxfs_dir; 
    for(i=0;i<depth;++i)
    {
        uint8_t dir_key = va_arg(*key_list,uint32_t);
        dir = get_child_dir_by_key(dir,dir_key);
        if(dir == NULL)
        {
            break;
        }
    }
    return dir;
}

uint8_t bxfs_hierarchy_write(bxfs_hierarchy_rw_t *param,...)
{
    bxfs_dir_node_t *dir;
    va_list key_list;
    va_start(key_list,param);
    dir = hierarchy_dir_get(&key_list, param->dir_depth);
    va_end(key_list);
    if(dir)
    {
        return bxfs_write_int(dir,param->record_key,param->data, param->length);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

uint8_t bxfs_hierarchy_read(bxfs_hierarchy_rw_t *param,...)
{
    bxfs_dir_node_t *dir;
    va_list key_list;
    va_start(key_list,param);
    dir = hierarchy_dir_get(&key_list, param->dir_depth);
    va_end(key_list);
    if(dir)
    {
        return bxfs_read_int(dir, param->record_key,param->data, &param->length);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

static uint8_t bxfs_record_list_get_int(bxfs_dir_node_t *dir,uint8_t *num,uint8_t *list)
{
    uint8_t i = 0;
    uint16_t record_buf_idx;
    bxfs_dir_node_t *parent = dir;
    for(record_buf_idx=parent->first_child_record;record_buf_idx!=INVALID_RECORD_OFFSET;record_buf_idx = bxfs_record[record_buf_idx].next_sibling)
    {
        list[i++] = bxfs_record[record_buf_idx].key;
        if(i == *num)
        {
            return BXFS_TMP_BUF_OVERFLOW;
        }
    }
    *num = i;
    return BXFS_NO_ERROR;
}

uint8_t bxfs_record_list_get(bxfs_dir_t dir,uint8_t *num,uint8_t *list)
{
    BX_ASSERT(dir);
    if(if_dir_exist(dir))
    {
        return bxfs_record_list_get_int(dir,num,list);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

uint8_t bxfs_hierarchy_record_list_get(bxfs_hierarchy_record_list_t *param,...)
{
    bxfs_dir_node_t *dir;
    va_list key_list;
    va_start(key_list,param);
    dir = hierarchy_dir_get(&key_list, param->dir_depth);
    va_end(key_list);
    if(dir)
    {
        return bxfs_record_list_get_int(dir,&param->num,param->list);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }
}

uint8_t bxfs_hierarchy_del_record(bxfs_hierarchy_del_record_t *param,...)
{
    bxfs_dir_node_t *dir;
    va_list key_list;
    va_start(key_list,param);
    dir = hierarchy_dir_get(&key_list, param->dir_depth);
    va_end(key_list);
    if(dir)
    {
        return bxfs_del_record_int(dir,param->record_key);
    }else
    {
        return BXFS_PARENT_DIR_NOT_FOUND;
    }

}

