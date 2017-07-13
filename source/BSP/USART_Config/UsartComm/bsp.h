#ifndef __BSP_H
#define __BSP_H


// #include "SysTemConfig.h"


extern void bps_config(void);

extern uint8_t bspReadServerData(uint8_t * data);
extern void bspSendServerData(uint8_t * data, uint8_t data_num);

extern uint8_t bspReadClientData(uint8_t * data);
extern void bspSendClientData(uint8_t * data, uint8_t data_num);

extern void LED1_next_status(void);

extern void LED2_next_status(void);

#endif
