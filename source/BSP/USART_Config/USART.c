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

#define   USART_GLOBALS
#include "USART.h"
#include "Com_Server.h"
#include "STM32_GPIO_Config.h"
#include "cs99xx_type.h"
#include "BSP_config.h"

/*接收主机发送的数据*/
// uint8_t receive_buf[BUFFER_SIZE];
uint8_t receive_data_num;
int8_t cur_point;/* 指向缓冲的当前位置 */

uint8_t syn_receive_buf[2][BUFFER_SIZE];
uint16_t syn_data_num[2];

/**************************************************************
函数名：Uart5_Init
功能：  初始化串口
参数：  无
返回值：无
***************************************************************/

void Uart5_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
	
	/* config UART5 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	/* UART5 GPIO config */
	/* Configure UART5 Tx (PC.12) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Configure UART5 Rx (PD.02) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* UART5 mode config */
	USART_InitStructure.USART_BaudRate = 9600; // 19200;// 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	USART_ClearFlag(UART5, USART_IT_RXNE);
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	USART_ITConfig(UART5, USART_IT_PE, ENABLE);
	USART_ITConfig(UART5, USART_IT_ERR, ENABLE);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART5_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART5_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART5, ENABLE);
}

// void UART5_IRQHandler(void)
// {
// 	OSIntEnter();
// 	
//     if(USART_GetFlagStatus(UART5, USART_IT_RXNE) == SET)
//     {
// 		USART_ClearFlag(UART5, USART_IT_RXNE);
// 		if(p_uart5_irq)
// 		{
// 			p_uart5_irq();
// 		}
// 	}
// 	
// 	/* 开启CR3,bit0的EIE: Error interrupt enable, 处理USART_IT_ERR,USART_IT_ORE_ER,USART_IT_NE,USART_IT_FE 错误  */
// 	if(USART_GetFlagStatus(UART5, USART_FLAG_ORE) != RESET) 
// 	{
// 		USART_ReceiveData(UART5); //取出来扔掉
// 		USART_ClearFlag(UART5, USART_FLAG_ORE);
// 	}
// 	
// 	if(USART_GetFlagStatus(UART5, USART_FLAG_NE) != RESET) 
// 	{
// 		USART_ClearFlag(UART5, USART_FLAG_NE);
// 	}
// 	
// 	if(USART_GetFlagStatus(UART5, USART_FLAG_FE) != RESET) 
// 	{
// 		USART_ClearFlag(UART5, USART_FLAG_FE);
// 	}
// 	
// 	if(USART_GetFlagStatus(UART5, USART_FLAG_PE) != RESET) 
// 	{
// 		USART_ClearFlag(UART5, USART_FLAG_PE);
// 	}
// 	
// 	OSIntExit();
// }

/* 发送一个字符 */
static void Uart5_PutChar(uint8_t ch)
{
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	USART_SendData(UART5, ch);
}

/* 发送字符串 */
void Uart5_PutStr(uint8_t *str, uint8_t len)
{
    uint8_t i;
    for(i = 0; i< len; i++) {
        Uart5_PutChar(str[i]);
    }
}
void Com_Client_PutStr(uint8_t *str, uint8_t len)
{
// 	extern void prints_2(unsigned char *p, uint32_t n);
    Uart5_PutStr(str, len);
// 	prints_2(str, len);
}

uint8_t Read_Usart_data(uint8_t *data)
{
    uint8_t i;
    for(i = 0; i < receive_data_num; i++) {
        data[i] = receive_buf[i]; 
    }
	
    data[i++] = 0;
	data[i] = 0;

    return receive_data_num;
}

void Clear_Usart_data(void)
{
    uint8_t i;
    for(i = 0; i < receive_data_num; i++) {
        receive_buf[i] = '\0'; 
    }
	receive_buf[i] = '\0'; 
    receive_data_num = 0;
}


void Uart5_SendStr(char str[])
{
    while( * str != '\0') {
        Uart5_PutChar( *str);
        str ++;
    }
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
