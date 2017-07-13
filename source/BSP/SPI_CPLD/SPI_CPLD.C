#include "stm32f10x.h"
#include "SPI_CPLD.H"
#include "BSP_config.h"
#include "string.h"
#include 	<math.h>


#define     CPLD_DELAY      1
#define     CPLD_DELAY_F()      {int i = 0; for(i = 0; i < 10; i++);}

#define			SPI_CS_EN()							GPIO_ResetBits(GPIOG,GPIO_Pin_11)
#define			SPI_CS_DIS()						GPIO_SetBits(GPIOG,GPIO_Pin_11)
#define			SPI_MOSI_HIGH()						GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define			SPI_MOSI_LOW()						GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define			SPI_CLK_HIGH()						GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define			SPI_CLK_LOW()						GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define			SPI_MISO_DI()						GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12)


#define			SPI_HV_CS_EN()						GPIO_ResetBits(GPIOE,GPIO_Pin_0)
#define			SPI_HV_CS_DIS()		    			GPIO_SetBits(GPIOE,GPIO_Pin_0)
#define			SPI_HV_MOSI_HIGH()					GPIO_SetBits(GPIOE,GPIO_Pin_1)
#define			SPI_HV_MOSI_LOW()					GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define			SPI_HV_CLK_HIGH()					GPIO_SetBits(GPIOE,GPIO_Pin_6)
#define			SPI_HV_CLK_LOW()					GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define			SPI_HV_MISO_DI()					() /* 不需要接收数据 */

void HV_SPI_CPLD_GPIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  //允许GPIOE时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void SPI_CPLD_GPIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);  //允许GPIOE时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			 //推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 				//上拉输入
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
}

void init_spi_cpld(void)
{
    SPI_CPLD_GPIO();
    HV_SPI_CPLD_GPIO();
}
//*************************************************************

/* 函数名：cpld_write
 * 描述  ：往CPLD写命令，并返回一字节
 * 输入  ：命令
 * 输出  ：返回值
 */
//**************************************************************
void cpld_write(u32 SendData)
{
	u8  ShiftCounter;
	u32 DataBuffer;
	
	SPI_CS_DIS();
    CPLD_DELAY_F();;
	
	SPI_CS_EN(); //CS=0;片选CPLD
    CPLD_DELAY_F();;
	
	for(ShiftCounter=0;ShiftCounter<26;ShiftCounter++)
	{
		DataBuffer = SendData;
		DataBuffer = DataBuffer & 0x00000001; //取数据最低位
		SendData = SendData >>1; //数据右移一位
		SPI_CLK_HIGH(); //时钟置高
		
		if(DataBuffer == 0x00000000)
		{
            SPI_MOSI_LOW();
		}
		else
		{
            SPI_MOSI_HIGH();
		}
        CPLD_DELAY_F();;
		
        SPI_CLK_LOW(); //数据写入CPLD
        CPLD_DELAY_F();;
	}
    
    CPLD_DELAY_F();
    
	SPI_CS_DIS(); //CS=1;取消片选
}

uint8_t cpld_hv_buf[2];

void cmd_cpld_hv_port(uint8_t ID, uint8_t bits, uint8_t status)
{
	status != 0? (cpld_hv_buf[ID] |= 1<<bits) : (cpld_hv_buf[ID] &= ~(1<<bits));
}
void reset_posrts_cpld_ctrl(void)
{
    memset(cpld_hv_buf, 0, sizeof cpld_hv_buf);
}
/* 从cpld读出电压电流相位差并计算出正弦值和余弦值 */
void cpld_count_angle(double *sin_v, double *cos_v)
{
    uint16_t cpld_data1 = 0;
    uint16_t cpld_data2 = 0;
	double cos_val = 0.0;
	double sin_val = 0.0;
    double PI = 3.1415926;
    
    cpld_data1 = ReadDataFromCPLD(CPLD_READ_PHASE);	//读数相位差
    cpld_data2 = ReadDataFromCPLD(CPLD_VOL_CUNT);//读电压计数值
    
    if(cpld_data2 > 0)
    {
        sin_val = sin((double)cpld_data1 / cpld_data2 * PI);
        cos_val = cos((double)cpld_data1 / cpld_data2 * PI);
    }
    
    *sin_v = sin_val;
    *cos_v = cos_val;
}

void hv_cpld_write(u32 SendData)
{
    #define CPLD_CMD_DELAY    5
	u8  ShiftCounter;
	u32 DataBuffer;
	
    if(SendData == CMD_CPLD_HV_H)
    {
        SendData |= cpld_hv_buf[HV_PORT_H];
    }
    else
    {
        SendData |= cpld_hv_buf[HV_PORT_L];
    }
    
	SPI_HV_CS_DIS();
    CPLD_DELAY_F();
	
	SPI_HV_CS_EN(); //CS=0;片选CPLD
    CPLD_DELAY_F();
	
	for(ShiftCounter=0;ShiftCounter<26;ShiftCounter++)
	{
		DataBuffer = SendData;
		DataBuffer = DataBuffer & 0x1; //取数据最低位
		SendData = SendData >>1; //数据右移一位
		SPI_HV_CLK_HIGH(); //时钟置高
        CPLD_DELAY_F();
		
		if(DataBuffer == 0x0)
		{
            SPI_HV_MOSI_LOW();
		}
		else
		{
            SPI_HV_MOSI_HIGH();
		}
        
        CPLD_DELAY_F();
		
		SPI_HV_CLK_LOW(); //数据写入CPLD
        CPLD_DELAY_F();
	}
    
	SPI_HV_CS_DIS(); //CS=1;取消片选
    CPLD_DELAY_F();
}

//从CPLD中读数据
u16 ReadDataFromCPLD(u32 SendData)
{
	u8  ShiftCounter;
	u16 DataBuffer=0;
	u32 SendDataBuffer;
	
	SendDataBuffer   = SendData;
	cpld_write(SendDataBuffer); //给CPLD发送读命令
	
    CPLD_DELAY_F();
	SPI_CS_EN(); //片选
	DataBuffer = 0;
	
	for(ShiftCounter=0;ShiftCounter<16;ShiftCounter++)
	{
		DataBuffer = DataBuffer << 1; //左移一位
        
		SPI_CLK_HIGH(); //时钟置高
        CPLD_DELAY_F();
        
		SPI_CLK_LOW();
        CPLD_DELAY_F();;
		
		if(SPI_MISO_DI() == 1)
		{
			DataBuffer = DataBuffer | 0x0001; //低位置1
		}
		else
		{
			DataBuffer = DataBuffer & 0xfffe; //低位置0
		}
	}
	
	SPI_CS_DIS();
    CPLD_DELAY_F();
	
	return(DataBuffer);
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
