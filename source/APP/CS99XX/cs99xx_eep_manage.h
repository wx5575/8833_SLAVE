/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_eep_manage.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_EEP_MANAGE_H__
#define __CS99XX_EEP_MANAGE_H__

#include "stm32f10x.h"

/******************************* AT24C512***A0 地址分配 *********************************/

#define		MEM_GROUP_BASE				((uint16_t)(0x2000))	/* 存放记忆组基地址 A0可以存19个记忆组其中第一个是默认记忆组 */
#if SYNCHRO_ROADS_EN>0
// 	#define		MEM_GROUP_BASE				((uint16_t)(0x800))	/* 存放记忆组基地址 A0可以存19个记忆组其中第一个是默认记忆组 */
	#define		SYN_ONE_SIZE				((uint16_t)sizeof(SYN_ROADS))
	#define		UN_ONE_SIZE					((uint16_t)sizeof(UN_STRUCT))
#else
	#define		SYN_ONE_SIZE				((uint16_t)0)
	#define		UN_ONE_SIZE					((uint16_t)sizeof(UN_STRUCT))
#endif

#define         E_NODE_SIZE                 ((uint16_t)UN_ONE_SIZE + SYN_ONE_SIZE)

// #if SYNCHRO_ROADS_EN>0
// 	#define		SYN_ROADS_BASE				((uint16_t)(MEM_GROUP_BASE+UN_ONE_SIZE)) /* 多路同步数据的基址 */
// 	#define 	STEP_OFFSET					((uint16_t)(UN_ONE_SIZE+SYN_ONE_SIZE)) /* 单步所占的空间，可能会增删成员，因此用sizeof */
// 	#define 	MEM_GROUP_OFFSET			((uint16_t)STEP_OFFSET*MAX_STEPS)	/* 单个记忆组占32*99个字节 */
// #else#endif
    
    #define     COUNT_REMAIM_SPACE(a,b)     (((a) < (b))? ((b) - (a)):0)
	#define 	MAX_FILES_A0				((uint16_t)((CHIP_SIZE - MEM_GROUP_BASE)/MEM_GROUP_OFFSET))/* 在A0片上能放19(0-18)个记忆组 1个默认记忆组 */
	#define 	MAX_FILES_A1				((uint16_t)(CHIP_SIZE/MEM_GROUP_OFFSET)) /* 在A1片上能放12个记忆组 */
	#define 	MAX_FILES_A2				((uint16_t)(COUNT_REMAIM_SPACE(MAX_FILES_A0 + MAX_FILES_A1 , MAX_FILES))) /* 在A1片上能放12个记忆组 */
	
    #define 	STEP_OFFSET					((uint16_t)sizeof(UN_STRUCT)) /* 单步所占的空间，可能会增删成员，因此用sizeof */
	#define 	MEM_GROUP_OFFSET			((uint16_t)STEP_OFFSET*MAX_STEPS)	/* 单个记忆组占32*99个字节 */
    
    
	#define 	SYS_PAR_BASE				((uint16_t)(0X0)) /* 系统参数的基址 */
	#define		INTERVAL_SPACE				((uint16_t)(0x00)) /* 不同参数之间存放的间距最小为0 容错空间 */
	
	#define 	SYS_FLAG_EE_ADDR			((uint16_t)SYS_PAR_BASE)	/* 存放标志 信息 */
	#define		SYS_FLAG_SIZE				((uint16_t)sizeof(SYS_FLAG))
	
	#define		SYS_PAR_EE_ADDR				((uint16_t)SYS_FLAG_EE_ADDR + SYS_FLAG_SIZE + INTERVAL_SPACE) /* 存放系统参数 占37个字节(可能会有变更)  */
	#define		SYS_PAR_SIZE				((uint16_t)sizeof(SYS_PAR))
	
//                     ||
//                     ||
//                     \/
//                 26字节空间
    
    #define     FILE_USED_STEP_FLAG_OFFSET  ((uint16_t)(sizeof step_used_flag))
    #define     FILE_USED_STEP_FLAG_BASE    ((uint16_t)FILE_STEP_TABLE_BASE - FILE_USED_STEP_FLAG_OFFSET * MAX_FILES)
    
    #define     FILE_STEP_TABLE_BASE        ((uint16_t)FILE_INFO_BASE-(2*MAX_STEPS*MAX_FILES))
    #define     E_GROP_TABLE_SIZE           ((uint16_t)(sizeof cur_group_table))
    #define     E_GROP_TABLE_OFFSET         ((uint16_t)(sizeof cur_group_table))
	
    #define 	FILE_INFO_BASE				((uint16_t)MEM_GROUP_BASE-(FILE_INFO_SIZE + INTERVAL_SPACE))	/* 存放文件 头信息 大约占0x5a0字节 其中第一个为默认的文件信息*/
	#define 	FILE_INFO_OFFSET			((uint16_t)sizeof(TEST_FILE))	/* 单个文件头占的空间 成员可能会变动因此使用sizeof */
	#define		FILE_INFO_SIZE				((uint16_t)FILE_INFO_OFFSET * MAX_FILES)

/******************************* AT24C512***A1 地址分配 *********************************/
#define		MEM_GROUP_BASE_A1				((uint16_t)0x0)

/******************************* AT24C512***A2 地址分配 *********************************/
#define		MEM_GROUP_BASE_A2				((uint16_t)(MEM_GROUP_OFFSET * MAX_FILES_A2))

#define 	CHECK_CHIP2_BASE				((uint16_t)(MEM_GROUP_BASE_A2 + 0x8))/* EEP检验用 */
#define 	RESULT_OFFSET					((uint16_t)sizeof(RESULT_STR))   /* 结果偏移值 */
#define     RESULT_LIST_DIS_BASE            ((uint16_t)CHECK_CHIP2_BASE)    /* 列表显示基址 */
#define     RESULT_LIST_DIS_TOTAL           ((uint16_t)(RESULT_OFFSET * MAX_STEPS))

#define		RESULT_BASE_A2					((uint16_t)(RESULT_LIST_DIS_BASE + RESULT_LIST_DIS_TOTAL)) /* 存储测试结果 */

#define		RESULT_AMOUNT_A2				((uint16_t)(CHIP_SIZE - RESULT_BASE_A2)/RESULT_OFFSET)
#define		RESULT_TOTAL_SIZE				((uint16_t)(RESULT_AMOUNT_A2 * RESULT_OFFSET))

#define		E_RES_TOTAL		                ((uint16_t)(RESULT_AMOUNT_A2/20*20))

/******************************* AT24C512***A3 地址分配 *********************************/
#define 	RATIO_ACW_BASE					((uint16_t)0x0)         /* 存放ACW校准系数 */
#define		RATIO_DCW_BASE					((uint16_t)1*1024)      /* 存放DCW校准系数 */
#define		RATIO_GR_BASE					((uint16_t)3*1024)      /* 存放GR校准系数 */

#define		RATIO_IR_BASE					((uint16_t)4*1024)      /* 存放IR校准系数 */
#define		RATIO_BBD_BASE					((uint16_t)8*1024)      /* 存放BBD校准系数 */

/*****************************************************************************************/
extern void save_one_step_to_eep(NODE_STEP	*node, uint8_t file_num, uint8_t step);
extern void read_one_step_to_eep(NODE_STEP	*node, uint8_t file_num, uint8_t step);

extern void save_list_to_eep(void);
extern void copy_cur_file_to_new_pos_eep(const int8_t n);

extern void init_group_step_used_flag_eep(void);
extern void init_file_step_table_eep(void);

extern int32_t read_ratio_from_eep(void);

extern int32_t read_paramenter_from_eep(void);

extern void save_as_cur_group_eep(const uint8_t file_num);
extern void save_init_group_to_eep(const uint8_t n, NODE_STEP *p_node);

extern void read_one_res_from_eep(RESULT_STR *res, const uint16_t n);

extern int32_t save_file_to_eep(const int8_t n);
extern void save_all_file_to_eep(void);

extern void read_file_to_eep(const int8_t n);
extern void read_all_file_to_eep(void);

extern void save_sys_flag_to_eep(void);
extern void save_sys_par_to_eep(void);

extern void read_sys_flag_to_eep(void);
extern void read_sys_par_from_eep(void);

extern void save_ratio_to_eep(const int8_t mode);
extern void read_ratio_to_eep(const int8_t mode);

extern void save_group_table_eep(const uint8_t file_num);
extern void save_step_used_flag_eep(const uint8_t file_num);

extern void read_group_table_eep(const uint8_t file_num);
extern void read_step_used_flag_eep(uint8_t n);

extern void clear_file_offset_addr_table_eep(uint8_t n);

extern uint8_t get_first_step_mode_eep(void);
extern void load_steps_to_list_eep(const uint16_t step);

extern void save_one_result_list_dis_eep(RESULT_STR *res, uint16_t n);
extern void save_one_result_eep(RESULT_STR *res, uint16_t n);
extern void read_one_result_eep(RESULT_STR *res, uint16_t n);
extern void read_result_list_dis_one_page_eep(RESULT_STR *res, uint16_t page, uint16_t cur_page_num);
extern void read_result_list_dis_one_eep(RESULT_STR *res, uint16_t n);

#endif //__CS99XX_EEP_MANAGE_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
