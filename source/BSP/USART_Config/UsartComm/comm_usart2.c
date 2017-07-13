#include "stm32f10x.h"
#include "comm_usart2.h"
#include "ucos_ii.h"

#include "BSP_config.h"
#include "STM32_GPIO_Config.h"

#include 	"serve_test.h"

/*接收数据缓冲和接收数据收尾节点*/
char UsartReceive_Data[BUFFER_SIZE_USART2] ;
uint16_t Usart2_rear = 0;
uint16_t Usart2_front = 0;

extern void ModBusServerReceiveNewData(void);
extern void ModBusClientReceiveNewData(void);


void Usart2Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity =  USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);
    
    /********使能 USART2**********/
    USART_Cmd(USART2, ENABLE);
    
    /* 若接收数据寄存器满，则产生中断 */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ClearFlag(USART2, USART_FLAG_TC); 
	
   /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART2_IT_PRE_PRIO;  /* 先占优先级0级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART2_IT_SUB_PRIO;	   //中断响应优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
    NVIC_Init(&NVIC_InitStructure);
    
    Usart2_SendData("", 1);
}

// void USART2_IRQHandler(void)
// {
// //     static int x;
//     
//     OSIntEnter();
// //     
// //     LED_PASS = !LED_PASS;
// //     
// //     if(++x%10 == 0)
// //     {
// //         LED_FAIL = !LED_FAIL;
// //     }
//     
//     if(USART_GetFlagStatus(USART2, USART_IT_RXNE) == SET)
//     {
//         USART_ClearFlag(USART2, USART_IT_RXNE);
//         UsartReceive_Data[Usart2_rear] = USART_ReceiveData(USART2);
//         ModBusServerReceiveNewData();
//         Usart2_rear = (Usart2_rear + 1) % BUFFER_SIZE_USART2;
//         
//         USART_ClearFlag(USART2, USART_IT_RXNE);//test_start
//     } 
//     OSIntExit();
// }

void Usart2_SendData(char * send_data , uint8_t num)
{
    uint8_t i;
    for(i = 0; i< num; i++) {                   
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);   
        USART_SendData(USART2 ,send_data[i]);
    }
}


#if 0
void Usart1_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity =  USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl =  USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    
    /********使能 USART1**********/
    USART_Cmd(USART1, ENABLE);     

    /* 若接收数据寄存器满，则产生中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC); 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, VECT_TAB_OFFSET);
  
   /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //通道设置为串口1中断
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
    NVIC_Init(&NVIC_InitStructure); 
    
    Usart1_SendData("", 1);	 
}

void USART1_IRQHandler(void)
{   
    
    if(USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
    {
        
        UsartReceive_Data[Usart1_rear] = USART_ReceiveData(USART1); 
 //
        ModBusServerReceiveNewData();
        ModBusClientReceiveNewData();

        Usart1_rear = (Usart1_rear + 1) % BUFFER_SIZE;                        
  
        USART_ClearFlag(USART1, USART_IT_RXNE);
    } 
    
}

void Usart1_SendData(char * send_data , uint8_t num)
{
    uint8_t i;
    for(i = 0; i< num; i++) {
        USART_SendData(USART1 ,send_data[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}

#endif

