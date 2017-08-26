/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：app.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __APP_H__
#define __APP_H__

/*********************** 包含头文件 *************************/
#include <ucos_ii.h>
#include "stm32f10x.h"
#include "cs99xx_config.h"
#include "cs99xx_struct.h"

/*********************** 宏定义 ****************************/



/* 蜂鸣器开关 */
// #if MUTE_EN
// #define BUZZER_SOUND(n)     {BUZZER = BUZZER_ON;g_buzzer_time = (n);}
// #else
#define RT_STRING(S1, S2)	(sys_par.language==CHINESE? S1:S2)
#define BUZZER_SOUND(n)
//{ \
//            if(ENABLE != sys_par.silent_sw) \
//            { \
//                BUZZER = BUZZER_ON; \
//                g_buzzer_time = (n); \
//            } \
//        }
// #endif
#define CYLINDER_CTRL(n)    {cur_cylinder_ctrl_status = CY_STOP; g_cylinder_ctrl_t = (n);}

#define PASS_WORD 		"888888"	/* 系统密码 */
#define CALIBRATE_PW 	"000000"	/* 校准密码 */
#define AUTHEN_PW_CAL   1   /* 验证校准密码 */
#define AUTHEN_PW_SYS   0   /* 验证系统密码 */

#ifndef NULL
#define NULL    0
#endif

/********************** 类型定义 *************************/
typedef struct Sys_St_Flag{
	int8_t ratio_state;/* 系统校准参数状态 */
}SYS_ST_FLAG;
/********************** 全局变量 *************************/

typedef uint8_t (*UART_FUN)(void);

#ifdef   APP_GLOBALS
#define  APP_EXT
#else
#define  APP_EXT  extern
#endif

enum {
REMOTE_COMM = 1,	/* 远控状态 */
LOCAL_COMM = 0,		/* 本机状态 */
};


APP_EXT SYS_ST_FLAG 	g_sys_state;	/* 系统状态 */
APP_EXT uint8_t g_buzzer_sw;	/* 蜂鸣器开关 */
APP_EXT uint8_t plain_code; /* 密码明码开关 1为明码 */
APP_EXT uint8_t gunlock;	/* 当打开键盘锁时，将该值置1，一回操作就不用再输入密码了 */

APP_EXT uint8_t g_exit;	/* 退回到主界面 */
APP_EXT uint8_t g_return;	/* 返回到上级菜单 */
APP_EXT uint8_t g_skip_page;/* 跳转页 */

APP_EXT uint32_t g_serial_num;/* 被测品的流水号 */
APP_EXT uint32_t g_comm_status;/* 联机状态标志 */
APP_EXT uint32_t g_short_delay;/* 短路中断延时 ACW与DCW可能有所区别 */
APP_EXT uint8_t  key_status;/* 标记按状态主要为通信提供 */

APP_EXT		char		 gUpdateTime[20];

APP_EXT 	OS_EVENT     *KeyboardQSem;	/* 消息队列 */
APP_EXT	 	void     	 *QMsgTbl[OS_MAX_QS];

#define     MDBUS_BUF_SIZE      20
APP_EXT 	OS_EVENT     *MdbusQSem;	/* Mdbus发送消息队列 */
APP_EXT	 	void     	 *MdbusQMsgTbl[MDBUS_BUF_SIZE];

APP_EXT 	OS_EVENT     *ScheduleSem;/* 调度信号量 */
APP_EXT 	OS_EVENT     *MemMutex; /* 访问memory的互斥互斥访问 */
APP_EXT 	OS_EVENT     *MdbusMutex;/* 访问mdbus发送队列的互斥互斥访问 */

//APP_EXT		uint16_t	 power_check_flag;	/* 电源检查标志 */

#if SYNCHRO_ROADS_EN>0
APP_EXT		OS_TMR		 * syn_tmr;			/* 为多路同步测试服务负责定时采集电流值 */
APP_EXT		OS_TMR		 * syn_scan_tmr;			/* 为多路同步测试服务负责定时扫描 */
#endif

/*****************************************************************
*					task stack
*****************************************************************/

APP_EXT OS_STK  Startup_Task_STK[STARTUP_TASK_STK_SIZE];
APP_EXT OS_STK  Keyscan_Task_STK[KEY_TASK_STK_SIZE];
APP_EXT OS_STK  Sample_Task_STK[SAMPLE_TASK_STK_SIZE];
APP_EXT OS_STK  Modbus_Send_STK[MODBUS_SEND_STK_SIZE];
APP_EXT OS_STK  Modbus_M_Send_STK[MODBUS_M_SEND_STK_SIZE];
APP_EXT OS_STK  Modbus_Master_Send_STK[MODBUS_MASTER_SEND_STK_SIZE];
APP_EXT OS_STK  Main_Task_STK[MAIN_TASK_STK_SIZE];

#if SYNCHRO_ROADS_EN>0
APP_EXT OS_STK  SYN_Roads_Task_STK[SYN_ROADS_TASK_STK_SIZE];
APP_EXT OS_STK  SYN_Send_Task_STK[SYN_SEND_TASK_STK_SIZE];
#endif



APP_EXT UART_FUN  p_prvvUARTRxISR;
APP_EXT UART_FUN  p_prvvUARTTxReadyISR;


/********************* 外部函数声明 *******************/
extern int8_t authenticate_pw(const int8_t n);

/********************* 函数声明 ********************/

extern uint8_t get_cal_pin(void);
extern void off_keyboard(void);
extern void on_keyboard(void);
extern void off_sample_task(void);
extern void on_sample_task(void);
extern void disable_sample_task(void);
extern void resume_sample_task(void);
extern void off_schedule_task(void);
extern void on_schedule_task(void);
extern void clear_mdbus_send_queue(void);
extern void clear_keyboard(void);

extern void startup_task(void* p_arg);
extern uint8_t get_key_task_state(void);

extern void mdbus_init_s(void);
extern void mdbus_init_m(void);
extern void my_refresh_lcd24064(void);

#endif //__APP_H__
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
