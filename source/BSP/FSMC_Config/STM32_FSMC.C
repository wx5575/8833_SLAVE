/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：STM32_FSMC.C
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#include "STM32_FSMC.H"

/*
 * 函数名：FSMC_Configuration
 * 描述  ：BANK4 NOR模式B配置
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void FSMC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;                  //声明GPIO的结构
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;   //主要用于NOR FLASH模式配置
  FSMC_NORSRAMTimingInitTypeDef  p;                     //主要设置NOR FLASH模式下的读写时序

/******************** 时钟配置 ********************/
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);     //开启FSMC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE 
                       | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG
					   | RCC_APB2Periph_AFIO, ENABLE);  //开启对应端口的时钟 
/******************** 管脚配置 ********************/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //输出速度
  /* 端口D */
  GPIO_InitStructure.GPIO_Pin  = 0xF8FB;                //FSMC_D1D0A18A17,A16D15D14D13,NE1`WAIT`WR`RW,CLK*D3D2					
  GPIO_Init(GPIOD, &GPIO_InitStructure);                //初始化端口D
  /* 端口E */
  GPIO_InitStructure.GPIO_Pin  = 0xFF88;                //FSMC_D12-D4***,A19***
  GPIO_Init(GPIOE, &GPIO_InitStructure);                //初始化端口E
  /* 端口F */
  GPIO_InitStructure.GPIO_Pin  = 0xF03F;                //FSMC_A9-A6,****,**A5-A0
  GPIO_Init(GPIOF, &GPIO_InitStructure);                //初始化端口F
  /* 端口G */
  GPIO_InitStructure.GPIO_Pin  = 0x167F;                //FSMC_***NE4,*NE3`NE2*,*INT2`A15-A10				
  GPIO_Init(GPIOG, &GPIO_InitStructure);                //初始化端口G

/******************** FSMC配置 ********************/
  p.FSMC_AddressSetupTime = 0XF;                        //地址建立时间
  p.FSMC_AddressHoldTime = 0XF;                         //地址保持时间
  p.FSMC_DataSetupTime = 0X50;                          //数据建立时间
  p.FSMC_BusTurnAroundDuration = 0XF;                   //总线恢复时间
  p.FSMC_CLKDivision = 0X2;                             //时钟分频
  p.FSMC_DataLatency = 0Xf;                             //数据保持时间
  p.FSMC_AccessMode = FSMC_AccessMode_B;                //在地址\数据线不复用的情况下ABCD模式的区别不大
                                                        //本成员配置只有使用扩展模式才有效

  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;      //数据线与地址线不复用
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;                  //存储器类型NOR FLASH
  
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;    //使用异步写模式禁止突发模式
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;  //本成员的配置只在突发模式下有效等待信号极性为低 
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;                  //禁止非对齐突发模式
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//本成员配置仅在突发模式下有效。NWAIT信号在什么时期产生
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;              //本成员的配置只在突发模式下有效禁用NWAIT信号 
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;              //禁止突发写操作 
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;       //写使能
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;          //扩展模式可以使用独立的读、写模式
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;                        //配置读写时序 
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;                            //配置写时序 

  
  /********************* BANK1——>LCD ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;                        //NOR FLASH的BANK4 
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //数据宽度为8位
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
  
/********************* BANK2——>SRAM ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;                        //NOR FLASH的BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //数据宽度为8位
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);   
  
  
/********************* BANK3——>TFT ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                        //NOR FLASH的BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;         //数据宽度为16位
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);   
  
   
/********************* BANK4——>LCD ************************/
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;                        //NOR FLASH的BANK4
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;         //数据宽度为8位
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE); 
}

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
