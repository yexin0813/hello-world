#include "Front.h"




#define FRONT_OVERTIME       10000                //前板状态超时

typedef void (*FrontState_t)(void);              //前板状态类型
FrontState_t g_front_state;                      //前板状态
u32 g_front_inited;                              //当前状态是否初始化过
u32 g_front_stamp;                               //前板超时时间戳
u32 g_front_error_num;                           //错误密码/指纹/卡号的次数

u32 g_config_stamp;                              //设置键时间戳
u32 g_config_last;                               //设置键的最后状态
u32 g_config_triggered;                          //设置键是否已经触发

    

ParamAwake_t g_front_param;//唤醒状态的参数结构

//===========================前板状态机局部函数===================================//


//===========================前板状态机状态函数===================================//
static void FrontSleep(void);                    //睡眠状态
static void FrontAwake(void);                    //唤醒状态,可以验证开锁密码/指纹/卡
static void FrontUnlock(void);                   //开锁状态
static void FrontAlarm(void);                    //锁定状态
static void FrontVerifyAdmin(void);              //验证管理员状态
static void FrontMenuTop(void);                  //顶层设置菜单状态
static void FrontMenuAddUser(void);              //添加用户
static void FrontMenuDeleteUser(void);           //删除用户
static void FrontAddAdmin(void);                 //添加管理员
static void FrontAddNormalUser(void);            //添加一般用户
static void FrontDeleteAllUser(void);            //删除所有用户
static void FrontDeleteNormalUser(void);         //删除一般用户
static void FrontAddNormalKeyUser(void);         //添加一般密码用户
static void FrontAddNormalFingerUser(void);      //添加一般指纹用户
static void FrontAddNormalRfidUser(void);        //添加一般卡用户




void ChangeNumtoString(void)
{
	  uint8_t cnt;
		uint8_t numlen = g_user_config.password_num_len;
	  for(cnt = 0;cnt < numlen;cnt ++)
		{
				if(cnt < 8)
				{
					g_user_config.passwordnum[numlen - 1 - cnt] = (g_user_config.password_unlock_lo >>(4*cnt)&0xF);
				}
				else
				{
					g_user_config.passwordnum[numlen - 1 - cnt] = (g_user_config.password_unlock_hi >>(4*(cnt -8))&0xF);
				}
		}
		return;
}
// 0 : fail
// 1 : pass
uint8_t CheckKey(u8 * checkbuffer,u8 buflen,u8 * password,u8 passwordlen)
{
		uint8_t result,cnt_t,offset;
		result = 0;
	  if(buflen < passwordlen)
		{
				return 0;
		}
		offset  = buflen - passwordlen;
		for(cnt_t = 0;cnt_t <=offset;cnt_t ++)
		{
			if(memcmp((const void *)(checkbuffer + cnt_t),(const void *)password,passwordlen)== 0)
			{
				result = 1;
				break;
			}
		}
		return result;
}
//睡眠状态：遇到滑盖Touch就唤醒
static void FrontSleep(void)
{
    if(g_front_inited)
		{
        if (FrontGetKeyTouch()||FrontGetFingerTouch())
				{
					  //g_hardware_input.key_touch = 0;
					  g_hardware_input.key = KEY_LED_NONE;
            g_front_state = FrontAwake;
            g_front_inited = 0;
					  
        }

    }
		else
		{
        ScreenClear();
        g_front_error_num = 0;

        g_front_inited = 1;
			  debugPrint(" Front Sleep\n");
			  g_hardware_input.key_touch = 0;
			  g_hardware_input.finger_touch = 0;
        //qDebug() << "FrontSleep";
    }
}

static u32 FrontGetStamp(void)
{
	return  systemTick();
}
static u32 FrontCalcElapsed(u32 tick)
{
	  uint32_t temptime;
		temptime = systemTick();
	  if(temptime >= g_front_stamp)
		{
			 temptime =  temptime - g_front_stamp;
		}
		else
		{
			 temptime = 0xFFFFFFFF - g_front_stamp + temptime;
		}
		return temptime;
}


//唤醒状态：可以输入开锁密码/按下手指/刷RFID开锁
static void FrontAwake(void)
{
    ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    s32 id, idx;
    KeyLed_t key;
    if(g_front_inited)
		{
        //按键输入
				key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_HASH)
						{            //#键
                if (param->password_len > 0)
								{  
										if(UserFindAdmin()||UserFindKeyUser())
										{
											  g_front_state = FrontUnlock;
											  g_front_inited = 0;
											
										}
										else
										{
											  //非法输入
											  g_front_state = FrontAlarm;
											  g_front_inited = 0;
										}
										 memset(param->password_buff,0,256);
										 param->password_len = 0;
							}
							else
							{
								  if(g_user_config.admin_write_index == 0)
									{
										 g_front_state = FrontMenuTop;
										 g_front_inited = 0;
									}
									else
									{
											g_front_state = FrontVerifyAdmin;
										  g_front_inited = 0;
									}
							}
								
            }
					  else if (key == KEY_LED_STAR)
						{   //*键
                //清除输入
							  memset(param->password_buff,0,256);
							  param->password_len = 0;
							
							 debugPrint(" \r\n clear input \r\n");
                //param->password_lo = 0xFFFFFFFF;
                //param->password_hi = 0xFFFFFFFF;
            }
						else
						{                               //数字键
                //输入一位
							  if(g_user_config.admin_write_index == 0)
								{
										g_front_state = FrontUnlock;
										g_front_inited = 0;
								}
								else
								{
										param->password_buff[param->password_len] = key;
										param->password_len ++;
										debugPrint(" %d",key);
								}
            }
        }
				if(FrontGetFingerTouch() == 1)//finger pressed
				{
						if(g_user_config.admin_write_index == 0)
						{
								g_front_state = FrontUnlock;
								g_front_inited = 0;
						}
						else
						{
							//匹配
							
						}
				}
				else
        //卡号输入
        {
            id = FrontGetRfid();
            if (id >= 0)
						{
                g_front_stamp = FrontGetStamp(); //更新时间戳
                idx = UserFindRfid(id);
                if (idx >= 0)
								{                   //开锁
                    g_front_state = FrontUnlock;
                    g_front_inited = 0;
                }
								else
								{                           //非法输入
                    g_front_state = FrontAlarm;
                    g_front_inited = 0;
                }
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
					  
        }
    }
		else
		{
        ScreenShowLogo();
        VoicePlay(VOICE_WELCOME);

        param->key_last = KEY_LED_NONE;
			  memset(param->password_buff,0,256);
				param->password_len = 0;
        //param->password_lo = 0xFFFFFFFF;
        //param->password_hi = 0xFFFFFFFF;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  //debugPrint(" Front Awake\r\n");
			  debugPrint(" *********************************\r\n");
			  debugPrint(" Welcome   \r\n");
			  debugPrint(" *********************************\r\n");
        //qDebug() << "FrontAwake";
    }
}
//开锁状态
static void FrontUnlock(void)
{
    if(g_front_inited)
		{

        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_error_num = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" Front Ulock\n");
        //qDebug() << "FrontUnlock";
    }
}
//锁定状态
static void FrontAlarm(void)
{
	   KeyLed_t key;
    if(g_front_inited)
		{
						   //
			  key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            g_front_state = FrontAwake;
            g_front_inited = 0;
        }
			  
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            if (g_front_error_num >= 5){

            }else{
                g_front_state = FrontSleep;
                g_front_inited = 0;
            }
        }
    }
		else
		{
        g_front_error_num++;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" Password Error\n");
        //qDebug() << "FrontAlarm";
    }
}
//验证管理员状态
static void FrontVerifyAdmin(void)
{
    ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    KeyLed_t key;
    if(g_front_inited)
		{
        //按键输入
        key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_HASH)
						{            //#键
                if (param->password_len > 0 )
								{   
									  if(UserFindAdmin())
										{
											  g_front_state = FrontMenuTop;
											  g_front_inited = 0;
											
										}
										else
										{
											  //非法输入
											  g_front_state = FrontAlarm;
											  g_front_inited = 0;
										}

                }
								memset(param->password_buff,0,256);
								param->password_len = 0;
            }
						else if (key == KEY_LED_STAR)
						{      //*键
                //清除输入
							  if(param->password_len > 0)
								{
									  memset(param->password_buff,0,256);
										param->password_len = 0;
									  debugPrint(" \r\n clear input \r\n");
								}
								else
								{
									 g_front_state = FrontAwake;
									 g_front_inited = 0;
								}
							  
            }
						else
						{                               //数字键
                //输入一位
							  param->password_buff[param->password_len] = key;
							  param->password_len ++;
							  debugPrint(" %d",key);
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
        param->key_last = KEY_LED_NONE;
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" please input admin password \n");
        //qDebug() << "FrontAdmin";
    }
}
//顶层设置菜单状态
static void FrontMenuTop(void)
{
		ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
		KeyLed_t key;
    if(g_front_inited)
		{
				//按键输入
        key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_1)
						{      
								//1键
								g_front_state = FrontMenuAddUser;
								g_front_inited = 0;

            }
						else if (key == KEY_LED_2)
						{   //2键
                //删除用户
								g_front_state = FrontMenuDeleteUser;
								g_front_inited = 0;
            }
						else if(key == KEY_LED_STAR) //* key
						{                            //返回上一层目录
                g_front_state = FrontAwake;
                g_front_inited = 0;
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
				//debugPrint(" Front Menu \r\n");
			  debugPrint(" *********************************\r\n");
			  debugPrint(" Press 1 :add user\r\n");
			  debugPrint(" Press 2 :delete user\r\n");
			  debugPrint(" *********************************\r\n");
        //qDebug() << "FrontMenu";
    }
}



//添加用户设置菜单状态
static void FrontMenuAddUser(void)
{
		ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
		KeyLed_t key;
    if(g_front_inited)
		{
				//按键输入
        key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_1)
						{      
								//1键
								g_front_state = FrontAddAdmin;
								g_front_inited = 0;

            }
						else if (key == KEY_LED_2)
						{   //2键
                //清除输入
								g_front_state = FrontAddNormalUser;
								g_front_inited = 0;
            }
						else if(key == KEY_LED_STAR) //* key
						{                            //返回上一层目录
                g_front_state = FrontMenuTop;
                g_front_inited = 0;
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" *********************************\r\n");
			  debugPrint(" Press 1 :add admin\r\n");
			  debugPrint(" Press 2 :add normal user\r\n");
			  debugPrint(" *********************************\r\n");
        //qDebug() << "FrontMenu";
    }
}


//删除用户设置菜单状态
static void FrontMenuDeleteUser(void)
{
		ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
		KeyLed_t key;
    if(g_front_inited)
		{
				//按键输入
        key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_1)
						{      
								//1键
								g_front_state = FrontDeleteAllUser;
								g_front_inited = 0;

            }
						else if (key == KEY_LED_2)
						{   //2键
                //清除输入
								g_front_state = FrontDeleteNormalUser;
								g_front_inited = 0;
            }
						else if(key == KEY_LED_STAR) //* key
						{                            //返回上一层目录
                g_front_state = FrontMenuTop;
                g_front_inited = 0;
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" *********************************\r\n");
			  debugPrint(" Press 1 :delete all user\r\n");
			  debugPrint(" Press 2 :delete normal user\r\n");
			  debugPrint(" *********************************\r\n");
        //qDebug() << "FrontMenu";
    }
}


static void FrontDeleteAllUser(void)
{
	  ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
		KeyLed_t key;
    if(g_front_inited)
		{
			  //按键输入
        key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_HASH)
						{            //#键
                if (param->password_len > 0 )
								{   
									  if(UserFindAdmin())
										{
											  g_front_state = FrontMenuDeleteUser;
											  g_front_inited = 0;
											  
											  debugPrint(" delete all user success  \r\n");
											  UserDeleteAllKeyUser();
												UserDeleteAllAdminUser();
										}
										else
										{
											  //非法输入
											  g_front_state = FrontMenuDeleteUser;
											  g_front_inited = 0;
											  
											  debugPrint(" password error ,delete fail  \r\n");
										}

                }
								memset(param->password_buff,0,256);
								param->password_len = 0;
            }
						else if (key == KEY_LED_STAR)
						{      //*键
                //清除输入
							  if(param->password_len > 0)
								{
									  memset(param->password_buff,0,256);
										param->password_len = 0;
									  debugPrint(" \r\n clear input \r\n");
								}
								else
								{
									 g_front_state = FrontMenuDeleteUser;
									 g_front_inited = 0;
								}
            }
						else
						{                               //数字键
                //输入一位
							  param->password_buff[param->password_len] = key;
							  param->password_len ++;
							  debugPrint(" %d",key);
            }
        }
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontMenuDeleteUser;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" please input admin password \n");
    }
}


static  void FrontDeleteNormalUser(void)
{
	  ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
		KeyLed_t key;
    if(g_front_inited)
		{
			  g_front_state = FrontMenuDeleteUser;
        g_front_inited = 0;
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontMenuDeleteUser;
            g_front_inited = 0;
        }
    }
		else
		{
        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
			  debugPrint(" delete normal user success\n");
			  //删除指纹
			  //删除RFID
			  //删除密码
			  UserDeleteAllKeyUser();
    }
	
	
}
//添加管理员用户
static void FrontAddAdmin(void)
{
	  ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    u32 id, idx;
    KeyLed_t key;
    if(g_front_inited)
		{
        //按键输入
				key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_HASH)
						{            //#键
                if (param->password_len > 0)
								{  
									 if(param->password_len > 12)
									 {
										  debugPrint(" password more than 12 ,fail\r\n");
									 }
									 else
									 {
										   debugPrint(" \r\n input password : ");
										   g_user_config.password_admin_hi = -1;
										   g_user_config.password_admin_lo = -1;
											 for(idx = 0;idx < param->password_len;idx ++)
											 {
													 g_user_config.password_admin_hi = (g_user_config.password_admin_hi << 4) | (g_user_config.password_admin_lo >> 28);
                           g_user_config.password_admin_lo = (g_user_config.password_admin_lo << 4) | (param->password_buff[idx] & 0xF);
												   debugPrint(" %d",param->password_buff[idx]);
											 }
											 g_user_config.verify_mode = 1;
											 g_user_config.password_num_len = param->password_len;
											 AdminSaveConfig();
											 debugPrint(" \r\n add adminer success  \r\n");
									 }
									 g_front_state = FrontMenuAddUser;
									 g_front_inited = 0;
									 memset(param->password_buff,0,256);
									 param->password_len = 0;
									#if 0
									//验证密码
									//有输入
									if ((param->password_lo == g_user_config.password_unlock_lo) &&
											(param->password_hi == g_user_config.password_unlock_hi))
									{//开锁
											g_front_state = FrontUnlock;
											g_front_inited = 0;
									}else
									{                       //非法输入
											g_front_state = FrontAlarm;
											g_front_inited = 0;
									}
									#endif
							}
								
            }
					  else if (key == KEY_LED_STAR)
						{   //*键
                //清除输入
							  if(param->password_len > 0)
								{
									 memset(param->password_buff,0,256);
							     param->password_len = 0;
									 debugPrint("\r\n clear input password   \r\n ");
								}
								else//返回上一层
								{ 
									  g_front_state = FrontMenuAddUser;
								    g_front_inited = 0;
								}
            }
						else
						{                               //数字键
                //输入一位
							  param->password_buff[param->password_len] = key;
							  param->password_len ++;
							  debugPrint(" %d",key);
            }
        }
				//超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();

			  debugPrint(" *********************************\r\n");
			  debugPrint(" Please add passrord for admin  \r\n");
			  debugPrint(" *********************************\r\n");
        //qDebug() << "FrontAwake";
    }

	
	
}

//添加一般用户
static void FrontAddNormalUser(void)
{
		ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    s32 id, idx;
    KeyLed_t key;
    if(g_front_inited)
		{
        //按键输入
				key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_1)
						{    //1键
								g_front_state = FrontAddNormalKeyUser;
								g_front_inited = 0;
            }
					  else if (key == KEY_LED_2)
						{   //2键
                g_front_state = FrontAddNormalFingerUser;
								g_front_inited = 0;
            }
						else if (key == KEY_LED_3)
						{   //3键
                g_front_state = FrontAddNormalRfidUser;
								g_front_inited = 0;
            }
						else if (key == KEY_LED_STAR)
						{   //*键
								g_front_state = FrontMenuAddUser;
								g_front_inited = 0;
            }
        }
				
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
					  
        }
    }
		else
		{
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();
				if(g_user_config.admin_write_index == 0)
				{
					  debugPrint(" *********************************\r\n");
			      debugPrint(" Please add adminer first  \r\n");
			      debugPrint(" *********************************\r\n");
					  g_front_state = FrontMenuAddUser;
						g_front_inited = 0;
					
				}
				else
				{
					  debugPrint(" *********************************\r\n");
			      debugPrint(" Press 1 :add key user  \r\n");
			      debugPrint(" Press 2 :add fig user  \r\n");
						debugPrint(" Press 3 :add rfid user \r\n");
			      debugPrint(" *********************************\r\n");
				}
			  
        //qDebug() << "FrontAwake";
    }
	
}


//添加一般密码用户
static void FrontAddNormalKeyUser(void)
{
	  ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    u32 id, idx;
    KeyLed_t key;
    if(g_front_inited)
		{
        //按键输入
				key = FrontGetKey();
        if (key == KEY_LED_NONE)
				{
            param->key_last = key;
        }
				else if (key != param->key_last)
				{
            g_front_stamp = FrontGetStamp();     //更新时间戳
            param->key_last = key;
            VoicePlay(VOICE_BEEP);
            if (key == KEY_LED_HASH)
						{            //#键
                if (param->password_len > 0)
								{  
									 if(param->password_len > 12)
									 {
										  debugPrint(" password more than 12 ,fail\r\n");
									 }
									 else
									 {
										   debugPrint(" \r\n input password : ");
										   g_user_config.password_unlock_hi = -1;
										   g_user_config.password_unlock_lo = -1;
											 for(idx = 0;idx < param->password_len;idx ++)
											 {
													 g_user_config.password_unlock_hi = (g_user_config.password_unlock_hi << 4) | (g_user_config.password_unlock_lo >> 28);
                           g_user_config.password_unlock_lo = (g_user_config.password_unlock_lo << 4) | (param->password_buff[idx] & 0xF);
												   debugPrint(" %d",param->password_buff[idx]);
											 }
											 g_user_config.verify_mode = 0;
											 g_user_config.password_num_len = param->password_len;
											 UserSaveConfig();
											 debugPrint(" \r\n add normal user success  \r\n");
									 }
									 g_front_state = FrontAddNormalUser;
									 g_front_inited = 0;
									 memset(param->password_buff,0,256);
									 param->password_len = 0;
							}
								
            }
					  else if (key == KEY_LED_STAR)
						{   //*键
                //清除输入
							  if(param->password_len > 0)
								{
									 memset(param->password_buff,0,256);
							     param->password_len = 0;
									 debugPrint("\r\n clear input password   \r\n ");
								}
								else//返回上一层
								{ 
									  g_front_state = FrontAddNormalUser;
								    g_front_inited = 0;
								}
            }
						else
						{                               //数字键
                //输入一位
							  param->password_buff[param->password_len] = key;
							  param->password_len ++;
							  debugPrint(" %d",key);
            }
        }
				//超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontSleep;
            g_front_inited = 0;
        }
    }
		else
		{
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();

			  debugPrint(" *********************************\r\n");
			  debugPrint(" Please add passrord for normal user  \r\n");
			  debugPrint(" *********************************\r\n");
    }
}
static void FrontAddNormalFingerUser(void)
{
		ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    u32 id, idx;
    if(g_front_inited)
		{
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontAddNormalUser;
            g_front_inited = 0;
        }
    }
		else
		{
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();

			  debugPrint(" *********************************\r\n");
			  debugPrint(" Please press finger  \r\n");
			  debugPrint(" *********************************\r\n");
    }
}
static void FrontAddNormalRfidUser(void)
{
	  ParamAwake_t *param = (ParamAwake_t *)&g_front_param;
    u32 id, idx;
    if(g_front_inited)
		{
        //超时处理
        if (FrontCalcElapsed(g_front_stamp) >= FRONT_OVERTIME)
				{
            g_front_state = FrontAddNormalUser;
            g_front_inited = 0;
        }
    }
		else
		{
			  memset(param->password_buff,0,256);
				param->password_len = 0;

        g_front_inited = 1;
        g_front_stamp = FrontGetStamp();

			  debugPrint(" *********************************\r\n");
			  debugPrint(" Please add rfid  \r\n");
			  debugPrint(" *********************************\r\n");
    }
	
}
//===========================前板状态机接口======================================//
//前板状态机初始化
void FrontInit(void)
{
    //初始化状态为睡眠状态
    g_front_state = FrontSleep;
    g_front_inited = 0;
    //初始化设置键状态为抬起状态
    g_config_last = 0;
    //用户管理初始化
    UserInit();
}
//前板状态机运行
void FrontTask(void)
{
    //前板状态机运行
    g_front_state();

    //判断设置键短按/长按
    u32 config_key = FrontGetConfigKey();
    if ((g_config_last == 0) && (config_key == 1))
		{//设置键按下时记录时间戳
        g_config_stamp = FrontGetStamp();
        g_config_triggered = 0;
    }
		else if ((g_config_last == 1) && (config_key == 1))
		{//持续按下且超时时触发"长按"事件,同时禁止后面继续触发直到抬起设置键
        if ((g_config_triggered == 0) && (FrontCalcElapsed(g_config_stamp) >= 3000))
				{
            //qDebug() << "LogPress";
            g_config_triggered = 1;
        }
    }
		else if ((g_config_last == 1) && (config_key == 0))
		{//抬起设置键时,如果没有触发"长按"就触发"短按"
        if (g_config_triggered == 0)
				{
            //进入管理员验证状态
            if ((g_front_state == FrontSleep) || (g_front_state == FrontAwake))
						{
                g_front_state = FrontVerifyAdmin;
                g_front_inited = 0;
            }
        }
        g_config_triggered = 0;
    }
    g_config_last = config_key;
}
