#ifndef __COMM_USART2_H
#define __COMM_USART2_H

#include "stm32f10x.h"
#include "bsp.h"

#define     BUFFER_SIZE_USART2         (512)

/*接收数据缓冲和接收数据收尾节点*/
extern char UsartReceive_Data[BUFFER_SIZE_USART2] ;
extern uint16_t Usart2_rear ;
extern uint16_t Usart2_front ;

extern void Usart2Config(void);
extern void Usart2_SendData(char * send_data , uint8_t num);

#endif

