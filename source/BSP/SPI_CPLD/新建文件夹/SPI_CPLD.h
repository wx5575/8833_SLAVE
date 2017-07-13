/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：SPI_CPLD.H
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：赵永杰
 * 历史版本：
 * 修改记录：2014.8.12 王鑫
 *
 */
#ifndef _SPI_CPLD_h
#define _CPI_CPLD_h

#include "stm32f10x.h"

/* 寄存器位 */
#define CPLD_GND_SELECT		0X80
#define	CPLD_IR_GR			0X40
#define CPLD_IR_F5			0x20
#define CPLD_IR_F4			0X10
#define	CPLD_IR_F3			0x08
#define CPLD_IR_F6			0X04
#define	CPLD_IR_F2			0X02
#define CPLD_IR_F1			0x01

#define	CPLD_W				0X01000000	/* 写 */
#define	CPLD_R				0X02000000	/* 读 */

/* 命令 */
#define CPLD_START				0X0101AAAA 	/* 启动计数器 */
#define CPLD_STOP				0X01015555	/* 停止计数 */
#define	CPLD_READ_DIFFER		0X02000000 	/* 读相位差 */
#define CPLD_CUR_CUNT			0x02010000	/* 读电流计数值 */


/*********************** 外部函数声明 ************************/
void cpld_init(void);
void cpld_write(u32 SendData);
u16 ReadDataFromCPLD(u32 SendData);


#endif


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
