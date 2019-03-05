#ifndef USER_H
#define USER_H
#include "MaTypes.h"

#define MAX_RFID_NUM         200                 //最大RFID数

typedef enum{
    VERIFY_MODE_NORMAL_USER,                     //一般用户
    VERIFY_MODE_ADMIN_USER,                      //管理员用户
}VerifyMode_t;                                   //验证模式类型

typedef struct{
    u32 password_admin_lo;                       //管理员密码低8位
    u32 password_admin_hi;                       //管理员密码高4位
    u32 password_unlock_lo;                      //开锁密码低8位
    u32 password_unlock_hi;                      //开锁密码高4位
    VerifyMode_t verify_mode;                    //开锁模式
 
	  u32 admin_write_index;                       //写入管理员设置的位置
	  
    u32 config_write_index;                      //写入新用户设置的位置
    u32 rfid_write_index;                        //写入新RFID的位置,同时也是查找RFID的结束位置
	  u32 password_num_len;
		u8  passwordnum[16];
}UserConfig_t;                                   //用户设置类型

//===========================用户管理接口=======================================//
extern volatile UserConfig_t g_user_config;               //全局用户设置

void UserInit(void);                             //用户管理初始化
void AdminSaveConfig(void);                      //保存管理员设置
void UserSaveConfig(void);                       //保存用户设置
s32  UserFindRfid(u32 rfid);                     //查找卡号,返回大于等于0的索引,或者-1表示未找到
s32  UserAddRfid(u32 rfid);                      //添加一个卡号,返回0表示成功,或者-1表示RFID满
void UserDeleteAllRfid(void);                    //删除所有卡号
void UserDeleteAllKeyUser(void);
void UserDeleteAllAdminUser(void);


u8 UserFindKeyUser(void);
u8  UserFindAdmin(void);

#endif // USER_H
