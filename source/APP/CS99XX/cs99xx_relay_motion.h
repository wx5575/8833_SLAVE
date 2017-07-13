/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_relay_motion.h
 * 摘  要  ：为通信程序提供处理函数
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_RELAY_MOTION__
#define __CS99XX_RELAY_MOTION__

#include "stm32f10x.h"
#include "cs99xx_struct.h"

#define RELAY_ON    1
#define RELAY_OFF   0


extern void shift_gear(uint8_t mode);
extern void ir_auto_find_gear(void);
extern void ir_auto_shift_gear(void);

extern void reset_ports(void);
extern void updata_port(TEST_PORT *p);
extern void set_sine_hz(const int8_t n);
extern void open_sine(float freq);
extern void close_sine(void);
extern void amp_relay_ctrl_on(void);
extern void amp_relay_ctrl_off(void);
extern void on_dc_gr_control_relay(void);

extern void dcw_big_cap_start_charge(void);
extern void dcw_big_cap_stop_charge(void);
extern void no_dcw_big_cap_stop_charge(void);

extern void dcw_big_cap_stop_dis_charge(void);
extern void dcw_big_cap_keep_dis_charge(void);
extern void dcw_big_cap_execute_dis_charge(void);
extern void dcw_big_cap_output_Impedan_sel(uint8_t n);
extern void dcw_big_cap_cal(void);

extern void test_ready(void);
extern void test_pass(void);
extern void test_fail(void);
extern void relay_ready(void);

#endif //__CS99XX_RELAY_MOTION__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
