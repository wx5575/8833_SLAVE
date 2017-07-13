/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：RTC_Config.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
 /****************** 包含头文件 *****************/
#define  RTC_GLOBALS
#include    "stm32f10x_rtc.h"
#include    "stm32f10x_pwr.h"
#include    "stm32f10x_bkp.h"
#include	<string.h>
#include	<stdio.h>

#include 	"GUI_Draw.h"
#include	"BSP_config.h"
#include	"RTC_Config.h"
#include 	"app.h"
#include 	"serve_test.h"

/******************** 声明内部函数 ***************/
static void RTC_NVIC_Conf(void);

void Time_Adjust(void);

/******************** 函数定义 ******************/

void on_rtc_int(void)
{
    RTC_INI(ENABLE);
}

void off_rtc_int(void)
{
    RTC_INI(DISABLE);
}
/*
******************************************************************************
* 函数名  		: rtc_wait_for_synchro
* 函数描述    	: rtc等待同步，带超时
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
void rtc_wait_for_synchro(void)
{
	uint32_t temp_time_out = 0;
  /* Clear RSF flag */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* Loop until RSF flag is set */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
	  if(temp_time_out++ > 0xffff)
	  {
          BKP_WriteBackupRegister(BKP_DR1, 0x0);
		  break;
	  }
  }
}

/*
******************************************************************************
* 函数名  		: rtc_wait_for_last_task
* 函数描述    	: rtc等待最近一次操作完成，带超时
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
void rtc_wait_for_last_task(void)
{
	uint32_t temp_time_out = 0;
  /* Loop until RTOFF flag is set */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
	  if(temp_time_out++ > 0xffff)
	  {
          BKP_WriteBackupRegister(BKP_DR1, 0x0);
		  break;
	  }
  }
}
/*******************************************************************************
* 函数名 		: set_data
* 函数描述    	: 设置RTC时间
* 输入参数    	: 无
* 输出结果     	: 无
* 返回值       	: 无
*******************************************************************************/
void set_sys_time(void)
{
	time_now.tm_year = TM_YEAR;
	time_now.tm_mon  = TM_MONTH;
	time_now.tm_mday = TM_MDAY;
	time_now.tm_hour = TM_HOUR;
	time_now.tm_min  = TM_MIN;
	time_now.tm_sec  = TM_SEC;
	
	Time_Adjust();
}

void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	/* Change the current time */
	RTC_SetCounter(Time_ConvCalendar(time_now));
	/* Wait until last write operation on RTC registers has finished */
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
}

/*******************************************************************************
* 函数名 		: RTC_Configuration
* 函数描述    	: 设置RTC
* 输入参数    	: 无
* 输出结果     	: 无
* 返回值       	: 无
*******************************************************************************/
void RTC_Configuration(void)
{
	uint32_t temp_time_out = 0;
// #define RCC_CLK_LSI /* 是否使用内部时钟配置 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	
#ifdef RCC_CLK_LSI/*由于内部时钟的频率在30-60kHz之间，因此无法准确得到32.768kHz时钟与秒中断 */
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
		if(temp_time_out++>0xffff)
		{
			break;
		}
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#else	/* 使用外部32.768kHz的时钟，就能得到准确的秒中断 */
	RCC_LSEConfig(RCC_LSE_ON);
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		if(temp_time_out++>0xffff)
		{
			break;
		}
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
#endif
	
	RCC_RTCCLKCmd(ENABLE);
	
// 	RTC_WaitForSynchro();
// 	RTC_WaitForLastTask();
	
	rtc_wait_for_synchro();
	rtc_wait_for_last_task();
	
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	
	RTC_SetPrescaler(32767);
	
// 	RTC_WaitForLastTask();
	rtc_wait_for_last_task();
	
	RTC_NVIC_Conf();
    soft_delay_us(2000);
//     temp_time_out = 0xfffff;
//     while(--temp_time_out);/* 延时 */
        
}

/*
******************************************************************************
* 函数名  		: RTC_NVIC_Conf
* 函数描述    	: 设置NVIC参数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
static void RTC_NVIC_Conf(void)
{
	/* 定义 NVIC 初始化结构体 */
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能 RTC 中断 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = RTC_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void rtc_init(void)
{
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* 配置RTC */ 
		RTC_Configuration();
		
		/* 设定时间 */
		set_sys_time();
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		//启用PWR和BKP的时钟（from APB1） 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
		/* 后备域解锁 */
		PWR_BackupAccessCmd(ENABLE);
		
		/* 等待寄存器同步 */
// 		RTC_WaitForSynchro(); /* 等待上次RTC寄存器写操作完成 死等 */
		rtc_wait_for_synchro();/* 等待操作完成带超时 */
		
		/* 允许RTC秒中断 */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
// 		RTC_WaitForLastTask(); /* 等待上次RTC寄存器写操作完成 死等 */
		rtc_wait_for_last_task();/* 等待操作完成带超时 */
		RTC_NVIC_Conf();
	}
    
	/* 定义了时钟输出宏，则配置校正时钟输出到PC13 */
	#ifdef RTCClockOutput_Enable 
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); 
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE); 

	/* Disable the Tamper Pin */
	BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper 
	functionality must be disabled */ 

	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock); 
	#endif 

	/* Clear reset flags */
	RCC_ClearFlag();
}

void GetTime(void)
{
	u32 CurrenTime = 0;
	CurrenTime = Time_GetTime();
	time_now = Time_ConvToCalendar(CurrenTime);
	
	sprintf(gUpdateTime, "%4d-%02d-%02d %02d:%02d:%02d",
		time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
}

char *transform_time_to_string(uint32_t time, char* str)
{
// 	u32 CurrenTime = 0;
// 	CurrenTime = Time_GetTime();
    static char temp_ch[21];
	time_now = Time_ConvToCalendar(time);
	
	sprintf(temp_ch, "'%4d-%02d-%02d %02d:%02d:%02d",
		time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}

char *transform_data_to_str(uint32_t time, char *str)
{
    static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%04d-%02d-%02d",
        time_now.tm_year,
		time_now.tm_mon+1,
		time_now.tm_mday);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}
char *transform_time_to_str(uint32_t time, char *str)
{
    static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%02d:%02d:%02d",
		time_now.tm_hour,
		time_now.tm_min,
		time_now.tm_sec);
    if(str != NULL)
        strcpy(str, (const char*)temp_ch);
	return temp_ch;
}

char *transform_time_to_str_for_mem(uint32_t time)
{
	static char temp_ch[15];
	time_now = Time_ConvToCalendar(time);
	sprintf(temp_ch, "%02d-%02d-%02d %02d:%02d",
		time_now.tm_year%100,
		time_now.tm_mon+1,
		time_now.tm_mday,
		time_now.tm_hour,
		time_now.tm_min);
	
	return temp_ch;
}
/*
******************************************************************************
* 函数名  		: Time_Show
* 函数描述    	: 得到当前时间字符串
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
void Time_Show(u8 x, u8 y)
{
}
/*
******************************************************************************
* 函数名  		: Time_SetCalendarTime
* 函数描述    	: 设置NVIC参数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetTime(Time_ConvCalendar(t));
	return;
}
/*
******************************************************************************
* 函数名  		: Time_ConvCalendar
* 函数描述    	: 设置NVIC参数
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
u32 Time_ConvCalendar(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2008格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	t.tm_mon -= 1;
	return mktime(&t);
}
/*
******************************************************************************
* 函数名  		: Time_GetTime
* 函数描述    	: 获取rtc 32bit计数值
* 输入参数      : 无
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
u32 Time_GetTime(void)
{
	return (u32)RTC_GetCounter();
}
/*
******************************************************************************
* 函数名  		: Time_ConvToCalendar
* 函数描述    	: 
* 输入参数      : time_t
* 输出结果      : 无
* 返回值        : struct tm
******************************************************************************
*/
struct tm Time_ConvToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
// 	t_tm->tm_mon -= 1;
	return *t_tm;
}
/*
******************************************************************************
* 函数名  		: Time_SetTime
* 函数描述    	: 设置时间
* 输入参数      : time_t
* 输出结果      : 无
* 返回值        : 无
******************************************************************************
*/
void Time_SetTime(time_t t)
{
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}
/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
	OSIntEnter();
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
  	{
    	/* 清除 RTC 秒中断 */
    	RTC_ClearITPendingBit(RTC_IT_SEC);
    	/* 更新时间显示标志位 */
		TimeDisplay = 1;
	}
	OSIntExit();
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
