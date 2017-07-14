/**
  ******************************************************************************
  * @file    crc.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   CRC计算
  ******************************************************************************
  */
#ifndef __CRC_H__
#define __CRC_H__

#include "sys_level.h"

extern uint16_t get_crc16(uint8_t *puchMsg,uint16_t usDataLen);
extern uint8_t get_crc8(uint8_t *ptr, uint16_t len);
extern uint32_t soft_crc32_byte(uint8_t *ptr, uint32_t len);
extern uint32_t soft_crc32_word(uint32_t *ptr, uint32_t len);
extern uint32_t stm32_crc32_word(uint32_t *pBuff, uint32_t len);
extern uint32_t stm32_crc32_byte(uint8_t *pBuff, uint32_t len);

#endif //__TOOLS_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
