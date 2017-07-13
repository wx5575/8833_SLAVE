/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：on_chip_flash_api.h
 * 摘  要  ：片上flash编程接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __ON_CHIP_FLASH_H__
#define __ON_CHIP_FLASH_H__


#define F_PAGE_SIZE_I   (1024*2)      //片上flash页大小

#define TYPE_NUM_ADDR       0x0807F800  /* 型号地址 */
#define SYS_LANGUAGE_ADDR   0x0807F000  //系统语言 CHINESE,ENGLISH
#define EXCEP_INFO_ADDR     0x0807E800  //异常信息存储位置

#define TYPE_POOL_ADDR      0x0804ea48  //型号池地址


#define MAX_EXCE_INFO_NUM   ((F_PAGE_SIZE_I - 2) / sizeof(EXCEP_INFO_STR))           /* 系统支持最多3条错误记录可以取得值是 1， 2 ，3 */

extern uint16_t on_chip_flash_page_buf[F_PAGE_SIZE_I/2];

extern void erase_pages_flash(uint32_t addr, uint32_t pages);
extern void write_u16_flash(uint32_t addr, uint16_t *buf, uint16_t size);
extern void write_one_page_bin(uint16_t *buf,uint32_t addr);

#endif //__ON_CHIP_FLASH_H__

/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE****/
