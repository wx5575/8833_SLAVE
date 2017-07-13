/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：MC14094.c
 * 摘  要  ：主程序
 * 当前版本：V1.0，孙世成编写
 * 修改记录：2014.8.12 王鑫增加了液晶对比度的控制
 *                     王鑫增加了多路端口的控制
 *
 */

#include "MC14094.h"
#include "string.h"

/* 测试控制 */
#define MC14094_SETCLK()				(GPIO_SetBits(GPIOB,GPIO_Pin_9))
#define MC14094_CLRCLK()				(GPIO_ResetBits(GPIOB,GPIO_Pin_9))
#define MC14094_SETDATA()				(GPIO_SetBits(GPIOB,GPIO_Pin_6))
#define MC14094_CLRDATA()				(GPIO_ResetBits(GPIOB,GPIO_Pin_6))
#define MC14094_SETST()					(GPIO_SetBits(GPIOB,GPIO_Pin_7))
#define MC14094_CLRST()					(GPIO_ResetBits(GPIOB,GPIO_Pin_7))

/* 液晶对比度调节 以RA结尾 */
#define MC14094_SETCLK_RA()				(GPIO_SetBits(GPIOG,GPIO_Pin_9))
#define MC14094_CLRCLK_RA()				(GPIO_ResetBits(GPIOG,GPIO_Pin_9))
#define MC14094_SETDATA_RA()			(GPIO_SetBits(GPIOB,GPIO_Pin_11))
#define MC14094_CLRDATA_RA()			(GPIO_ResetBits(GPIOB,GPIO_Pin_11))
#define MC14094_SETST_RA()				(GPIO_SetBits(GPIOB,GPIO_Pin_10))
#define MC14094_CLRST_RA()				(GPIO_ResetBits(GPIOB,GPIO_Pin_10))

/* 测试端口控制 */
#define MC14094_SETCLK_PORT()			(GPIO_SetBits(GPIOE,GPIO_Pin_6))
#define MC14094_CLRCLK_PORT()			(GPIO_ResetBits(GPIOE,GPIO_Pin_6))
#define MC14094_SETDATA_PORT()			(GPIO_SetBits(GPIOE,GPIO_Pin_1))
#define MC14094_CLRDATA_PORT()			(GPIO_ResetBits(GPIOE,GPIO_Pin_1))
#define MC14094_SETST_PORT()			(GPIO_SetBits(GPIOE,GPIO_Pin_0))
#define MC14094_CLRST_PORT()			(GPIO_ResetBits(GPIOE,GPIO_Pin_0))

#define MC14094_Dly					 360 // (720 * 1)//(720) // 

uint8_t 	MC14094_BUFFER[MC14094_NUM];
uint8_t		MC14094_BUFFER_RA[MC14094_NUM_RA];/* 液晶对比度 */
uint8_t		MC14094_BUFFER_PORT[MC14094_NUM_PORT];/* 测试端口 */

/*
 * 函数名：MC14094_GPIO_Config
 * 描述  ：引脚配置
 * 输入  ：无
 * 输出  ：无
 */
static void MC14094_GPIO_Config(void)
{
	/* 声明结构体变量 */
	GPIO_InitTypeDef GPIO_InitStructure;
    
	/* 开对应IO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE);
	/* 配置IO */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
    
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化端口B
	
	MC14094_CLRDATA_PORT();
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;/* 串行时钟 */
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*
 * 函数名：MC14094_Delay
 * 描述  ：阻塞延时
 * 输入  ：无
 * 输出  ：无
 */
static void MC14094_Delay(u32 t)
{
	while(t--);
}

/*
 * 函数名：MC14094_WriteByte
 * 描述  ：向4094写一个字节数据
 * 输入  ：d 要写的数据
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_WriteByte(u8 d)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		MC14094_CLRCLK();
		if(d & 0x80)
		{
			MC14094_SETDATA();
		}
		else
		{
			MC14094_CLRDATA();
		}
		MC14094_Delay(MC14094_Dly);
		MC14094_SETCLK();
		MC14094_Delay(MC14094_Dly);
		d=d<<1;
	}
}

/*
 * 函数名：MC14094_WriteByte
 * 描述  ：向4094写n个字节数据
 * 输入  ：p 指向数据缓存
 *         n 要写入数据的个数
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_Write(u8 *p, u8 n)
{
	p += (n - 1);
	MC14094_CLRST();
    
	for(; n > 0; n--)
	{
		MC14094_WriteByte(*p--);
	}
    
	MC14094_SETST();
    MC14094_Delay(MC14094_Dly);
    MC14094_Delay(MC14094_Dly);
    MC14094_Delay(MC14094_Dly);
	MC14094_CLRST();
}

/*
 * 函数名：MC14094_WriteByte
 * 描述  ：设置4094缓存的指定位状态
 * 输入  ：ID 级联4094的ID 
 *         bit 指定的位
 *         status 要设置的状态
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_CMD(u8 ID,u8 bits,u8 status)
{
	status != 0? (MC14094_BUFFER[ID] |= bits) : (MC14094_BUFFER[ID] &= ~bits);
}

/*
 * 函数名：MC14094_CMD_BYTE
 * 描述  ：设置4094缓存的指定chip的状态
 * 输入  ：ID 级联4094的ID 
 *         status 要设置的状态
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_CMD_BYTE(u8 ID, u8 status)
{
	MC14094_BUFFER[ID] = status;
}

/*
 * 函数名：MC14094_Updata
 * 描述  ：将4094缓存中的数据写入到硬件中
 * 输入  ：无
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_Updata(void)
{
	MC14094_Write(MC14094_BUFFER,MC14094_NUM);
}

/*
 * 函数名：MC14094_Init
 * 描述  ：MC14094初始化
 * 输入  ：无
 * 输出  ：无
 */
void MC14094_Init(void)
{
	u8 i;
	MC14094_GPIO_Config();
	for(i=0;i<MC14094_NUM;i++)MC14094_BUFFER[i]=0;
	MC14094_Write(MC14094_BUFFER,MC14094_NUM);
    
    reset_posrts_mc14094_ctrl();
}

/*********************************************************
*				液晶屏对比度设置
*********************************************************/

/*
 * 函数名：MC14094_WriteByte_RA
 * 描述  ：向4094写一个字节数据
 * 输入  ：d 要写的数据
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_WriteByte_RA(u8 d)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		MC14094_CLRCLK_RA();
		if(d & 0x80)
		{
			MC14094_SETDATA_RA();
		}
		else
		{
			MC14094_CLRDATA_RA();
		}
		MC14094_Delay(MC14094_Dly);
		MC14094_SETCLK_RA();
		MC14094_Delay(MC14094_Dly);
		d=d<<1;
	}
}

/*
 * 函数名：MC14094_Write_RA
 * 描述  ：向4094写n个字节数据
 * 输入  ：p 指向数据缓存
 *         n 要写入数据的个数
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_Write_RA(u8 *p, u8 n)
{
	p += (n-1);
    
	MC14094_CLRST_RA();
	for(;n>0;n--)
	{
		MC14094_WriteByte_RA(*p--);
	}
    
	MC14094_SETST_RA();
    MC14094_Delay(MC14094_Dly);
	MC14094_CLRST_RA();
}
/*
 * 函数名：MC14094_CMD_RA
 * 描述  ：设置4094缓存的指定chip的状态
 * 输入  ：status 要设置的状态
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_CMD_RA(u8 status)
{
	MC14094_BUFFER_RA[0] = status;
}
/*
 * 函数名：MC14094_Updata_RA
 * 描述  ：将4094缓存中的数据写入到硬件中
 * 输入  ：无
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_Updata_RA(void)
{
	MC14094_Write_RA(MC14094_BUFFER_RA,MC14094_NUM_RA);
}

/*********************************************************
*				端口设置继电器控制
*********************************************************/

/*
 * 函数名：MC14094_WriteByte_Port
 * 描述  ：向4094写一个字节数据
 * 输入  ：d 要写的数据
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_WriteByte_Port(u8 d)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		MC14094_CLRCLK_PORT();
		if(d & 0x80)
		{
			MC14094_SETDATA_PORT();
		}
		else
		{
			MC14094_CLRDATA_PORT();
		}
		MC14094_Delay(MC14094_Dly);
		MC14094_SETCLK_PORT();
		MC14094_Delay(MC14094_Dly);
		d=d<<1;
	}
}

/*
 * 函数名：MC14094_Write_Port
 * 描述  ：向4094写n个字节数据
 * 输入  ：p 指向数据缓存
 *         n 要写入数据的个数
 * 输出  ：无
 * 返回  : 无
 */
static void MC14094_Write_Port(u8 *p, u8 n)
{
	p += (n-1);
	MC14094_CLRST_PORT();
	for(;n>0;n--)
	{
		MC14094_WriteByte_Port(*p--);
	}
	MC14094_SETST_PORT();
}

/*
 * 函数名：MC14094_CMD_Port
 * 描述  ：设置4094缓存的指定位状态
 * 输入  ：ID 级联4094的ID 
 *         bit 指定的位
 *         status 要设置的状态
 * 输出  ：无
 * 返回  : 无
 */
void MC14094_CMD_Port(u8 ID, u8 bits, u8 status)
{
	status != 0? (MC14094_BUFFER_PORT[ID] |= 1<<bits) : (MC14094_BUFFER_PORT[ID] &= ~(1<<bits));
}

/*
 * 函数名：reset_posrts_mc14094_ctrl
 * 描述  ：清空4094缓存中数据 并 更新到硬件
 * 输入  ：无
 * 输出  ：无
 * 返回  : 无
 */
void reset_posrts_mc14094_ctrl(void)
{
    memset(MC14094_BUFFER_PORT, 0, sizeof MC14094_BUFFER_PORT);
    MC14094_Updata_Port();
}
/*
 * 函数名：MC14094_Updata_Port
 * 描述  ：更新端口寄存器状态
 * 输入  ：无
 * 输出  ：无
 * 返回	 ：无
 */
void MC14094_Updata_Port(void)
{
	MC14094_Write_Port(MC14094_BUFFER_PORT, MC14094_NUM_PORT);
}



