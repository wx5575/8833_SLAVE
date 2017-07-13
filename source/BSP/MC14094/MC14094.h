/*
 * Copyright(c) 2013,
 * All rights reserved
 * 文件名称：MC14094.h
 * 摘  要  ：头文件
 * 当前版本：V1.0，孙世成编写
 * 修改记录：
 *
 */
#ifndef __MC14094_H
#define __MC14094_H


#if defined(__cplusplus)
    extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif
			
/******************* <Include> ********************/
#include "stm32f10x.h"

/******************* <define> *********************/
#define		MC14094_NUM				(3)
#define		MC14094_NUM_RA			(1)
#define		MC14094_NUM_PORT		(4)

#define		MC14094_A				(0)
#define		MC14094_B				(1)
#define		MC14094_C				(2)

#define		MC14094_X				(0)
#define		MC14094_Y				(1)
        
#define		MC14094_H1				(0)
#define		MC14094_H2				(1)
#define		MC14094_L1				(2)
#define		MC14094_L2				(3)

/*++++++++++++++++++ 4094A +++++++++++++++++++++*/
#define		MC14094_CD4053_A		(0x01)
#define		MC14094_CD4053_B		(0x02)
#define		MC14094_CD4053_C		(0x04)

#define		MC14094_A_VOL_SEL		(0x08)

#define		MC14094_PLC_TEST		(0x10)
#define		MC14094_PLC_FAIL		(0x20)
#define		MC14094_PLC_PASS		(0x40)

#define		MC14094_PRO_CHANGE		(0x80) //限幅保护电路


/*++++++++++++++++++ 4094B +++++++++++++++++++++*/
#define		MC14094_GFI_RELAY		(0x01)
#define		MC14094_AMP_RELAY3		(0x02)
#define		MC14094_AMP_RELAY2		(0x04)
#define		MC14094_AMP_RELAY1		(0x08)
#define		MC14094_GR_W			(0x10)
#define		MC14094_AC_DC			(0x20)
#define		MC14094_W_FILES			(0x40)
#define		MC14094_W_IR			(0x80)

/*++++++++++++++++++ 4094C +++++++++++++++++++++*/
#define		MC14094_C_10M			(0x04)
#define		MC14094_C_100M			(0x02)
#define		MC14094_C_1G			(0x01)
#define		MC14094_C_GR			(0x08)
#define		MC14094_C_10G			(0x10)
#define		MC14094_C_100G			(0x20)
#define		MC14094_C_100mA			(0x40)
#define		MC14094_C_GND_FLOAT		(0x80)

#define		MC14094_C_2uA			MC14094_C_10G
#define		MC14094_C_20uA			MC14094_C_1G
#define		MC14094_C_200uA			MC14094_C_100M

extern uint8_t MC14094_BUFFER[MC14094_NUM];
/******************* 函数声明 *********************/
extern void MC14094_Init(void);
extern void MC14094_CMD(u8 ID,u8 bits,u8 status);
extern void MC14094_CMD_BYTE(u8 ID, u8 status);
extern void MC14094_Updata(void);

/*********************************************************
*				液晶屏对比度
*********************************************************/
// #define CONTRAST_LOW	0XD0
// #define CONTRAST_HIGH	0XEF
extern void MC14094_CMD_RA(u8 data);
extern void MC14094_Updata_RA(void);
extern void MC14094_Updata_Port(void);

/*********************************************************
*   	        	多路端口
*********************************************************/

#define HV_H_PORT1      ((uint8_t)(0))
#define HV_H_PORT2      ((uint8_t)(1))
#define HV_H_PORT3      ((uint8_t)(2))
#define HV_H_PORT4      ((uint8_t)(3))
#define HV_H_PORT5      ((uint8_t)(4))
#define HV_H_PORT6      ((uint8_t)(5))
#define HV_H_PORT7      ((uint8_t)(6))
#define HV_H_PORT8      ((uint8_t)(7))


#define HV_L_PORT1      ((uint8_t)(7))
#define HV_L_PORT2      ((uint8_t)(6))
#define HV_L_PORT3      ((uint8_t)(5))
#define HV_L_PORT4      ((uint8_t)(4))
#define HV_L_PORT5      ((uint8_t)(3))
#define HV_L_PORT6      ((uint8_t)(2))
#define HV_L_PORT7      ((uint8_t)(1))
#define HV_L_PORT8      ((uint8_t)(0))

#define D1_Q1           ((uint8_t)(0))
#define D1_Q2           ((uint8_t)(1))
#define D1_Q3           ((uint8_t)(2))
#define D1_Q4           ((uint8_t)(3))
#define D1_Q5           ((uint8_t)(4))
#define D1_Q6           ((uint8_t)(5))
#define D1_Q7           ((uint8_t)(6))
#define D1_Q8           ((uint8_t)(7))

#define D2_Q1           ((uint8_t)(0))
#define D2_Q2           ((uint8_t)(1))
#define D2_Q3           ((uint8_t)(2))
#define D2_Q4           ((uint8_t)(3))
#define D2_Q5           ((uint8_t)(4))
#define D2_Q6           ((uint8_t)(5))
#define D2_Q7           ((uint8_t)(6))
#define D2_Q8           ((uint8_t)(7))

extern void reset_posrts_mc14094_ctrl(void);
extern void MC14094_CMD_Port(u8 ID,u8 bits,u8 status);
extern void MC14094_Updata_Port(void);

/**************************************************/
#if defined(__cplusplus)
    }
#endif 
/******************************************************************************
 *                             END  OF  FILE                                                                          
******************************************************************************/
#endif

