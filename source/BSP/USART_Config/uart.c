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
#include "stm32f10x.h"
#include "UART.h"
// #include "BSP.h"
#include "STM32_GPIO_Config.h"
#include "app.h"
#include "string.h"

#define MAX485_CTRL       (PAout(1))
#define MAX485_CTRL_LOW      0
#define MAX485_CTRL_HIGH     1

#define     BUFFER_SIZE         ( 1024)
#define     MAX_485_EN
#define     UART_TIMEOUT   (10)    //ms

#define UART_PORT   USART2

static uint8_t uart_send_buf[BUFFER_SIZE];
static uint8_t uart_receive_data[BUFFER_SIZE];

static uint8_t *send_buf;/* 发送缓冲区 */
static uint16_t send_count;/* 发送数据计数 */
static uint8_t *send_buf_bak;/* 发送缓冲区备份 以备重发使用 */
static uint16_t send_count_bak;/* 发送数据计数 以备重发使用 */
static uint8_t uart_send_st;/* 串口发送状态 */
static uint32_t uart_timeout;
static uint32_t receive_count;/* 接收字节计数 */

/**************************************************************
函数名：Uart5_Init
功能：  初始化串口
参数：  无
返回值：无
***************************************************************/

void uart_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config UART5 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    
	/* UART GPIO config */
	/* Configure UART5 Tx  as alternate function push-pull */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	/* Configure UART Rx  as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    /* 485控制引脚 */
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//     
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	/* UART mode config */
	USART_InitStructure.USART_BaudRate = 115200; // 19200;// 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART_PORT, &USART_InitStructure);
	USART_ClearFlag(UART_PORT, USART_IT_RXNE);
	
	USART_ITConfig(UART_PORT, USART_IT_RXNE, ENABLE);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(UART_PORT, ENABLE);
}

static void soft_delay_us_(u32 dly_us)
{
	unsigned int dly_i;
	while(dly_us--)
	{
		for(dly_i=0;dly_i<802;dly_i++);
	}
}
static void soft_delay_10us(u32 dly_us)
{
    soft_delay_us_(dly_us);
}
void set_send_data_i(uint8_t *buf, uint16_t c)
{
#ifdef MAX_485_EN
    MAX485_CTRL = MAX485_CTRL_HIGH;
//     {int i;for(i=0;i<100;i++);}
    soft_delay_10us(1000);
#endif
    send_buf = buf;
    send_count = c;
	send_buf_bak = send_buf;/* 发送缓冲区备份 以备重发使用 */
	send_count_bak = send_count;/* 发送数据计数 以备重发使用 */
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}
/* 重发最后一次发送数据 */
void re_send_last_data_i(void)
{
#ifdef MAX_485_EN
    MAX485_CTRL = MAX485_CTRL_HIGH;
//     {int i;for(i=0;i<100;i++);}
    soft_delay_10us(1000);
#endif
	
	send_count = send_count_bak;/* 发送数据计数 以备重发使用 */
    send_buf = send_buf_bak;/* 发送缓冲区备份 以备重发使用 */
	
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}

uint8_t get_send_status_i(void)
{
    uint8_t res = uart_send_st;
    
    uart_send_st = 0;
    
    return res;
}
uint8_t* get_receive_data_i(void)
{
    return uart_receive_data;
}
uint32_t get_receive_data_count_i(void)
{
    return receive_count;
}
void clear_receive_data_count_i(void)
{
    receive_count = 0;
}
uint8_t * get_uart_send_buf_i(void)
{
    return uart_send_buf;
}
void reset_uart_timeout_i(void)
{
    uart_timeout = UART_TIMEOUT;
}
uint32_t get_uart_timeout_i(void)
{
    return uart_timeout;
}
uint32_t sub_uart_timeout_i(void)
{
    uart_timeout--;
    
    return uart_timeout;
}

void clear_receive_count_i(void)
{
    receive_count = 0;
	memset(uart_receive_data, 0, sizeof(uart_receive_data));
}

void close_uart_i(void)
{
	USART_Cmd(UART_PORT, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
}

void USART2_IRQHandler1(void)
{
    if(USART_GetITStatus(UART_PORT, USART_IT_RXNE) == SET)
    {
		USART_ClearITPendingBit(UART_PORT, USART_IT_RXNE);
        reset_uart_timeout_i();
        uart_receive_data[receive_count] = USART_ReceiveData(UART_PORT);
        receive_count = (receive_count + 1) % BUFFER_SIZE;
	}
    if(USART_GetITStatus(UART_PORT, USART_IT_TC) == SET)
    {
        if(send_count == 0)
        {
            USART_ITConfig(UART_PORT, USART_IT_TC, DISABLE);
            USART_ITConfig(UART_PORT, USART_IT_RXNE, ENABLE);
            uart_send_st = 1;//标记发送完成
        #ifdef MAX_485_EN
            MAX485_CTRL = MAX485_CTRL_LOW;
        #endif
        }
        else
        {
            USART_SendData(UART_PORT , *send_buf);
            send_count--;
            send_buf++;
        }
	}
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
