/* Includes ------------------------------------------------------------------*/
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "i2c_ee.h"

#define I2C_SETCLK()				(GPIO_SetBits(GPIOC,GPIO_Pin_11))
#define I2C_CLRCLK()				(GPIO_ResetBits(GPIOC,GPIO_Pin_11))
#define I2C_SETSDA()				(GPIO_SetBits(GPIOC,GPIO_Pin_13))
#define I2C_CLRSDA()				(GPIO_ResetBits(GPIOC,GPIO_Pin_13))
#define I2C_READSDA()				(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13))
#define I2C_SETWP()					(GPIO_SetBits(GPIOD,GPIO_Pin_3))
#define I2C_CLRWP()					(GPIO_ResetBits(GPIOD,GPIO_Pin_3))

u16 EEPROM_ADDRESS;

static void I2C_GPIO_SDA_IN(void)
{ 
	GPIO_InitTypeDef	GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
static void I2C_GPIO_SDA_OUT(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;	       // 开漏输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void I2C_GPIO_CAL_IN(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	=GPIO_Mode_IPU;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
static void I2C_Delay(u32 t)
{
	while(t--);
}
static void I2C_Start(void)
{
	/* 初始状态准备 */
	I2C_CLRCLK();/* 时钟 低 */
	I2C_SETSDA();//数据 高
	I2C_SETCLK();/* 时钟高 */
	
	/* 发start信号 */
	I2C_Delay(I2C_Dly);
	I2C_CLRSDA();// 数据 低         ---\___
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();//时钟 低
	I2C_Delay(I2C_Dly);
}
static void I2C_Stop(void)
{
	/* 准备 */
	I2C_CLRSDA();/* 数据 低 */
	I2C_SETCLK();/* 时钟 高 */
	
	/* 发stop信号 */
	I2C_Delay(I2C_Dly);
	I2C_SETSDA();// 数据高         ___/---
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();/* 时钟低 */
	I2C_Delay(I2C_Dly);
}
/*接收应答位*//*只要产生一个脉冲，主器件没有必要接收数据位*/
static void I2C_Receive_ACK(void)
{
	I2C_SETCLK();
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*发送应答位*/
static void I2C_Send_ACK(void)
{
	I2C_CLRSDA();
	I2C_Delay(I2C_Dly);
	I2C_SETCLK();//发送应答位
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*发送非应答位*/
static void I2C_Send_NOACK(void)
{
	I2C_SETSDA();//发送非应答位
	I2C_Delay(I2C_Dly);
	I2C_SETCLK();
	I2C_Delay(I2C_Dly);
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
}
/*I2C写一个字节数据到24C512*/
static void I2Cwrite_byte_to24C512(unsigned char d)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{	
		if(d & 0x80)
		{
			I2C_SETSDA();
		}
		else
		{
			I2C_CLRSDA();
		}
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);
		d=d<<1;
	}
}
/*I2C读一个字节数据到24C512和一个非应答位,最后SDA为输出*/
static unsigned char I2Cread_byte_from24C512()
{
	unsigned char i;
	unsigned char d;
	I2C_GPIO_SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		if(I2C_READSDA() & 0x01)
		{
			d=(d<<1)+1;
		}
		else
		{
			d=d<<1;
		}
	}
	I2C_CLRCLK();
	I2C_Delay(I2C_Dly);
	I2C_GPIO_SDA_OUT();
	// I2C_Send_NOACK();
	return d;
}
static u8 I2C_Wait_Idle(void)
{
	u8 s=0;
	u16 timeover = I2C_TimeOver;
	do{
		I2C_Start();//产生 I2Cx传输 START条件
		I2Cwrite_byte_to24C512(EEPROM_ADDRESS);
		I2C_GPIO_SDA_IN();
		
		I2C_SETCLK();
		I2C_Delay(I2C_Dly);
		s = I2C_READSDA();
		I2C_CLRCLK();
		I2C_Delay(I2C_Dly);

		I2C_GPIO_SDA_OUT();
		I2C_Stop();
		I2C_Delay(I2C_Dly*10);
	}while(s && timeover--);
	if(timeover == 0)
	{
		return I2C_Error_Timeover;
	}
	return I2C_OK;
}
static I2C_SetAddr(u16 Addr)
{
	/* Send STRAT condition */
	I2C_Start();//产生 I2Cx传输 START条件
	/* Send EEPROM address for write */
	I2Cwrite_byte_to24C512(EEPROM_ADDRESS); /* 零位是0表示写 */
	I2C_Receive_ACK(); //(sun)主机只发送脉冲，并不监测数据位
	/* Send the EEPROM's internal address to write to */
	I2Cwrite_byte_to24C512((Addr>>8) & 0xFF);//高8位
	I2C_Receive_ACK();
	I2Cwrite_byte_to24C512(Addr & 0xFF);//低8位
	I2C_Receive_ACK();
}
/*
 * 函数名：I2C_GPIO_Config
 * 描述  ：I2C1 I/O配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    /* 使能与 I2C1 有关的时钟 */
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE);
    
    /* PC11-I2C1_SCL、PC12-I2C1_SDA*/
    GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_11 | GPIO_Pin_13 ;
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_3 ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOD,GPIO_Pin_3);/* 写保护 */
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
    
    /* PC11-I2C1_SCL、PC12-I2C1_SDA*/
//   GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_15 ;
//   GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//   GPIO_Init(GPIOG, &GPIO_InitStructure);
}


/*
 * 函数名：I2C_EE_Init
 * 描述  ：I2C 外设(EEPROM)初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void I2C_EE_Init(void)
{

  I2C_GPIO_Config(); 
	
/* 根据头文件i2c_ee.h中的定义来选择EEPROM要写入的地址 */
#ifdef EEPROM_Block0_ADDRESS
  /* 选择 EEPROM Block0 来写入 */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
// #endif

#elif defined EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
// #endif

#elif defined EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
// #endif

#elif defined EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

/*
 * 函数名：I2C_EE_BufferWrite
 * 描述  ：将缓冲区中的数据写到I2C EEPROM中
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址
 *         -NumByteToWrite 要写入EEPROM的字节数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	Addr = WriteAddr % I2C_PageSize;//写入地址是每页的第几位
	count = I2C_PageSize - Addr;//在开始的一页还可以写入的个数
	NumOfPage = NumByteToWrite / I2C_PageSize;//要写入的页数
	NumOfSingle = NumByteToWrite % I2C_PageSize;//不足一页的个数
    
	I2C_EE_WriteProtect(0);
	/* If WriteAddr is I2C_PageSize aligned  */
	if(Addr == 0) //写入地址是页的开始
	{
	    /* If NumByteToWrite < I2C_PageSize */
	    if(NumOfPage == 0) //数据小于一页
	    {
	      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//写少于一页的数据
	    }
	    /* If NumByteToWrite > I2C_PageSize */
	    else //数据大于等于一页
	    {
	      while(NumOfPage--)//要写入的页数
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); //写一页的数据
	        WriteAddr += I2C_PageSize;
	        pBuffer += I2C_PageSize;
	      }	
	      if(NumOfSingle!=0)//剩余数据小于一页
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);//写少于一页的数据
	      }
	    }
	}
  /* If WriteAddr is not I2C_PageSize aligned  */
	else //写入地址不是页的开始
	{
	    /* If NumByteToWrite < I2C_PageSize */
	    if(NumOfPage== 0) //数据小于一页
	    {
            /* 开始页只能存count个字节，显然放不下，需要跨页存放 */
			if(NumByteToWrite > count)
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, count);//写少于一页的数据
				WriteAddr += count;
				pBuffer += count;
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite-count);//将剩下的数据写到下一页
			}
            /* 开始页可以放下所有的数据 */
			else
			{
				I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
	    }
	    /* If NumByteToWrite > I2C_PageSize */
	    else//数据大于等于一页
	    {
	      NumByteToWrite -= count;
	      NumOfPage = NumByteToWrite / I2C_PageSize; //重新计算要写入的页数
	      NumOfSingle = NumByteToWrite % I2C_PageSize;//重新计算不足一页的个数
	      
	      if(count != 0)//在此处count一定不为0，此判断条件好象有点多余
	      { 
	        I2C_EE_PageWrite(pBuffer, WriteAddr, count);//将开始的空间写满一页
	        WriteAddr += count;
	        pBuffer += count;
	      }
          
	      while(NumOfPage--)//要写入的页数
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);//写一页的数据
	        WriteAddr += I2C_PageSize;
	        pBuffer += I2C_PageSize;
	      }
          
	      if(NumOfSingle != 0)//剩余数据小于一页
	      {
	        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); //写少于一页的数据
	      }
	    }
	}
    
	I2C_EE_WriteProtect(1);
}
/*
 * 函数名：I2C_EE_ByteWrite
 * 描述  ：写一个字节到I2C EEPROM中
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址 
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr)//写一个字节到EEPROM
{
	I2C_SetAddr(WriteAddr);
	/* Send the byte to be written */
	I2Cwrite_byte_to24C512(*pBuffer);//发送数据
	I2C_Receive_ACK();
	/* Send STOP condition */
	I2C_Stop();//产生 I2Cx传输 STOP条件
}
/*
 * 函数名：I2C_EE_PageWrite
 * 描述  ：在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
 *         不能超过EEPROM页的大小。AT24C02每页有8个字节。
 * 输入  ：-pBuffer 缓冲区指针
 *         -WriteAddr 接收数据的EEPROM的地址 
 *         -NumByteToWrite 要写入EEPROM的字节数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite)//写少于一页的数据
{
	I2C_SetAddr(WriteAddr);
	/* While there is data to be written */
	while(NumByteToWrite--) 
	{
		I2Cwrite_byte_to24C512(*pBuffer);//发送数据
		I2C_Receive_ACK();
	    /* Point to the next byte to be written */
	    pBuffer++;
	}
	/* Send STOP condition */
	I2C_Stop();//产生 I2Cx传输 STOP条件
	if(I2C_Wait_Idle()!=I2C_OK)return;
}
/*
 * 函数名：I2C_EE_BufferRead
 * 描述  ：从EEPROM里面读取一块数据。 
 * 输入  ：-pBuffer 存放从EEPROM读取的数据的缓冲区指针。
 *         -WriteAddr 接收数据的EEPROM的地址。 
 *         -NumByteToWrite 要从EEPROM读取的字节数。
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 */
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)//将EEPROM的数据读入缓冲器
{
	I2C_SetAddr(ReadAddr);
	/* Send STRAT condition */
	I2C_Start();//产生 I2Cx传输 START条件
	/* Send EEPROM address for read */
	I2Cwrite_byte_to24C512(EEPROM_ADDRESS | 0x01);
	I2C_Receive_ACK(); //(sun)主机只发送脉冲，并不监测数据位
	while(NumByteToRead--) 
	{
		/* Read a byte from the EEPROM */
		*pBuffer = I2Cread_byte_from24C512();//返回通过 I2Cx最近接收的数据
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
		if(NumByteToRead > 0)
			I2C_Send_ACK();
	}
	I2C_Send_NOACK();
	/* Send STOP condition */
	I2C_Stop();//产生 I2Cx传输 STOP条件
}
void I2C_EE_WriteProtect(u8 wp)
{
	if(wp)
	{
		I2C_SETWP();
	}
	else
	{
		I2C_CLRWP();
	}
}
/*************************END OF FILE*************************************/
