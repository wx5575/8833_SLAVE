
#ifndef __AUTO_CALIBRATION_H__
#define __AUTO_CALIBRATION_H__


#ifndef NULL
#define NULL 0
#endif

typedef enum{
    /* ACW 0-12 */
    /* 电压 */
    ACW_VOL_SEG_11,
    ACW_VOL_SEG_12,
    ACW_VOL_SEG_13,
    ACW_VOL_SEG_21,
    ACW_VOL_SEG_22,
    ACW_VOL_SEG_23,
    /* 电流 */
    ACW_CUR_G_200uA,
    ACW_CUR_G_2mA,
    ACW_CUR_G_10mA,
    ACW_CUR_G_20mA,
    ACW_CUR_G_50mA,
    ACW_CUR_G_100mA,
    ACW_CUR_G_200mA,
    
    /* DCW 13-26 */
    /* 电压 */
    DCW_VOL_SEG_11,
    DCW_VOL_SEG_12,
    DCW_VOL_SEG_13,
    DCW_VOL_SEG_21,
    DCW_VOL_SEG_22,
    DCW_VOL_SEG_23,
    /* 电流 */
    DCW_CUR_G_2uA,
    DCW_CUR_G_20uA,
    DCW_CUR_G_200uA,
    DCW_CUR_G_2mA,
    DCW_CUR_G_10mA,
    DCW_CUR_G_20mA,
    DCW_CUR_G_50mA,
    DCW_CUR_G_100mA,
    
    /* IR 27-48 */
    /* 电压 */
    IR_VOL_SEG_11,
    IR_VOL_SEG_12,
    IR_VOL_SEG_13,
    IR_VOL_SEG_21,
    IR_VOL_SEG_22,
    IR_VOL_SEG_23,
    /* 电流 */
    IR_RES_G_11,
    IR_RES_G_12,
    IR_RES_G_13,
    IR_RES_G_21,
    IR_RES_G_22,
    IR_RES_G_23,
    IR_RES_G_31,
    IR_RES_G_32,
    IR_RES_G_33,
    IR_RES_G_41,
    IR_RES_G_42,
    IR_RES_G_43,
    IR_RES_G_51,
    IR_RES_G_52,
    IR_RES_G_53,
    IR_RES_G_54,
    
    /* GR 49-54 */
    /* 电流 */
    GR_CUR_SEG_11,
    GR_CUR_SEG_12,
    GR_CUR_SEG_13,
	
    GR_CUR_SEG_21,
    GR_CUR_SEG_22,
    GR_CUR_SEG_23,
    
    GR_CUR_SEG_31,
    GR_CUR_SEG_32,
    GR_CUR_SEG_33,
	
    GR_CUR_SEG_41,
    GR_CUR_SEG_42,
    GR_CUR_SEG_43,
	
    GR_CUR_SEG_51,
    GR_CUR_SEG_52,
    GR_CUR_SEG_53,
	
    GR_CUR_SEG_61,
    GR_CUR_SEG_62,
    GR_CUR_SEG_63,
    
    GR_CUR_SEG_71,
    GR_CUR_SEG_72,
    GR_CUR_SEG_73,
	
    GR_CUR_SEG_81,
    GR_CUR_SEG_82,
    GR_CUR_SEG_83,
	
    ORDER_SIZE,
}CAL_ORDER_EUME;
/* 校准数据的基本信息 */
typedef struct{
    uint8_t *name;/* 校准项目名称 */
    float ad;/* 采集的AD值 */
    float ad_vol;/* 采集的电压AD值 */
    float ad_cur;/* 采集的电流AD值 */
    float da;/* 输出的DA值 */
    float dis;/* 校准点的显示值 */
    uint8_t gear;/* 目标档位 */
    uint32_t measure;/* 测量值 */
}CAL_BASE_INFO;

/* 校准电压使用的数据结构 */
typedef struct{
    CAL_BASE_INFO base[3];
    float *da_k;/* 指向DA校准系数全局变量的k */
    float *da_b;/* 指向DA校准系数全局变量的b */
    float *ad_k;/* 指向AD校准系数全局变量的k */
    float *ad_b;/* 指向AD校准系数全局变量的b */
    void (*count_ratio)(void* p_this);/* 计算系数 */
    void (*save)(const uint8_t mode);/* 保存系数 */
}AUTO_CAL_VOL;

/* 校准接地电阻使用的数据结构 */
typedef struct{
    CAL_BASE_INFO base[3];
    float *da_k;/* 指向DA校准系数全局变量的k */
    float *da_b;/* 指向DA校准系数全局变量的b */
    float *ad_v_k;/* 指向AD校准系数全局变量的k */
    float *ad_v_b;/* 指向AD校准系数全局变量的b */
    float *ad_i_k;/* 指向AD校准系数全局变量的k */
    float *ad_i_b;/* 指向AD校准系数全局变量的b */
    void (*count_ratio)(void* p_this);/* 计算系数 */
    void (*save)(const uint8_t mode);/* 保存系数 */
}AUTO_CAL_GR_CUR;

/* 校准电流使用的数据结构 */
typedef struct{
    CAL_BASE_INFO base;
    float da_k;/* 用来计算DA值 */
    float da_b;/* 用来计算DA值 */
    float *ad_i_k;/* 指向校准系数全局变量的k */
    uint16_t output;/* 输出电压 */
    void (*count_ratio)(void* p_this);/* 计算系数 */
    void (*save)(const uint8_t mode);/* 保存系数 */
}AUTO_CAL_CUR;

/* 校准IR电阻使用的数据结构 */
typedef struct{
    CAL_BASE_INFO base;
    float *k;/* 指向校准系数全局变量的k */
    uint8_t count;/* 计数 */
    void (*count_ratio)(void* p_this);/* 计算系数 */
    void (*save)(const uint8_t mode);/* 保存系数 */
}AUTO_CAL_IR_RES;

/* 校准电流使用的数据结构 */
typedef union{
    AUTO_CAL_VOL vol;
    AUTO_CAL_CUR cur;
    AUTO_CAL_IR_RES ir_res;
    AUTO_CAL_GR_CUR gr;
}CAL_INFO_UN;


typedef struct{
    uint16_t num;/* 次序编号 */
    uint16_t type;/* 类型 */
    uint8_t mode;/* 测试模式 */
    void (*fun)(void);/* 处理函数 */
    uint32_t data;/* 用户数据 */
    uint32_t range;/* 数据范围 */
    void (*get_cal_info)(CAL_BASE_INFO *info);/* 获取校准点信息 */
}CAL_ORDER;

typedef struct{
    uint8_t state;/* 校准状态 */
    uint16_t cal_total_points;/* 总共的校准点数 */
}ATUTO_CAL_FLAG;

#ifdef   AUTO_CALIBRATE_GLOBALS
#define  AUTO_CALIBRATE_EXT
#else
#define  AUTO_CALIBRATE_EXT  extern
#endif

// AUTO_CALIBRATE_EXT uint8_t frame_buf[20];

AUTO_CALIBRATE_EXT CAL_ORDER *cal_order;//[ORDER_SIZE];
AUTO_CALIBRATE_EXT ATUTO_CAL_FLAG auto_cal;/* 自动校准使用的数据 */

AUTO_CALIBRATE_EXT float auto_measure;  /* 自动校准测量值 */

AUTO_CALIBRATE_EXT uint16_t g_cur_cal_opt_num;/* 当前校准项目编号 */
AUTO_CALIBRATE_EXT CAL_INFO_UN cal_info_un;/* 校准信息联合体 */

extern void select_cur_range_pool(uint8_t mode);
extern void auto_calibration(void);


#endif //__AUTO_CALIBRATION_H__

