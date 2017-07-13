/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_result.c
 * 摘  要  ：系统时间管理
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include    "stm32f10x.h"
#include	"keyboard.h"
#include	"RTC_Config.h"
#include 	"app.h"
#include 	"serve_test.h"
#include 	"SysTemConfig.h"

/*
 * 函数名：serve_timekey
 * 描述  ：服务于时间设置模块 是一个内联函数 被sys_settime调用
 * 输入  ：uint16_t keyvalue 键值
 *		   uint8_t *n *n是记录当前已经输入的位数包括-和：的数目*n的范围是(0-16)
 * 输出  ：无
 */
static __inline void serve_timekey(const uint16_t keyvalue, uint8_t *n)
{
	uint8_t flag = 0;
    
	switch(keyvalue)
	{
		case KEY_0:
			change_data(0);
			break;
		case KEY_1:
			change_data(1);
			break;
		case KEY_2:
			change_data(2);
			break;
		case KEY_3:
			change_data(3);
			break;
		case KEY_4:
			change_data(4);
			break;
		case KEY_5:
			change_data(5);
			break;
		case KEY_6:
			change_data(6);
			break;
		case KEY_7:
			change_data(7);
			break;
		case KEY_8:
			change_data(8);
			break;
		case KEY_9:
			change_data(9);
			break;
		default:
			flag = 1;
			break;
	}
    
	if(flag == 0)
	{
		update_data(pmenu_data);
		*n += 1;
		switch(*n)
		{
			case 2:
			case 5:
			case 8:
			case 11:
			case 14:
				*n += 1;
			break;
			default:
				break;
		}
		if(*n > 16)
		{
			*n = 0;
		}
	}
}

/*
 * 函数名：JudgeSaveTime
 * 描述  ：判断输入的时间数据是否合法
 * 输入  ：uint8_t *p 指向输入时间的临时空间
 * 输出  ：无
 * 返回  ：int8_t -1表示输入的数据非法，0表示合法
 */
int8_t JudgeSaveTime(const uint8_t *p)
{
	int16_t year = p[0] + 2000;
	uint8_t s = 0;	/* 标记闰年 1为闰年 0为平年 */
	/* 判断润年 */
	if((year%4 == 0 && year%100 != 0) || year%400 == 0)
	{
		s = 1;
	}
	
	/* 检查月 */
	if(p[1] > 12)
	{
		return	-1;
	}
	
	/* 检查日 */
	if(p[2] > 31)
	{
		return	-1;
	}
	
	/* 检查小时 */
	if(p[3] > 23)
	{
		return	-1;
	}
	
	/* 检查分钟 */
	if(p[4] > 59)
	{
		return	-1;
	}
	/* 检查秒 */
	if(p[5] > 59)
	{
		return -1;
	}
	
	switch	(p[1])
	{
		case 2:	
			if(s)
			{
				/* 闰年2月不能超过29天 */
				if(p[2] > 29)
				{
					return	-1;
				}
			}
			else
			{
				/* 平年2月不能超过28天 */
				if(p[2] > 28)
				{
					return	-1;
				}
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:	
			if(p[2] > 30)
			{
				return	-1;
			}
			break;
		default:
			break;
	}

	return	0;
}

/*
 * 函数名：sys_settime
 * 描述  ：设置系统时间 被menu_subtask调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void sys_updata(uint32_t w)
{
    BKP_WriteBackupRegister(BKP_DR2, 0xA5A5);
    
    updata_comm_flag |= RESET_INTERFACE_MCU;
    OSTimeDlyHMSM(0,0,2,500);
    NVIC_SystemReset();
}
/*
 * 函数名：sys_settime
 * 描述  ：设置系统时间 被menu_subtask调用
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void sys_settime(uint32_t w)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
	int8_t sign = 0;
	uint8_t n = 0;	/* 标示当前设置的参数 */
	int8_t m = 0;
	int8_t t = 0;
	uint8_t time_data[6] = {0};
	u32 CurrenTime = 0;
	uint16_t year_base = 0;
	DATA_INFO time_info[] = {
		{/*年*/	(void*)0/*pData*/,45+6*2/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
		{/*月*/	(void*)0/*pData*/,45+6*5/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,2/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
		{/*日*/	(void*)0/*pData*/,45+6*8/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,3/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
		{/*时*/	(void*)0/*pData*/,45+6*11/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,4/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
		{/*分*/	(void*)0/*pData*/,45+6*14/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,5/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
		{/*秒*/	(void*)0/*pData*/,45+6*17/*x*/,32/*y*/,0/*type*/,2/*lon*/,0/*decs*/,6/*total*/,6/*cur*/,0/*ch_bit*/,0/*page*/,0/*bits*/,},
	};
l:	
	CurrenTime = Time_GetTime();
	time_now = Time_ConvToCalendar(CurrenTime);
	year_base = time_now.tm_year/100*100;
	
	time_data[0] = time_now.tm_year%100;	/* 取出年的低两位 */
	time_data[1] = time_now.tm_mon+1;
	time_data[2] = time_now.tm_mday;
	time_data[3] = time_now.tm_hour;
	time_data[4] = time_now.tm_min;
	time_data[5] = time_now.tm_sec;
	
	Time_Show(45,17);
	gui_draw_time_manage();
	pmenu_data = &time_info[0];
	pmenu_data->pData = &time_data[0];
	cursor_pos(pmenu_data->x, pmenu_data->y);
	
	while(1)
	{
		pkey_value = OSQPend(KeyboardQSem,10,&err);
		serve_timekey(*pkey_value, &n);
		/* 根据n的值来更新pmenu_data指向的数据 */
		switch(n)
		{
			case 0:
			case 3:
			case 6:
			case 9:
			case 12:
			case 15:
				pmenu_data = &time_info[n/3];
				pmenu_data->ch_bit = 0;
				pmenu_data->pData = &time_data[n/3];
				break;
			case 1:
			case 4:
			case 7:
			case 10:
			case 13:
			case 16:
				pmenu_data->ch_bit = 1;
				break;
			default:
				break;
		}
		cursor_pos(45+6*2 + n * 6, 32);/* 更新光标位置 */
		
		switch(*pkey_value)
		{
			case KEY_UP:
				if ( pmenu_data->ch_bit == 0)
				{
					t = 10;	/* 十位的权是10 */
				}
				else
				{
					t = 1; /* 个位的权是1 */
				}
				
				t = time_data[n/3]/t;
				m = t%10;
				if(++m > 9)
				{
					m = 0;
				}
				change_data(m);
				update_data(pmenu_data);
				break;
			case KEY_DOWN:
				if ( pmenu_data->ch_bit == 0)
				{
					t = 10;
				}
				else
				{
					t = 1;
				}
				t = time_data[n/3]/t;
				m = t%10;
				
				if(--m < 0)
				{
					m = 9;
				}
				change_data(m);
				update_data(pmenu_data);
				break;
			case KEY_SHIFT:
				sign = 1;
				if(++n > 16)
				{
					n = 0;
				}
				switch(n)
				{
					case 2:
					case 5:
					case 8:
					case 11:
					case 14:
						n++;
					break;
					default:
						break;
				}
				break;
			case KEY_ENTER:
				sign = JudgeSaveTime(time_data);
				if(sign == -1)
				{
					CurrenTime = Time_GetTime();
					time_now = Time_ConvToCalendar(CurrenTime);
					
					time_data[0] = time_now.tm_year%100;	/* 取出年的低两位 */
					time_data[1] = time_now.tm_mon+1;
					time_data[2] = time_now.tm_mday;
					time_data[3] = time_now.tm_hour;
					time_data[4] = time_now.tm_min;
					time_data[5] = time_now.tm_sec;
					GetTime();
					break;
				}
				time_now.tm_year = time_data[0]+year_base;
				time_now.tm_mon  = time_data[1];
				time_now.tm_mday = time_data[2];
				time_now.tm_hour = time_data[3];
				time_now.tm_min  = time_data[4];
				time_now.tm_sec  = time_data[5];
				
// 				Time_SetCalendarTime(time_now);
				{
// 					extern void Time_Adjust(void);
					Time_Adjust();
					GetTime();
				}
				break;
			case KEY_BACK:
				g_return = 1;
				return ;
			case KEY_EXIT:
				g_exit = 1;
				return ;
            case KEY_0 & KEY_1:
                set_sys_time();
                goto l;
		}
		Time_Show(45,17);
		OSTimeDlyHMSM(0,0,0,50);
	}
}




/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
