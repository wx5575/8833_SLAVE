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

static uint8_t *send_buf;/* ���ͻ����� */
static uint16_t send_count;/* �������ݼ��� */
static uint8_t *send_buf_bak;/* ���ͻ��������� �Ա��ط�ʹ�� */
static uint16_t send_count_bak;/* �������ݼ��� �Ա��ط�ʹ�� */
static uint8_t uart_send_st;/* ���ڷ���״̬ */
static uint32_t usart_timeout;///<��ʱ��ʱ
static uint8_t usart_receive_over;///<������ɱ�־
static uint32_t receive_count;/* �����ֽڼ��� */


/**
  * @brief  USART3 ����
  * @param  ��
  * @retval ��
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
    
    /*IO���Ÿ��ù������ã���֮ǰ�汾��ͬ*/
    /*����USART3*/
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
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //ʹ�� USART3�ж�
    USART_Cmd(USART3, ENABLE); //ʹ�� USART3
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //Ƕ��ͨ��ΪUSART3_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    //��Ӧ���ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART3��������
  * @param  ��
  * @retval ��
  */
void usart3_send_data(uint8_t *data, uint32_t len)
{
    send_buf = data;
    send_count = len;
	send_buf_bak = send_buf;/* ���ͻ��������� �Ա��ط�ʹ�� */
	send_count_bak = send_count;/* �������ݼ��� �Ա��ط�ʹ�� */
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}
/**
  * @brief  USART3��������
  * @param  ��
  * @retval ��
  */
void usart3_resend_data(void)
{
    send_buf = send_buf_bak;
    send_count = send_count_bak;
    
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, ENABLE);
}

/**
  * @brief  ��ȡ����״̬
  * @param  ��
  * @retval ����״̬
  */
uint8_t usart3_get_send_status(void)
{
    uint8_t res = uart_send_st;
    
    uart_send_st = 0;
    
    return res;
}
/**
  * @brief  ��ȡ�������ݻ�������ַ
  * @param  ��
  * @retval �������ݻ�������ַ
  */
uint8_t* usart3_get_receive_data(void)
{
    return uart_receive_data;
}
/**
  * @brief  ��ȡ���ռ�����
  * @param  ��
  * @retval ��
  */
uint32_t usart3_get_receive_data_count(void)
{
    return receive_count;
}
/**
  * @brief  ��ս��ռ�����
  * @param  ��
  * @retval ��
  */
void usart3_clear_receive_data_count(void)
{
    receive_count = 0;
}
/**
  * @brief  ��ȡ���ڷ������ݻ�����
  * @param  ��
  * @retval �������ݻ�������ַ
  */
uint8_t * usart3_get_send_buf(void)
{
    return uart_send_buf;
}
/**
  * @brief  ��λ��ʱ��ʱ��
  * @param  ��
  * @retval ��
  */
void usart3_reset_timeout(void)
{
    usart_timeout = UART_TIMEOUT;
}
/**
  * @brief  ��ȡ��ʱ������ֵ
  * @param  ��
  * @retval ��ʱ������ֵ
  */
uint32_t usart3_get_timeout(void)
{
    return usart_timeout;
}
/**
  * @brief  ��ʱ��������һ
  * @param  ��
  * @retval ��
  */
uint32_t usart3_sub_timeout(void)
{
    usart_timeout--;
    
    return usart_timeout;
}

/**
  * @brief  ���²��жϴ��ڵĳ�ʱʱ���Ƿ�Ϊ0�����Ϊ0�ͱ�ʾ���������,�˺����Ǳ���ʱ�жϵ���
  * @param  ��
  * @retval ��
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
  * @brief  ��ȡ������ɱ�־
  * @param  ��
  * @retval ������ɱ�־
  */
uint8_t get_usart3_receive_over_flag(void)
{
    return usart_receive_over;
}
/**
  * @brief  ��ս�����ɱ�־
  * @param  ��
  * @retval ��
  */
void clear_usart3_receive_over_flag(void)
{
    usart_receive_over = 0;
}
/**
  * @brief  ��ս��ռ�����
  * @param  ��
  * @retval ��
  */
void usart3_clear_receive_count(void)
{
    receive_count = 0;
}
/**
  * @brief  USART3�жϷ������
  * @param  ��
  * @retval ��
  */
void usart3_close(void)
{
	USART_Cmd(UART_PORT, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_TC, DISABLE);
    USART_ITConfig(UART_PORT, USART_IT_RXNE, DISABLE);
}

/**
  * @brief  USART3�жϷ������
  * @param  ��
  * @retval ��
  */
void USART3_IRQHandler(void)
{
	OSIntEnter();
    
    if(USART_GetITStatus(UART_PORT, USART_IT_RXNE) == SET)
    {
        usart3_reset_timeout();//��λ��ʱ��ʱ��
        uart_receive_data[receive_count] = USART_ReceiveData(UART_PORT);
        receive_count = (receive_count + 1) % BUFFER_SIZE;
	}
    if(USART_GetITStatus(UART_PORT, USART_IT_TC) == SET)
    {
        if(send_count == 0)
        {
            USART_ITConfig(UART_PORT, USART_IT_TC, DISABLE);
            USART_ITConfig(UART_PORT, USART_IT_RXNE, ENABLE);
            uart_send_st = 1;//��Ƿ������
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