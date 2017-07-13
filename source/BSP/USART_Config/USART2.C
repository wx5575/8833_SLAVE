#include "stm32f10x.h"
#include "usart2.h"
#include "General.h"



static void NVIC_Config(void);
void DMA_Config(void);

DMA_InitTypeDef DMA_InitStructure;

u8 USART2_FIFO_Write = 0,USART2_FIFO_Read = 0;
u8 USART2_BUF[REVBUFF_SIZE];


/*******************************************************************************
* Function Name   : NVIC_Configuration
* Description        : Configures NVIC and Vector Table base location.
* Input                    : None
* Output                 : None
* Return                 : None
*******************************************************************************/
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
   /* Enable the USART1 Interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //通道设置为串口1中断
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
//    NVIC_Init(&NVIC_InitStructure); 						   //初始化


	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   //打开中断
	NVIC_Init(&NVIC_InitStructure); 						   //初始化
}

/*******************************************************************************
	函数名：USART1_Configuration
	输  入:
	输  出:
	功能说明：
	初始化串口硬件设备，启用中断
	配置步骤：
	(1)打开GPIO和USART1的时钟
	(2)设置USART1两个管脚GPIO模式
	(3)配置USART1数据格式、波特率等参数
	(4)使能USART1接收中断功能
	(5)最后使能USART1功能
*/
void USART1_Configuration(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* 第4步：配置USART1参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

    /* 若接收数据寄存器满，则产生中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* 第5步：使能 USART1， 配置完毕 */
	USART_Cmd(USART1, ENABLE);

    /* 如下语句解决第1个字节无法正确发送出去的问题 */
    USART_ClearFlag(USART1, USART_FLAG_TC);     // 清标志
	
	
}


/*******************************************************************************
	函数名：USART1_Configuration
	输  入:
	输  出:
	功能说明：
	初始化串口硬件设备，启用中断
	配置步骤：
	(1)打开GPIO和USART1的时钟
	(2)设置USART1两个管脚GPIO模式
	(3)配置USART1数据格式、波特率等参数
	(4)使能USART1接收中断功能
	(5)最后使能USART1功能
*/
void USART2_Configuration(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 第1步：打开GPIO和USART2部件的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* 第2步：将USART2 Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 第3步：将USART2 Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);

	/* 第4步：配置USART2参数
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

    /* 若接收数据寄存器满，则产生中断 */
//     USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
// 	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

	/* 第5步：使能 USART2， 配置完毕 */
	USART_Cmd(USART2, ENABLE);

    /* 如下语句解决第1个字节无法正确发送出去的问题 */
//     USART_ClearFlag(USART2, USART_FLAG_TC);     // 清标志
// 	DMA_Config();
}
/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */

void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟
	NVIC_Config();	   			//配置DMA中断
	
 	/*设置DMA源：内存地址&串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (vu32)&(USART2->DR);	   

	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART2_BUF;
	
	/*方向：从 外设 --> 内存*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
    DMA_InitStructure.DMA_BufferSize = REVBUFF_SIZE;
	
	/*外设地址不增*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*外设数据单位*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMA模式：一次传输，循环*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;	 
	
	/*优先级：中*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/*禁止内存到内存的传输	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*配置DMA1的4通道*/		   
    DMA_Init(DMA1_Channel6, &DMA_InitStructure); 	   
	
	DMA_Cmd (DMA1_Channel6,ENABLE);					//使能DMA
// 	DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断

}

/*
 * 函数名：NVIC_Config
 * 描述  ：DMA 中断配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void DMA1_Channel6_IRQHandler(void)
{	
//判断是否为DMA发送完成中断
   if(DMA_GetFlagStatus(DMA1_FLAG_TC6)==SET) 
   {  
   //LED关闭  
	 
	DMA_ClearFlag(DMA1_FLAG_TC6); 
	}	
}

//******************************************************************
//函数名称：Uart1_PutChar
//参  数：
//返回值：
//功  能：向串口1发送一个字符
//描  述：
//创建者：赵永杰
//日  期：2013.1.17
//修改者：
//日  期：
//备  注：
//******************************************************************
void Uart1_PutChar(u8 ch)
{
  USART_SendData(USART1, (u8) ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


/****************************************************************************** 
	*函数名称：void Uart2_PutChar(u8 ch)
	*
	*函数功能：向串口2发送一个字符
	*
	*入口函数：ch
	*
	*出口函数：无
	*
	*编 写 者：
	*
	*日    期：
	*
	*审 核 者：
	*
	*日    期：
	*
	*附    注：发送的字符用单引号，比如发送A为Uart2_PutChar('A')
	*
******************************************************************************/  

void Uart2_PutChar(u8 ch)
{
  USART_SendData(USART2, (u8) ch);
  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}
uint32_t rt_uart_write(const void* buffer, uint32_t size)
{
	int i;
	char *ptr;
	
	ptr = (char*)buffer;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	for(i=0;i<size;i++)
	{
		Uart2_PutChar(*ptr++);
	}
	OSTimeDly(2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	return (uint32_t) ptr - (uint32_t) buffer;
}

/****************************************************************************** 
	*函数名称：void prints(unsigned char *p)
	*
	*函数功能：向串口1发送一串字符
	*
	*入口函数：*p
	*
	*出口函数：无
	*
	*编 写 者：
	*
	*日    期：
	*
	*审 核 者：
	*
	*日    期：
	*
	*附    注：发送的字符用双引号，比如发送ABCD为Uart2_PutChar("ABCD")
	*
******************************************************************************/ 

void prints(unsigned char *p)
{
  while(*p != '\0')
  {
    Uart1_PutChar(*p++);
  }
}




/****************************************************************************** 
	*函数名称：void prints_2(unsigned char *p)
	*
	*函数功能：向串口2发送一串字符
	*
	*入口函数：*p
	*
	*出口函数：无
	*
	*编 写 者：
	*
	*日    期：
	*
	*审 核 者：
	*
	*日    期：
	*
	*附    注：发送的字符用双引号，比如发送ABCD为Uart2_PutChar("ABCD")
	*
******************************************************************************/ 

void prints_2(unsigned char *p, uint32_t n)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
  while(n--)
  {
    Uart2_PutChar(*p++);
  }
  OSTimeDly(2);
  GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}



/****************************************************************************** 
	*函数名称：void printi(signed long val)
	*
	*函数功能：
	*
	*入口函数：signed long val
	*
	*出口函数：无
	*
	*编 写 者：
	*
	*日    期：
	*
	*审 核 者：
	*
	*日    期：
	*
	*附    注：
	*
******************************************************************************/ 

void printi(signed long val)
{
	signed char Ascll[11],i=0;
	if(val >= 0)
		Ascll[10] = 0;
	else
	{
    	Ascll[10] = 1;
    	val = -val;
	}
	if(val == 0) Ascll[i++] = '0';
	while(val > 0)
	{
		Ascll[i++]= val%10 + '0';//转化为AScll码序列 
		val /= 10;
	}
	if(Ascll[10])Uart1_PutChar('-');
	for(i-=1;i>=0;i--)
		Uart1_PutChar(Ascll[i]);
}



/*******************************************************************/
/*                                                                 */
/* STM32在串口1接收1字节                                           */
/* 说明：串口1接收中断                                             */
/*                                                                 */
/*******************************************************************/
void USART1_IRQHandler(void)            //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求，因此必须在中断服务程序中对中断源进行判别，然后分别进行处理。当然，如果只涉及到一个中断请求，是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯
{
  u8 dat;
  
   
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {     
    dat = USART_ReceiveData(USART1);
// 	u8_ReceiveData = dat;
	
    Uart1_PutChar(dat);
  }
}





/*******************************************************************/
/*                                                                 */
/* STM32在串口2接收1字节                                           */
/* 说明：串口2接收中断                                             */
/*                                                                 */
/*******************************************************************/

void USART2_IRQHandler(void)            //在中断服务程序中，由于主机响应中断时并不知道是哪个中断源发出中断请求，因此必须在中断服务程序中对中断源进行判别，然后分别进行处理。当然，如果只涉及到一个中断请求，是不用做上述判别的。但是无论什么情况，做上述判别是个好习惯
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    //若接收数据寄存器满
  {
		USART2_BUF[USART2_FIFO_Write++] = USART_ReceiveData(USART2);
		if(USART2_FIFO_Write>=20)USART2_FIFO_Write=0;
  }
}



