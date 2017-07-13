#ifndef __I2C_EE_H
#define	__I2C_EE_H

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0 /* E2 = 1 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 2 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 3 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 4 */

#define I2C_Speed				400000
#define I2C_PageSize			128			/* AT24C512每页有128个字节 */
#define I2C_Dly					10
#define I2C_TimeOver			100
#define CHIP_SIZE				0X10000 //(64*1024)	/* 一个片子的容量 64k */

extern u16 EEPROM_ADDRESS;

enum{
	I2C_OK=0,
	I2C_Error_Timeover,
};

extern void I2C_EE_Init(void);
extern void I2C_GPIO_CAL_IN(void);
extern void I2C_EE_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
extern void I2C_EE_ByteWrite(u8* pBuffer, u16 WriteAddr);
extern void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite);
extern void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
extern void I2C_EE_WriteProtect(u8 wp);

extern void eep_buf_read(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead, uint8_t id);
extern void eep_buf_write(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite, uint8_t id);

#endif /* __I2C_EE_H */
