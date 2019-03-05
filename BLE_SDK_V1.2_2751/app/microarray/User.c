#include "User.h"
#include "Front.h"
#include <string.h>
#include "m_flash.h"
typedef struct{
    u32 password_lo;                      //开锁密码低8位
    u32 password_hi;                      //开锁密码高4位
	  u32 password_num_len;
    VerifyMode_t verify_mode;                    //开锁模式
}UserData_t;                                     //保存到Flash的用户设置数据类型

#define FLASH_ERASE_SIZE     4096                //Flash擦除页大小
#define MAX_USER_DATA_NUM    (FLASH_ERASE_SIZE / sizeof(UserData_t))//一个擦除页中可以保存的用户数据个数

//===========================Flash仿真接口函数==================================//
//Flash分为3页,分别保存用户设置数据，RFID，指纹
//extern ParamAwake_t g_front_param;//唤醒状态的参数结构;

//Flash初始化
void FlashInit(void)
{
  
}

//ADMIN_BASEADDR


//从Flash中读出一个管理员用户数据结构
void FlashReadAdminData(u32 idx, UserData_t *p_data)
{
		flash_read(ADMIN_BASEADDR + idx * sizeof(UserData_t), sizeof(UserData_t), (uint8_t * )p_data);
	/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(UserData_t));
        g_flash.read((char *)p_data, sizeof(UserData_t));
    }*/
}
//向Flash中写入一个管理员用户数据结构
void FlashWriteAdminData(u32 idx, UserData_t *p_data)
{
	 eflash_write(ADMIN_BASEADDR + idx * sizeof(UserData_t), sizeof(UserData_t), (uint32_t *)p_data);
	/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(UserData_t));
        g_flash.write((char *)p_data, sizeof(UserData_t));
    }*/
}
//擦除管理员用户数据所在页
void FlashEraseAdminData(void)
{
	 eflash_page_erase(ADMIN_BASEADDR);
	/*
    if (g_flash.isOpen()){
        u32 fill = -1;
        for(u32 i = 0; i < FLASH_ERASE_SIZE / sizeof(u32); i++){
            g_flash.write((const char *)&fill, sizeof(u32));
        }
    }*/
}



//从Flash中读出一个用户数据结构
void FlashReadUserData(u32 idx, UserData_t *p_data)
{
		flash_read(KEY_MAIN_BASEADDR + idx * sizeof(UserData_t), sizeof(UserData_t), (uint8_t * )p_data);
	/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(UserData_t));
        g_flash.read((char *)p_data, sizeof(UserData_t));
    }*/
}
//向Flash中写入一个用户数据结构
void FlashWriteUserData(u32 idx, UserData_t *p_data)
{
	 eflash_write(KEY_MAIN_BASEADDR + idx * sizeof(UserData_t), sizeof(UserData_t), (uint32_t *)p_data);
	/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(UserData_t));
        g_flash.write((char *)p_data, sizeof(UserData_t));
    }*/
}
//擦除用户数据所在页
void FlashEraseUserData(void)
{
	 eflash_page_erase(KEY_MAIN_BASEADDR);
	/*
    if (g_flash.isOpen()){
        u32 fill = -1;
        for(u32 i = 0; i < FLASH_ERASE_SIZE / sizeof(u32); i++){
            g_flash.write((const char *)&fill, sizeof(u32));
        }
    }*/
}

//从Flash中取出一个RFID
u32 FlashReadRfid(u32 idx)
{
    u32 rfid = -1;
	  flash_read(RFID_MAIN_BASEADDR + idx * sizeof(u32), sizeof(u32), (uint8_t * )&rfid);
/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(u32) + FLASH_ERASE_SIZE);
        g_flash.read((char *)&rfid, sizeof(u32));
    }*/
    return rfid;
}
//向Flash中写入一个RFID
void FlashWriteRfid(u32 idx, u32 rfid)
{
		eflash_program(RFID_MAIN_BASEADDR + idx * sizeof(u32), rfid);
	/*
    if (g_flash.isOpen()){
        g_flash.seek(idx * sizeof(u32) + FLASH_ERASE_SIZE);
        g_flash.write((char *)&rfid, sizeof(u32));
    }*/
}
//擦除RFID数据所在页
void FlashEraseRFID(void)
{
	 eflash_page_erase(RFID_MAIN_BASEADDR);
		/*
    if (g_flash.isOpen()){
        g_flash.seek(FLASH_ERASE_SIZE);
        u32 fill = -1;
        for(u32 i = 0; i < FLASH_ERASE_SIZE / sizeof(u32); i++){
            g_flash.write((const char *)&fill, sizeof(u32));
        }
    }*/
}

//===========================用户管理接口=======================================//
volatile UserConfig_t g_user_config;                      //全局用户设置

//用户管理初始化
void UserInit(void)
{
    u32 index,RFID;
    volatile UserData_t user_data;
	
    //TODO:将Flash仿真移到hardware目录里面去，并修改接口
    FlashInit();
    g_user_config.admin_write_index = 0;
	  //读出最新的管理员用户设置
		for (index = 0; index < MAX_USER_DATA_NUM; index++){
					//读出最后一个密码不为全F的结构,就是最新的数据
					FlashReadAdminData(index, (UserData_t *)&user_data);
					if (user_data.password_num_len == -1)
					{
							break;
					}
			}

		g_user_config.admin_write_index = index;

		if (index > 0){
        //读出最新的设置
        FlashReadAdminData(index - 1, (UserData_t *)&user_data);
        g_user_config.password_admin_lo = user_data.password_lo;
        g_user_config.password_admin_hi = user_data.password_hi;
        g_user_config.verify_mode = user_data.verify_mode;
				g_user_config.password_num_len = user_data.password_num_len;
    }else{
        //没有最新的设置就使用默认设置
        g_user_config.password_admin_lo = 0xFF123456;
        g_user_config.password_admin_hi = 0xFFFFFFFF;
			  g_user_config.password_num_len = 6;
        g_user_config.verify_mode = VERIFY_MODE_ADMIN_USER;
    }
		
		
    //读出最新的用户设置
    for (index = 0; index < MAX_USER_DATA_NUM; index++){
        //读出最后一个密码不为全F的结构,就是最新的数据
        FlashReadUserData(index, (UserData_t *)&user_data);
        if (user_data.password_num_len == -1)
				{
            break;
				}
    }
    g_user_config.config_write_index = index;
#if 0
    if (index > 0){
        //读出最新的设置
        FlashReadUserData(index - 1, (UserData_t *)&user_data);
        g_user_config.password_unlock_lo = user_data.password_lo;
        g_user_config.password_unlock_hi = user_data.password_hi;
        g_user_config.verify_mode = user_data.verify_mode;
    }else{
        //没有最新的设置就使用默认设置
        g_user_config.password_admin_lo = 0xFF123456;
        g_user_config.password_admin_hi = 0xFFFFFFFF;
        g_user_config.password_unlock_lo = 0xFF123456;
        g_user_config.password_unlock_hi = 0xFFFFFFFF;
			  g_user_config.password_num_len = 6;
        g_user_config.verify_mode = VERIFY_MODE_ADMIN_USER;
			 
			  ChangeNumtoString();
			 // CheckKey(testbuf,100,(u8 *)g_user_config.passwordnum,6);
			
			  user_data.password_lo = 0xFF123456;
        user_data.password_hi = 0xFFFFFFFF;
			  user_data.password_num_len = 6;
			  user_data.verify_mode = VERIFY_MODE_NORMAL_USER;
    }
#endif

    //计算已有的RFID数
    for(index = 0; index < MAX_RFID_NUM; index++){
        if (FlashReadRfid(index) == -1){
            break;
        }
    }
		g_user_config.rfid_write_index = index;

#if 0	
		//for test  flash
		for (index = 0; index < MAX_USER_DATA_NUM; index++){
        //读出最后一个密码不为全F的结构,就是最新的数据
        FlashWriteUserData(index, &user_data);
    }
		
		RFID = 0x55aaa55a;
		for(index = 0; index < 5; index++)
		{
        FlashWriteRfid(index,RFID);

    }
		
		//FlashEraseUserData();
		//FlashEraseRFID();
 #endif
}


//保存用户设置
void AdminSaveConfig(void)
{
    UserData_t user_data;

    //如果用户设置已经满了,就先擦除此页
    if (g_user_config.admin_write_index >= MAX_USER_DATA_NUM)
		{
        FlashEraseAdminData();
        g_user_config.admin_write_index = 0;
    }
    //写入用户设置到Flash
   
    
    user_data.verify_mode = g_user_config.verify_mode;
		if(user_data.verify_mode)//管理员
		{
				 user_data.password_lo = g_user_config.password_admin_lo;
         user_data.password_hi = g_user_config.password_admin_hi;
		}
		else//error
		{
				debugPrint(" \r\n add admin verify mode error  \r\n");
		}
		user_data.password_num_len = g_user_config.password_num_len;
    FlashWriteAdminData(g_user_config.admin_write_index++, &user_data);
}
//保存用户设置
void UserSaveConfig(void)
{
    UserData_t user_data;

    //如果用户设置已经满了,就先擦除此页
    if (g_user_config.config_write_index >= MAX_USER_DATA_NUM)
		{
        FlashEraseUserData();
        g_user_config.config_write_index = 0;
    }
    //写入用户设置到Flash
   
    
    user_data.verify_mode = g_user_config.verify_mode;
		if(user_data.verify_mode)//error
		{
				  debugPrint(" \r\n add user verify mode error  \r\n");
		}
		else//普通用户
		{
				user_data.password_lo = g_user_config.password_unlock_lo;
				user_data.password_hi = g_user_config.password_unlock_hi;
		}
		user_data.password_num_len = g_user_config.password_num_len;
    FlashWriteUserData(g_user_config.config_write_index++, &user_data);
}
//查找卡号,返回大于等于0的索引,或者-1表示未找到
s32  UserFindRfid(u32 rfid)
{
    u32 index;
    u32 tmp_id;
    for(index = 0; index < MAX_RFID_NUM; index++){
        tmp_id = FlashReadRfid(index);
			  if(tmp_id == -1){
					 return -1;
				}
        if (tmp_id == rfid){
            return index;
        }
    }
    return -1;
}
//添加一个卡号
s32 UserAddRfid(u32 rfid)
{
    if(g_user_config.rfid_write_index >= MAX_RFID_NUM){
        return -1;
    }
    FlashWriteRfid(g_user_config.rfid_write_index++, rfid);
    return 0;
}
//删除所有卡号
void UserDeleteAllRfid(void)
{
    FlashEraseRFID();
    g_user_config.rfid_write_index = 0;
}
void UserDeleteAllKeyUser(void)
{
	 FlashEraseUserData();
   g_user_config.config_write_index = 0;
	
}
void UserDeleteAllAdminUser(void)
{
	 FlashEraseAdminData();
   g_user_config.admin_write_index = 0;
}


u8  UserFindAdmin(void)
{
	u32 index;
	uint8_t findresult;
	UserData_t user_data;
	//ChangeNumtoString();
	for (index = 0; index < MAX_USER_DATA_NUM; index++)
	{
			//读出最后一个密码不为全F的结构,就是最新的数据
			FlashReadAdminData(index, (UserData_t *)&user_data);
			if (user_data.password_num_len == -1)
			{
					return 0;
			}
			g_user_config.password_unlock_hi = user_data.password_hi;
      g_user_config.password_unlock_lo = user_data.password_lo;	
			g_user_config.password_num_len = user_data.password_num_len;
      ChangeNumtoString();			
			findresult = CheckKey(g_front_param.password_buff,g_front_param.password_len,(u8 *)g_user_config.passwordnum,g_user_config.password_num_len);
			if(findresult)
			{
				  return 1;
			}
	}
	return 0;
}

u8 UserFindKeyUser(void)
{
	u32 index;
	uint8_t findresult;
	UserData_t user_data;
	//读出最新的用户设置
	for (index = 0; index < MAX_USER_DATA_NUM; index++){
			//读出最后一个密码不为全F的结构,就是最新的数据
			FlashReadUserData(index, (UserData_t *)&user_data);
			if (user_data.password_num_len == -1)
			{
					return 0;
			}
			g_user_config.password_unlock_hi = user_data.password_hi;
      g_user_config.password_unlock_lo = user_data.password_lo;	
			g_user_config.password_num_len = user_data.password_num_len;			
      ChangeNumtoString();			
			findresult = CheckKey(g_front_param.password_buff,g_front_param.password_len,(u8 *)g_user_config.passwordnum,g_user_config.password_num_len);
			if(findresult)
			{
				  return 1;
			}
			
	}
	
	
}

