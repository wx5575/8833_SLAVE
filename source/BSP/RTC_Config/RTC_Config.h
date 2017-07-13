/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：RTC_Config.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __RTC_Config_H__
#define __RTC_Config_H__

/**************** 包含头文件 ***************/
#include <time.h>
#include "stm32f10x.h"

/**************** 宏定义 ******************/

/* 定义默认的年月日时分秒 */
#ifdef	 RTC_GLOBALS
#define TM_YEAR		2016
#define TM_MONTH	3
#define TM_MDAY		10
#define TM_HOUR		0
#define TM_MIN		0
#define TM_SEC		0
#endif

/************** 声明全局变量 ****************/
#ifdef   RTC_GLOBALS
#define  RTC_EXT
#else
#define  RTC_EXT  extern
#endif
RTC_EXT vu32 TimeDisplay;
RTC_EXT struct tm time_now;

/************** 外部函数声明 ****************/
extern char *transform_time_to_string(uint32_t time, char* str);
extern char *transform_time_to_str(uint32_t time, char *str);
extern char *transform_data_to_str(uint32_t time, char *str);
extern char *transform_time_to_str_for_mem(uint32_t time);

extern void RTC_Configuration(void);
extern void rtc_init(void);
extern void GetTime(void);
extern void Time_Show(uint8_t x, uint8_t y);
extern void Time_SetCalendarTime(struct tm t);

extern uint32_t Time_ConvCalendar(struct tm t);
extern uint32_t Time_GetTime(void);
extern struct tm Time_ConvToCalendar(time_t t);
extern void Time_SetTime(time_t t);
extern void RTC_IRQHandler(void);

extern void on_rtc_int(void);
extern void off_rtc_int(void);

extern int8_t JudgeSaveTime(const uint8_t *p);
extern void Time_Adjust(void);
extern void set_sys_time(void);

#endif // __RTC_Config_H__
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
