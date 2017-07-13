/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：usart.h
 * 摘  要  ：串口通信
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */


#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "string.h"
#include "cs99xx_config.h"

#define __USART2_ENABLE

#define     BUFFER_SIZE     (200)	//接收数据缓冲大小
#define 	SYN_TIM_OUT		(100)


#ifdef   USART_GLOBALS
#define  USART_EXT
#else
#define  USART_EXT  extern
#endif

USART_EXT FlagStatus receive_ok;
USART_EXT uint8_t receive_buf[BUFFER_SIZE];
USART_EXT uint8_t receive_data_num;
USART_EXT uint8_t syn_receive_buf[2][BUFFER_SIZE];
USART_EXT uint16_t syn_data_num[2];
USART_EXT uint8_t receive_data_num;
USART_EXT int8_t cur_point;/* 指向缓冲的当前位置 */

// USART_EXT uint16_t cur_data_lon;
USART_EXT uint16_t uart5_timeout;
USART_EXT void (*p_uart5_irq) (void);
USART_EXT uint8_t ReceiveCmdData[BUFFER_SIZE];

void Uart5_Init(void);

void Uart5_SendStr(char str[]);

void Com_Client_PutStr(uint8_t *str, uint8_t len);
uint8_t Read_Usart_data(uint8_t *data);
void Clear_Usart_data(void);


#endif

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
