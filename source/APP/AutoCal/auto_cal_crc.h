
#ifndef __AUTO_CAL_CRC_H__
#define __AUTO_CAL_CRC_H__

extern uint8_t Protocol_CRC_Check(uint8_t *data, uint16_t len);
extern uint16_t ComputeCRC16(uint8_t *databuf, uint8_t length);

#endif //__AUTO_CAL_CRC_H__
