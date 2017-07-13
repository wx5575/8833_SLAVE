#ifndef _SPI_CPLD_h
#define _CPI_CPLD_h

#include "stm32f10x.h"

/* 命令 */
#define	CPLD_W					0X01000000	/* 写 */

#define CPLD_SET_FREQUENCY		0x01000000 /* 设置输出频率 其中低16位为频率控制字 */
#define CPLD_START_SINEWAVE		0x01020066 /* 启动正弦波输出 */
#define CPLD_STOP_SINEWAVE		0x01020000 /* 停止正弦波输出 */


#define CPLD_START				0X0101AAAA 	/* 启动计数器 */
#define CPLD_STOP				0X01015555	/* 停止计数 */
#define	CPLD_READ_PHASE			0X02000000 	/* 读相位差 */
#define CPLD_VOL_CUNT			0x02010000	/* 读电流计数值 */
#define CPLD_R_COUNT			0x02010000	/* 读电压波形计数 */
#define CPLD_COMM_CHECK			0x02020000	/* 读校验值，通信检查使用 返回 0x5555表示通信正常 */

#define COUNT_SIN_FREQ_WORD(F)	((F)*pow(2, 25)/pow(10, 6)) /* 计算频率关键字 */

/**************************多路高压板控制*********************************************/
#define CMD_CPLD_HV_H				0X01000000  /* 高端控制 */
#define CMD_CPLD_HV_L				0X01010000  /* 低端控制 */
/* 状态缓存 */
#define HV_PORT_H           (0)
#define HV_PORT_L           (1)

#define CPLD_HV_H1           ((uint8_t)(0))
#define CPLD_HV_H2           ((uint8_t)(1))
#define CPLD_HV_H3           ((uint8_t)(2))
#define CPLD_HV_H4           ((uint8_t)(3))
#define CPLD_HV_H5           ((uint8_t)(4))
#define CPLD_HV_H6           ((uint8_t)(5))
#define CPLD_HV_H7           ((uint8_t)(6))
#define CPLD_HV_H8           ((uint8_t)(7))

#define CPLD_HV_L1           ((uint8_t)(0))
#define CPLD_HV_L2           ((uint8_t)(1))
#define CPLD_HV_L3           ((uint8_t)(2))
#define CPLD_HV_L4           ((uint8_t)(3))
#define CPLD_HV_L5           ((uint8_t)(4))
#define CPLD_HV_L6           ((uint8_t)(5))
#define CPLD_HV_L7           ((uint8_t)(6))
#define CPLD_HV_L8           ((uint8_t)(7))
/*************************************************************************************/

extern void init_spi_cpld(void);

extern void reset_posrts_cpld_ctrl(void);
extern void cpld_write(u32 SendData);
extern u16 ReadDataFromCPLD(u32 SendData);

extern void hv_cpld_write(u32 SendData);
extern void cmd_cpld_hv_port(uint8_t ID, uint8_t bits, uint8_t status);
extern void cpld_count_angle(double *sin, double *cos);

#endif

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/

