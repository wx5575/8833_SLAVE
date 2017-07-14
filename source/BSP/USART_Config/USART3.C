/************************************************************
Copyright (C), 2012-2022, yin.
FileName: main.c
Author: ycw Version :  1.0 Date: 2012.04.26
Description: USART3 SendData  
Version: V2.0                 
Function List:USART3 SendData 
History: V1.0                     
<author> <time> <version > <desc>
YCW 12/04/26 1.0 build this moudle
***********************************************************/

#include "USART3.H"
#include <ucos_ii.h>

#define     BUFFER_SIZE         (512)
//#define     MAX_485_EN
#define     UART_TIMEOUT   (5)    //30ms

#define UART_PORT   USART3

static uint8_t uart_send_buf[BUFFER_SIZE];
static uint8_t uart_receive_data[BUFFER_SIZE];

static uint8_t *send_buf;/* 发送缓冲区 */
static uint16_t send_count;/* 发送数据计数 */
static uint8_t *send_buf_bak;/* 发送缓冲区备份 以备重发使用 */
static uint16_t send_count_bak;/* 发送数据计数 以备重发使用 */
static uint8_t uart_send_st;/* 串口发送状态 */
static uint32_t usart_timeout;///<超时计时
static uint8_t usart_receive_over;///<接收完成标志
static uint32_t receive_count;/* 接收字节计数 */


/**
  * @brief  USART3 配置
  * @param  无
  * @retval 无
  */
void usart3_config(uint32_t baud_rate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    
	/* UART GPIO config */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	/* Configure UART Rx  as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*IO引脚复用功能设置，与之前版本不同*/
    /*配置USART3*/
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate =baud_rate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ClockStructInit(&USART_ClockInitStruct);
    USART_ClockInit(USART3, &USART_ClockInitStruct);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //使能 USART3中断
    USART_Cmd(USART3, ENABLE); //使能 USART3
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //嵌套通道为USART3_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //响应优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART3发送数据
  * @param  无
  * @retval 无
  */
void usart3_send_data(uint8_t *data, uint32_t len)
{
    send_buf = data;
    send_count = len;
	send_buf_bak = send_buf;/* 发送缓冲区备份 以备重发使用 */
	send_count_bak = send_count;/* 发送数据计数 以备重发使用 */
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}
/**
  * @brief  USART3发送数据
  * @param  无
  * @retval 无
  */
void usart3_resend_data(void)
{
    send_buf = send_buf_bak;
    send_count = send_count_bak;
    
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}

/**
  * @brief  获取发送状态
  * @param  无
  * @retval 发送状态
  */
uint8_t usart3_get_send_status(void)
{
    uint8_t res = uart_send_st;
    
    uart_send_st = 0;
    
    return res;
}
/**
  * @brief  获取接收数据缓冲区地址
  * @param  无
  * @retval 接收数据缓冲区地址
  */
uint8_t* usart3_get_receive_data(void)
{
    return uart_receive_data;
}
/**
  * @brief  获取接收计数器
  * @param  无
  * @retval 无
  */
uint32_t usart3_get_receive_data_count(void)
{
    return receive_count;
}
/**
  * @brief  清空接收计数器
  * @param  无
  * @retval 无
  */
void usart3_clear_receive_data_count(void)
{
    receive_count = 0;
}
/**
  * @brief  获取串口发送数据缓冲区
  * @param  无
  * @retval 发送数据缓冲区地址
  */
uint8_t * usart3_get_send_buf(void)
{
    return uart_send_buf;
}
/**
  * @brief  复位超时定时器
  * @param  无
  * @retval 无
  */
void usart3_reset_timeout(void)
{
    usart_timeout = UART_TIMEOUT;
}
/**
  * @brief  获取超时计数器值
  * @param  无
  * @retval 超时计数器值
  */
uint32_t usart3_get_timeout(void)
{
    return usart_timeout;
}
/**
  * @brief  超时计数器减一
  * @param  无
  * @retval 无
  */
uint32_t usart3_sub_timeout(void)
{
    usart_timeout--;
    
    return usart_timeout;
}

/**
  * @brief  更新并判断串口的超时时间是否为0，如果为0就表示接收完成了,此函数是被定时中断调用
  * @param  无
  * @retval 无
  */
void usart3_judge_timout(void)
{
    if(usart_timeout > 0)
    {
        usart_timeout--;
        
        if(usart_timeout == 0)
        {
            usart_receive_over = 1;
        }
    }
}
/**
  * @brief  获取接收完成标志
  * @param  无
  * @retval 接收完成标志
  */
uint8_t get_usart3_receive_over_flag(void)
{
    return usart_receive_over;
}
/**
  * @brief  清空接收完成标志
  * @param  无
  * @retval 无
  */
void clear_usart3_receive_over_flag(void)
{
    usart_receive_over = 0;
}
/**
  * @brief  清空接收计数器
  * @param  无
  * @retval 无
  */
void usart3_clear_receive_count(void)
{
    receive_count = 0;
}
/**
  * @brief  USART3中断服务程序
  * @param  无
  * @retval 无
  */
void usart3_close(void)
{
	USART_Cmd(UART_PORT, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
}

/**
  * @brief  USART3中断服务程序
  * @param  无
  * @retval 无
  */
void USART3_IRQHandler(void)
{
	OSIntEnter();
    
    if(USART_GetITStatus(UART_PORT, USART_IT_RXNE) == SET)
    {
        usart3_reset_timeout();//复位超时计时器
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
    
	OSIntExit();
}
