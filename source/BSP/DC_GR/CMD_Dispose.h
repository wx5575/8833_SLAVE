#ifndef __CMD_DISPOSE_H
#define __CMD_DISPOSE_H

#include <string.h>   
#include "DC_GR_Server.h"
#include "stm32f10x_conf.h"
#include "common_tools.h"
#include "USART.h"



/*处理接收到的指令*/
int32_t ReceiveCmdDispose(char  CmdStr[]);

void Send_Test_Method(uint8_t method);
void Send_Vol_Gear(uint32_t vol_gear);
void Send_Test_Start(uint32_t test_current);
void Send_Test_Stop(void);
void Send_Test_Time(uint32_t test_time);
void Send_Ask_Result(void);
void Send_Test_Calib(uint32_t CalibCurrent);


#endif  
