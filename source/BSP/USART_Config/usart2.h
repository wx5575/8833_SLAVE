#ifndef _usart_h
#define _usart_h
#include "stm32f10x.h"


#define USART_BOUND 9600	  					//通信波特率为9600
#define REVBUFF_SIZE 			(256*2)

extern u8 u8_ReceiveData;

extern u8 USART2_FIFO_Write;
extern u8 USART2_FIFO_Read;
extern u8 USART2_BUF[REVBUFF_SIZE];


void NVIC_Configuration(void);
void USART1_Configuration(u32 bound);
void USART2_Configuration(u32 bound);
void Uart1_PutChar(u8 ch);
void Uart2_PutChar(u8 ch);
void prints(unsigned char *p);
void printi(signed long val);
void prints_2(unsigned char *p, uint32_t n);


#endif


