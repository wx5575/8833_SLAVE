/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：calibration.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__


/**************** 宏定义 ********************/

#define		EEP_MONITOR		0X5A5AA5A5

/**************** 数据结构定义 ********************/
#define MAX_SYN_TOTAL_NUM 60  //
typedef struct{
	float dac_k[2];/* dac电压系数 */
	float dac_b[2];
	float adc_v_k[2];/* 电压系数 */
	float adc_v_b[2];
    
	float adc_cur_k[10];
	
	float arc_k;/* 电弧校准值 在使用时要算一下系数k=arc_value/4095 */
	
	float adc_r_k;/* 真实电流系数 */
	float adc_r_b;
	
	float syn_adc_2mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_20mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_100mA[MAX_SYN_TOTAL_NUM];
    
    float free_space[10];/* 机动空间 以便增加数据 不影响整体大小 保持整个 结构size 不变 */
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_ACW;

typedef struct{
	float dac_k[2];/* dac电压系数 */
	float dac_b[2];
	float adc_v_k[2];/* 电压系数 */
	float adc_v_b[2];
    
	float adc_cur_k[10];
	
	float arc_k;/* 电弧校准值 在使用时要算一下系数k=arc_value/4095 */
	
	float adc_r_k;/* 真实电流系数 */
	float adc_r_b;
    
    float free_space[10];/* 机动空间 以便增加数据 不影响整体大小 保持整个 结构size 不变 */
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_BBD;

typedef struct{
	float dac_k[2];/* dac电压系数 */
	float dac_b[2];
	float adc_v_k[2];/* 电压系数 */
	float adc_v_b[2];
	
	float adc_cur_k[10];
	
	float arc_k;/* 电弧校准值 在使用时要算一下系数k=arc_value/4095 */
	
	float adc_a_b;
	float adc_r_k;/* 真实电流系数 */
	float adc_r_b;
    
	float syn_adc_2mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_20mA[MAX_SYN_TOTAL_NUM];
	float syn_adc_100mA[MAX_SYN_TOTAL_NUM];
    
    float free_space[10];/* 机动空间 以便增加数据 不影响整体大小 保持整个 结构size 不变 */
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_DCW;


typedef struct{
#define SECT_VOL     22  //电压采样点的最大值
#define CAL_POINTS   18  //校准点的数目最大值
    
	float dac_k[2];/* dac电压系数 */
	float dac_b[2];
	float adc_v_k[2];/* 电压系数 */
	float adc_v_b[2];
    
    float res_k[CAL_POINTS][SECT_VOL];/* 18个电阻采样点 22个电压采样点 电压100v-5000v */
    
    float free_space[212];/* 机动空间 以便增加数据 不影响整体大小 保持整个 结构size 不变 */
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_IR;

typedef struct{
#define GR_VOL_GEARS 8	/* 0 正常GR; 1 20mV; 2 200mV; 3 2000mV; 4 12V; 5 12V-100mA; 6 12V-1000mA */
	
	float dac_k[GR_VOL_GEARS];/* dac电压系数 */
	float dac_b[GR_VOL_GEARS];
	float adc_v_k[GR_VOL_GEARS];/* 电压系数 */
	float adc_v_b[GR_VOL_GEARS];
	float adc_i_k[GR_VOL_GEARS];/* 电流系数 */
	float adc_i_b[GR_VOL_GEARS];
    
    float free_space[10];/* 机动空间 以便增加数据 不影响整体大小 保持整个 结构size 不变 */
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_GR;


typedef struct{
	float dac_k;/* dac电压系数 */
	float dac_b;
	float adc_v_k;/* 电压系数 */
	float adc_v_b;
	float adc_i_k;/* 电流系数 */
	float adc_i_b;
    
    uint32_t crc_val;/* crc校验值 必须放在最后一个字段 */
}RATIO_DC_GR;

typedef struct {
	char *point_menu[8][5][2];/* 8 可以放8个变压器属性 5 */
	float point_da[8][5][2];
}VOL_CAL, CUR_CAL;

typedef struct {
	char *point_menu[5][16];/* 校准点 用于显示 */
	float point_da[5][16];/* 标准电阻值 */
    uint8_t gear[5][16];/* 对应的硬件档位 */
}RES_CAL;

/******************* 全局变量声明 *********************/
#ifdef   CALIBRATE_GLOBALS
#define  CALIBRATE_EXT
#else
#define  CALIBRATE_EXT  extern
#endif

CALIBRATE_EXT uint8_t cur_set_meal;/* 当前套餐 */
CALIBRATE_EXT uint8_t cur_cal_ir_res_meal;/* 当前套餐 IR 电阻套餐 */
CALIBRATE_EXT const VOL_CAL acw_vol_cal[][2];
CALIBRATE_EXT const VOL_CAL dcw_vol_cal[][2];
CALIBRATE_EXT const VOL_CAL ir_vol_cal[][2];
CALIBRATE_EXT const CUR_CAL gr_cur_cal[];
CALIBRATE_EXT const RES_CAL ir_cur_cal;
CALIBRATE_EXT const float default_ir_res_k[];

CALIBRATE_EXT RATIO_ACW ratio_acw;
CALIBRATE_EXT RATIO_DCW ratio_dcw;
CALIBRATE_EXT RATIO_IR ratio_ir;
CALIBRATE_EXT RATIO_GR ratio_gr;
CALIBRATE_EXT RATIO_DC_GR (*ratio_dc_gr)[5];
CALIBRATE_EXT RATIO_BBD ratio_bbd;


/******************* 函数声明 *********************/
extern uint8_t get_cal_dcw_vol_meal(void);
extern uint8_t get_cal_acw_vol_meal(void);
extern uint8_t get_cal_ir_meal(void);
extern uint8_t geat_ir_res_cal_points_num(void);
extern uint8_t get_ir_res_meal(void);
extern uint8_t select_gr_cal_menu(void);
extern void clear_menu(void);
extern void serve_numkey(const uint16_t keyvalue);
void startup_cal(const int8_t mode, const int8_t gear);
void stop_cal(const int8_t mode);

void init_ratio(uint8_t mode);
void init_ratio_all(void);
void correctness(void);
uint8_t is_acw_segment(void);
uint8_t is_dcw_segment(void);
uint8_t is_ir_segment(void);
void confirm_vol_segment_s(void);
void get_acw_vol_cal_point_range(uint32_t *range);
void get_dcw_vol_cal_point_range(uint32_t *range);
void get_ir_vol_cal_point_range(uint32_t *range);

#endif //__CALIBRATION_H__
 /******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
