#include "bxfs.h"
#define RECORD_KEY1  1
#define RECORD1 "abcd"
#define RECORD_KEY2  2
#define RECORD2 "efgh"
#define RECORD_KEY3  3
#define RECORD3 "1234567890"


uint8_t tmp[20];
void bxfs_test()
{
    bxfs_dir_t dir0,dir1,dir2;
    uint8_t rslt = bxfs_mkdir(&dir0,ROOT_DIR,5);
    rslt = bxfs_mkdir(&dir1,dir0,5);
    rslt = bxfs_mkdir(&dir2,dir1,5);
    rslt = bxfs_write(dir2,RECORD_KEY1,RECORD1,sizeof(RECORD1));
    rslt = bxfs_write(dir2,RECORD_KEY2,RECORD2,sizeof(RECORD2));
    rslt = bxfs_write(dir1,RECORD_KEY3,RECORD3,sizeof(RECORD3));
    uint16_t length = 20;
    rslt = bxfs_read(dir2,RECORD_KEY1,tmp,&length);
    length = 20;
    rslt = bxfs_read(dir1,RECORD_KEY3,tmp,&length);
    rslt = bxfs_read(dir2,RECORD_KEY2,tmp,&length);
    uint8_t list_length;
    rslt = bxfs_record_list_get(dir2,&list_length,tmp);
    rslt = bxfs_del_record(dir1,RECORD_KEY3);
    rslt = bxfs_del_dir(dir0,false);
    rslt = bxfs_del_dir(dir0,true);
}

