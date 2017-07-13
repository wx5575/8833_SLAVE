#ifndef __IIC_Config_H
#define __IIC_Config_H


#include "stm32f10x.h"


#define SCL_H          GPIO_SetBits(GPIOB , GPIO_Pin_6)
#define SCL_L          GPIO_ResetBits(GPIOB , GPIO_Pin_6)
   
#define SDA_H          GPIO_SetBits(GPIOB , GPIO_Pin_7)
#define SDA_L          GPIO_ResetBits(GPIOB , GPIO_Pin_7)

#define SDA_read       GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_7)

#define IIC_WR_Enable  GPIO_ResetBits(GPIOB , GPIO_Pin_5)//			//使能写
#define IIC_WR_Disable GPIO_SetBits(GPIOB , GPIO_Pin_5)				//进制写

#define IIC_ADDR   	  0xA0			 //器件地址
       



u8 I2C_Start(void);
void I2C_Stop(void);
static void I2C_Ack(void);
static void I2C_NoAck(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
u8 I2C_ReceiveByte(void);
u8 I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress);
u8 I2C_Write_N_Byte(u8* pBuffer, u16 length, u16 WriteAddress, u8 DeviceAddress);
u8 I2C_ReadByte(u8* pBuffer,   u16 length,   u16 ReadAddress,  u8 DeviceAddress);
	


#endif


