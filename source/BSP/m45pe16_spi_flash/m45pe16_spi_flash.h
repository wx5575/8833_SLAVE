/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：m45pe16_spi_flash.h
 * 摘  要  ：主程序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
 
#ifndef __M45PE16_SPI_FLASH_H__
#define __M45PE16_SPI_FLASH_H__

typedef struct
{
	uint32_t ChipID;		/* 芯片ID */
	char ChipName[16];		/* 芯片型号字符串，主要用于显示 */
	uint32_t TotalSize;		/* 总容量 */
	uint16_t PageSize;		/* 页面大小 */
}SFLASH_INFO;

extern void init_m45pe16_spi_flash(void);
extern void m45_read_buffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
extern void m45_page_erase(uint32_t _uiReadAddr, uint32_t _uiSize);
extern void m45_page_write(uint8_t * _pBuf, uint32_t _uiWriteAddr);
extern void m45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);

#endif //__M45PE16_SPI_FLASH_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
