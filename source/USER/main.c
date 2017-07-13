/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：main.c
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

/******************** 加载文件 ************************/

#include    "app.h"
#include    "BSP_config.h"

/******************** 函数定义 ************************/

int main(void)
{
    uint8_t err = 0;
	
    init_4094();/* 开机立即初始化4094 */
	
	OSInit();//系统初始化
    
	/* 创建消息队列为键盘服务 */
	KeyboardQSem  = OSQCreate(QMsgTbl, OS_MAX_QS);
	MdbusQSem  = OSQCreate(MdbusQMsgTbl, MDBUS_BUF_SIZE);
	
	/* 创建信号量为测试测序服务 */
	ScheduleSem		= OSSemCreate(0);
    
	/*创建互斥量保护临界段*/
	MemMutex = OSMutexCreate(PIP_PRIO,&err);
	MdbusMutex = OSMutexCreate(PIP_PRIO,&err);
    
	/* 创建启动任务 */
	OSTaskCreate(startup_task,(void*)0,
				&Startup_Task_STK[STARTUP_TASK_STK_SIZE-1],
				STARTUP_TASK_PRIO);
	
	OSStart(); /* 启动操作系统 */
	
  	return 0;
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
