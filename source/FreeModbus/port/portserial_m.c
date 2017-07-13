/*
 * FreeModbus Libary: STM32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include    "ucos_ii.h"
#include	"BSP_config.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED
/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);
static void prvvUARTRxISR(void);
/* ----------------------- Start implementation -----------------------------*/

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if (xRxEnable)
	{
#if  MB_MASTER_RS485_ENABLE>0
		MASTER_RS485_RECEIVE_MODE;
// 		soft_delay_1us(200);
#endif
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
	}
	if (xTxEnable)
	{
#if  MB_MASTER_RS485_ENABLE>0
		EXIT_CRITICAL_SECTION(  ); //开全局中断
		MASTER_RS485_SEND_MODE;
		cs_delay_ms(5);
#endif
		USART_ITConfig(UART5, USART_IT_TC, ENABLE);
	}
	else
	{
		USART_ITConfig(UART5, USART_IT_TC, DISABLE);
	}
}

void vMBMasterPortClose(void)
{
	USART_ITConfig(UART5, USART_IT_TXE | USART_IT_RXNE, DISABLE);
	USART_Cmd(UART5, DISABLE);
}
//默认一个主机 串口5 波特率可设置  奇偶检验可设置
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//======================时钟初始化=======================================
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	//======================IO初始化=======================================	
	//UART5_TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//UART5_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//配置485发送和接收模式
#if  MB_MASTER_RS485_ENABLE>0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
	//======================串口初始化=======================================
	USART_InitStructure.USART_BaudRate = ulBaudRate;
	//设置校验模式
	switch (eParity)
	{
	case MB_PAR_NONE: //无校验
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		break;
	case MB_PAR_ODD: //奇校验
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	case MB_PAR_EVEN: //偶校验
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		break;
	default:
		return FALSE;
	}

	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	if (ucPORT != 5)
		return FALSE;

	ENTER_CRITICAL_SECTION(); //关全局中断

	USART_Init(UART5, &USART_InitStructure);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART5, ENABLE);

	//=====================中断初始化======================================
	//设置NVIC优先级分组为Group2：0-3抢占式优先级，0-3的响应式优先级
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART5_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART5_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	EXIT_CRITICAL_SECTION(); //开全局中断
    USART_SendData(UART5, 0);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	return TRUE;
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
	USART_SendData(UART5, ucByte);
//     while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	return TRUE;
}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = USART_ReceiveData(UART5);
	return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
	pxMBMasterFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
	pxMBMasterFrameCBByteReceived();
}

void xxx(void)
{
    prvvUARTRxISR();
    prvvUARTTxReadyISR();
    
}
// /*******************************************************************************
//  * Function Name  : USART2_IRQHandler
//  * Description    : This function handles USART2 global interrupt request.
//  * Input          : None
//  * Output         : None
//  * Return         : None
//  *******************************************************************************/
void UART5_IRQHandler(void)
{
	OSIntEnter();
    
	//接收中断
	if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		prvvUARTRxISR();
	}
	//发送中断
// 	if (USART_GetITStatus(UART5, USART_IT_TXE) == SET)
	if (USART_GetITStatus(UART5, USART_IT_TC) == SET)
	{
		prvvUARTTxReadyISR();
	}
    
	OSIntExit();
}

#endif
