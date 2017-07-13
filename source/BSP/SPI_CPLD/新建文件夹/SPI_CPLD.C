/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：SPI_CPLD.H
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：赵永杰
 * 历史版本：
 * 修改记录：2014.8.12 王鑫
 *
 */
/************************ 加载文件 ***********************************/
#include "stm32f10x.h"
#include "SPI_CPLD.H"

/************************ 宏定义 ************************************/
#define			SPI_CS_EN()				GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define			SPI_CS_DIS()			GPIO_SetBits(GPIOE,GPIO_Pin_2)
#define			SPI_MOSI_HIGH()			GPIO_SetBits(GPIOE,GPIO_Pin_3)
#define			SPI_MOSI_LOW()			GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define			SPI_CLK_HIGH()			GPIO_SetBits(GPIOE,GPIO_Pin_5)
#define			SPI_CLK_LOW()			GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define			SPI_MISO_DI()			GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

/************************** 函数定义 *******************************/

void SPI_CPLD_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  //允许GPIOE时钟

	//1、2、4脚定义为输出管脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//上拉输入	 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

void cpld_init(void)
{
	SPI_CPLD_GPIO();
}

/*
 * 函数名：cpld_write
 * 描述  ：往CPLD写命令
 * 输入  ：命令
 * 输出  ：返回值
 */
void cpld_write(u32 SendData)
{
	u8  ShiftCounter;
	u32 DataBuffer;
	
	SPI_CS_EN();																				//CS=0;片选CPLD

	for(ShiftCounter=0;ShiftCounter<26;ShiftCounter++)
	{
		DataBuffer 		 = SendData;
		DataBuffer     = DataBuffer & 0x00000001;				//取数据最低位
		SendData       = SendData >>1;									//数据右移一位 
		SPI_CLK_HIGH(); 																//时钟置高

		if(DataBuffer == 0x00000000)
		{
			SPI_MOSI_LOW();
		}
		else
		{
			SPI_MOSI_HIGH();
		}

		SPI_CLK_LOW();																	//数据写入CPLD
	}
   
	SPI_CS_DIS();																		//CS=1;取消片选
}
/*
 * 函数名：ReadDataFromCPLD
 * 描述  ：从CPLD中读数据
 * 输入  ：SendData 包含读标志读 读命令 
 * 输出  ：
 */
u16 ReadDataFromCPLD(u32 SendData)
{
	u8  ShiftCounter;
	u16 DataBuffer=0;
	u32 SendDataBuffer;
	
	SendDataBuffer   = SendData;	
	cpld_write(SendDataBuffer);																//给CPLD发送读命令
	
	SPI_CS_EN();																							//片选
	
	for(ShiftCounter=0;ShiftCounter<16;ShiftCounter++)
	{

		DataBuffer = DataBuffer << 1;													//左移一位

		SPI_CLK_HIGH();																				//时钟置高

		SPI_CLK_LOW();
		
		if(SPI_MISO_DI() == 1)
		{
			DataBuffer = DataBuffer | 0x0001;  									 	//低位置1
		}
		else
		{
			DataBuffer = DataBuffer & 0xfffe; 										//低位置0
		}
	}
	
	SPI_CS_DIS();
	
	return(DataBuffer);
}



/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
