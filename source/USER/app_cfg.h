/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：app_cfg.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __APP_CFG_H__
#define __APP_CFG_H__


/************** 硬件选择配置 *****************************/

#define DMA_FOR_ADC_EN	0 /* 1 使用DMA 采集数据 0使用普通方法 */


/*************** 软件配置 ********************************/
enum{
    /* 任务优先级 */
    PIP_PRIO					    = 1,	/* 提升优先级 */
    
    TIMER_TASK_PRIO				    = 2,    /* 任务优先级0被软件定时器使用了 */
    SCHEDULE_TASK_PRIO			    = 4, 	/* 键盘扫描优先级 */
    MODBUS_SEND_TASK_PRIO		    = 3,	/* MODBUS发送任务 */
    MODBUS_M_SEND_TASK_PRIO		    = 18,	/* MODBUS发送任务 */
    MODBUS_MASTER_SEND_TASK_PRIO    = 22,   /* Modebus 主机发送任务 */
    MODBUS_RECEIVE_TASK_PRIO	    = 21,	/* 直流模块优先级 */
    STARTUP_TASK_PRIO     		    = 24,	/* 启动任务优先级 */
    Keyscan_TASK_PRIO			    = 26, 	/* 键盘扫描优先级 */
    Main_TASK_PRIO				    = 27,	/* 主任务优先级 */
    SAMPLE_TASK_PRIO			    = 25,	/* 采样任务优先级 */
    
    /* 任务ID */
    MODBUS_SEND_TASK_ID             = 0,
    Main_TASK_ID			        = 1,
    Keyscan_TASK_ID			        = 2,
    SAMPLE_TASK_ID			        = 3,
    MODBUS_RECEIVE_TASK_ID	        = 4,
    SCHEDULE_TASK_ID		        = 7,
    MODBUS_MASTER_SEND_TASK_ID      = 8,
    MODBUS_M_SEND_TASK_ID      		= 9,
    
    /* 任务栈的大小 */
    STARTUP_TASK_STK_SIZE 			= 200,	/* 200*4Byte  */
    MODBUS_MASTER_SEND_STK_SIZE     = 300,
    MODBUS_SEND_STK_SIZE			= 300,
    MODBUS_M_SEND_STK_SIZE			= 300,
    MAIN_TASK_STK_SIZE				= 1024,	/* 900 * 4Byte */
    KEY_TASK_STK_SIZE				= 50,
    SCHEDULE_TASK_STK_SIZE			= STARTUP_TASK_STK_SIZE,
    SAMPLE_TASK_STK_SIZE			= 200,
    MODBUS_RECEIVE_TASK_STK_SIZE	= 300,
};

#endif



/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
