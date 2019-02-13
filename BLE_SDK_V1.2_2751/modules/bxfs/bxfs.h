#ifndef BXFS_H_
#define BXFS_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#define BXFS_DIR_NODE_MAX       40
#define BXFS_RECORD_NODE_MAX    50
#define BXFS_SECTION_SIZE (0x1000)
#define BXFS_ERASE_BLOCK_SIZE (BXFS_SECTION_SIZE)
#define BXFS_TMP_BUF_SIZE (128)
#define ROOT_DIR ((void *)root_dir_ptr)

enum bxfs_error_code
{
    BXFS_NO_ERROR,
    BXFS_INSUFFICIENT_NODE_BUF,
    BXFS_INSUFFICIENT_RECORD_DATA_BUF,
    BXFS_DIR_IDX_OVERFLOW,
    BXFS_PARENT_DIR_NOT_FOUND,
    BXFS_DIR_NOT_EXISTED,
    BXFS_DIR_NOT_EMPTY,
    BXFS_DIR_KEY_ALREADY_EXISTED,
    BXFS_RECORD_KEY_ALREADY_EXISTED,
    BXFS_RECORD_KEY_NOT_FOUND,
    BXFS_TMP_BUF_OVERFLOW,
};

enum node_type_enum
{
    RECORD_NODE = 0,
    DIRECTORY_NODE,
};
    
typedef void * bxfs_dir_t;

typedef struct
{
    uint8_t *data;
    uint16_t length;
    uint8_t record_key;
    uint8_t dir_depth;
}bxfs_hierarchy_rw_t;

typedef struct
{
    uint8_t num;
    uint8_t *list;
    uint8_t dir_depth;
}bxfs_hierarchy_record_list_t;

typedef struct
{
    uint8_t record_key;
    uint8_t dir_depth;
}bxfs_hierarchy_del_record_t;

struct bxfs_dir_node_s;
extern void const *root_dir_ptr;

void bxfs_init(uint32_t base,uint16_t section_num);

uint8_t bxfs_mkdir(bxfs_dir_t *dir_to_make,bxfs_dir_t upper_dir,uint8_t dir_key);

uint8_t bxfs_write(bxfs_dir_t dir,uint8_t record_key,uint8_t *data,uint16_t length);

uint8_t bxfs_read(bxfs_dir_t dir,uint8_t record_key,uint8_t *data,uint16_t *length_ptr);

uint8_t bxfs_del_record(bxfs_dir_t dir,uint8_t record_key);

uint8_t bxfs_del_dir(bxfs_dir_t dir,bool force);

uint8_t bxfs_hierarchy_write(bxfs_hierarchy_rw_t *param,...);

uint8_t bxfs_hierarchy_read(bxfs_hierarchy_rw_t *param,...);

uint8_t bxfs_record_list_get(bxfs_dir_t dir,uint8_t *num,uint8_t *list);

uint8_t bxfs_hierarchy_record_list_get(bxfs_hierarchy_record_list_t *param,...);

uint8_t bxfs_hierarchy_del_record(bxfs_hierarchy_del_record_t *param,...);


#endif
