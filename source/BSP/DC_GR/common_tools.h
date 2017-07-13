#ifndef __COMMON_TOOLS_H
#define __COMMON_TOOLS_H

#include "string.h"
#include "stm32f10x_conf.h"
#include "app_cfg.h"


void SplitCmdData(char *cmd_data , char receive[][20], uint8_t size);
void CmdStrCatDouble(char str[] , double d_data , uint8_t d_len , const uint8_t point_place);
void CmdStrCatInt(char str[] , uint16_t i_data , uint8_t i_len);
uint32_t StrToInt(char *str);


#endif
