/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：serve_test.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __SERVE_TEST_H__
#define __SERVE_TEST_H__


/******************************* 加载文件 *********************************/
#include "stm32f10x.h"
#include "STM32_GPIO_Config.h"                  /* 引入位绑定 */
#include "cs99xx_struct.h"
#include "cs99xx_parameter.h"
#include "i2c_ee.h"

/******************************* 宏定义 *********************************/




/* 带保护的刷屏 */
#define LCD_REFRESH() {/*OS_CPU_SR cpu_sr;OS_ENTER_CRITICAL();*/my_refresh_lcd24064();/*OS_EXIT_CRITICAL();*/}


/* 测试程序使用 */
#define PI				((float)3.14159)
#define ROUND			((float)0.5)            /* 用于AD四舍五入  */

/* 计算指定小数点位数后的数据长度 */
#define COUNT_STR_LON_1DECS(Data)       (Data < 100? 3 : (Data < 1000? 4 : 5))
#define COUNT_STR_LON_2DECS(Data)       (Data < 1000 ? 4 : 5)
#define COUNT_LONG(data, decs)          (decs == 1? COUNT_STR_LON_1DECS(data) : (decs==2? COUNT_STR_LON_2DECS(data) : 5))

#define COUNT_FLOAT_LON_1DECS(Data)     (Data < 10? 3 : (Data < 100? 4 : 5))
#define COUNT_FLOAT_LON_2DECS(Data)     (Data < 10? 4 : 5)
#define COUNT_FLOAT_LON(data, decs)     (decs == 0? 4 : (decs == 1? COUNT_FLOAT_LON_1DECS(data) : (decs == 2? COUNT_FLOAT_LON_2DECS(data) : 5)))

#define COUNT_FLOAT_DECS(data)          (data < 10? 3 : ((data) < 100? 2 : ((data) < 1000? 1 : 0)))

        
        
/* 程序中各种档位定义 */
enum{
	/* 交流档位 */
	AC_2uA		= 0X1,
	AC_20uA		= 0X2,
	AC_200uA	= 0X3,
	AC_2mA		= 0X4,
	AC_10mA		= 0X5,
	AC_20mA		= 0X6,
	AC_50mA		= 0x7,
	AC_100mA	= 0X8,
	AC_200mA	= 0X9,
    AC_GEAR_END,
	
	/* 直流电流档位 */
	DC_2uA		= 0X1,
	DC_20uA		= 0x2,
	DC_200uA	= 0X3,
	DC_2mA		= 0x4,
	DC_10mA		= 0X5,
	DC_20mA		= 0X6,
	DC_50mA		= 0X7,
	DC_100mA	= 0X8,
    DC_GEAR_END,
	
	/* 绝缘电阻档位 */
	IR_10MOHM	= 0X01,
	IR_100MOHM	= 0X02,
	IR_1GOHM	= 0x03,
	IR_10GOHM	= 0X04,
	IR_100GOHM	= 0X05,
	
	/* 直流接地电阻档位 */
	DC_GR_20mV	= 0x2,
	DC_GR_200mV	= 0x3,
	DC_GR_2000mV= 0X4,
	DC_GR_12V	= 0X5,
	DC_GR_12V_100mA  = 0x6,
	DC_GR_12V_1000mA = 0x7,
	
	/* 通信时档位分为4个 */
	AC_0_GEAR_COMM	= 0X0,
	AC_1_GEAR_COMM	= 0X1,
	AC_2_GEAR_COMM	= 0X2,
	AC_3_GEAR_COMM	= 0X3,
	
	/* 通信时档位分为6个 */
	DC_0_GEAR_COMM	= 0X0,
	DC_1_GEAR_COMM	= 0X1,
	DC_2_GEAR_COMM	= 0X2,
	DC_3_GEAR_COMM	= 0X3,
	DC_4_GEAR_COMM	= 0X4,
	DC_5_GEAR_COMM	= 0X5,
	
	/* 通信时IR档位分为6个 */
	IR_1_GEAR_COMM	= 0X1,
	IR_2_GEAR_COMM	= 0X2,
	IR_3_GEAR_COMM	= 0X3,
	IR_4_GEAR_COMM	= 0X4,
	IR_5_GEAR_COMM	= 0X5,
	IR_6_GEAR_COMM	= 0X6,
};

/* 程序中标志状态 */
enum{
    /* DC_GR运行状态 */
	DC_GR_STOPING   = 0,
	DC_GR_RUNING    = 1,
    
    /* 测试运行状态 */
    TEST_STOPING    = 0,
    TEST_RUNING     = 1,
    
    /* IR自动切换档位时使用 */
    GEAR_UP         = 0,    /* ir向上换挡 */
    GEAR_DOWN       = 1,    /* ir向下换档 */
    
    /* 调用保存结果的函数时要给出的标志 INIT_RESULT 初始化结构 SAVE_RESULT保存结果到eep */
    INIT_RESULT     = 1,    /* 初始化结果信息 	*/
    SAVE_RESULT     = 2,    /* 保存结果			*/
    
    OFFSET_CON      = 0,    /* 偏移测试连续模式 */
    OFFSET_SIG      = 1,    /* 偏移测试单步模式 */
    
    U_TEST_TIME     = 0,    /* 刷新测试时间 */
    U_OFFSET_TIME   = 1,    /* 刷新偏移测试时间 */
};

/* 错误码 */
typedef enum err_num_enum{
    ERR_NONE        = 0,    /* 没有错误 */
    ERR_HIGH        = 1,    /* 电流上限报警 */
    ERR_LOW         = 2,    /* 电流下限报警 */
    ERR_REAL        = 3,    /* 真实电流报警 */
    ERR_CHARGE      = 4,    /* 充电报警 */
    ERR_OPEN        = 5,    /* 开路报警 */
    ERR_SHORT       = 6,    /* 短路报警 */
    ERR_ARC         = 7,    /* ARC 报警 */
    ERR_GFI         = 8,    /* GFI 报警 */
    ERR_AMP         = 9,    /* 功放报警 */
    ERR_GEAR        = 10,   /* 档位报警 */
    ERR_VOL         = 11,   /* 电压异常 */
}ERR_NUM_ENUM;

typedef enum{
    STAGE_READY     = 0,    /* 预备阶段 */
    STAGE_RISE      = 1,    /* 上升阶段 */
    STAGE_TEST      = 2,    /* 测试阶段 */
    STAGE_FALL      = 3,    /* 下降阶段 */
    STAGE_CHANGE    = 4,    /* 缓变阶段 */
    STAGE_CHARGE    = 5,    /* 充电阶段 */
    STAGE_DIS_CHA   = 6,    /* 放电阶段 */
    STAGE_INTER     = 7,    /* 间隔阶段 */
    STAGE_STAB      = 8,    /* 稳压阶段 */
}TEST_STAGE;

/************************************************/

/* 外部中断线开关宏定义 */
// #define AMP_INT(s)		(s == ENABLE? (EXTI_LINE9_ENABLE = 1):(EXTI_LINE9_ENABLE =0))   /* 功放异常中断开关 */
#define SHORT_INT(s)	(s == ENABLE? (EXTI_LINE10_ENABLE = 1):(EXTI_LINE10_ENABLE =0))     /* 短路异常中断开关 */
#define ARC_INT(s)		(s == ENABLE? (EXTI_LINE11_ENABLE = 1):(EXTI_LINE11_ENABLE =0))     /* 电弧异常中断开关	*/
#define STOP_INT(s)		(s == ENABLE? (EXTI_LINE5_ENABLE = 1):(EXTI_LINE5_ENABLE =0))       /* 复位异常中断开关	*/
#define GFI_INT(s)		(s == ENABLE? (EXTI_LINE12_ENABLE = 1):(EXTI_LINE12_ENABLE = 0))    /* GFI 异常中断开关 */

#define RTC_INI(s)      (s == ENABLE? (RTC_INI_ENABLE = 1):(RTC_INI_ENABLE = 0))            /* RTC 中断开关 */

/* 标志简写宏定义 */

#define DIS_SPEED       (test_flag.dis_speed)
#define STOP_COMM       (test_flag.stop_comm)
#define STOP            (test_flag.stop)
#define TERMINATE       (test_flag.terminate)
#define PASS            (test_flag.pass)
#define NEXT            (test_flag.next)
#define FAIL            (test_flag.fail)
#define CUR_FAIL        (test_flag.cur_step_fail)
#define TEST            (test_flag.test)
#define TESTING         (test_flag.testing)
#define HIGH            (test_flag.high_vol)
#define WAIT            (test_flag.wait)
#define OVER            (test_flag.test_over)
#define CUR_OVER        (test_flag.step_over)
#define ERR_INT         (test_flag.err_int)
#define CONT            (test_flag.cont)
#define FAIL_RE_START   (test_flag.fail_re_start)
#define CLEAR_F_BUF     (test_flag.clear_f_buf)
#define GR_OVER         (test_flag.gr_over)
#define ERR_NUM         (test_flag.err_num)
#define ERR_NUM_OVER	(test_flag.err_num_test_over)

#define OFFSET_BBD      (app_flag.offset_bbd)
#define OFFSET_NEXT     (app_flag.offset_next)

/******************************* 数据类型定义 ********************************************/
/* 系统运行时使用的临时标志 */
typedef struct {
	int8_t stop_comm;             /* 当上位机发送复位指令 */
    int8_t start_comm;            /* 当上位机发送启动指令时 */
	int8_t stop;                  /* 当发生异常时停止测试 */
    int8_t terminate;             /* 当按下复位键标记终止测试 */
	int8_t gradation;             /* 标志测试的不同阶段 取值见枚举 TEST_STAGE */
	int8_t high_vol;              /* 高压标志 1 高压 */
	int8_t pass;                  /* 测试合格 1 合格 */
	int8_t fail;                  /* 标志 本次测试失败 1 失败 */
    int8_t next;                  /* 允许下一步测试 */
	int8_t cur_step_fail;         /* 当前步测试失败标志 */
	int8_t test;                  /* 标志进入测试状态 */
	int8_t testing;               /* 正在测试标志 */
	int8_t wait;                  /* 标志间隔等待 1 正在等待 */
// 	int8_t retry;                 /* 标志测试从新开始，当前步被更新了 */
	int8_t test_over;             /* 测试结束 */
	int8_t step_over;             /* 单步测试结束 */
    int8_t fail_re_start;         /* 失败时按下启动键的标记 */
	int8_t cont;                  /* 当前步被设为连续测试 */
	int8_t err_amp_cont;          /* 功放报警计数 */
    uint32_t err_amp_timer;        /* 功放报警定时器 */
	int8_t clear_f_buf;           /* 清空滤波缓冲 */
	int8_t save_only_one;         /* 每次测试只能保存一次结果 */
	int8_t do_only_one;           /* 调一次 */
	int8_t pass_only_one;           /* 调一次 */
	int8_t updata_state;          /* 更新测试状态 */
	int8_t ir_err_times;          /* ir下限报警计数 */
// 	int8_t stop_re_screen;        /* 停止刷新屏幕 */
	int8_t stop_collect;          /* 停止采样 */
	int8_t gr_over;	              /* gr测试结束标志 */
	int8_t refresh_for_comm;      /* 通信要求刷新下屏幕信息 */
	int16_t dc_discharge_time;    /* 直流放电时间 */
	int8_t dis_speed;             /* 控制显示的速度 */
	int8_t vol_change_flag;       /* 电压改变标志 解决下降时电压由于刷新速度慢引起卡顿 不连续问题 */
	uint8_t into_set;	          /* 退出当前菜单，并进入到设置界面 */
	uint8_t into_test;	          /* 退出当前菜单，并进入到测试界面 */
    uint8_t into_auto_cal_ui;     /* 退出当前菜单进入 自动校准界面 */
	uint8_t g_start;              /* 列表显示时直接启动测试 */
	uint8_t g_stop;               /* 列表显示时停止测试返回测试待命 */
	
	int8_t confirm_test_init_ok;  /* 确认测试初始化OK */
	int8_t stop_uart;             /* 停止直流gr模块的通信 */
// 	int8_t uart_ok;               /* 串口通信数据接收ok */
	int8_t uart_next;             /* 发送下一个串口指令 */
	int8_t err_num;               /* 错误码 */
	int8_t err_num_test_over;     /* 测试时间到错误码 */
    int8_t test_status;           /* 测试状态 RUNING STOPING */
	int8_t bbd_time;              /* bbd获取电容值时自动选择档位时使用 */
	int8_t err_times;             /* 对发生的错误计数当计数值达到报警值后就报警 */
	int8_t re_times_per_second;   /* 计数名秒刷新的次数 */
    int8_t record_per_second_re;  /* 记录每秒刷新的次数 */
	int8_t dis_status;            /* 显示状态 */
    uint8_t dc_gr_open_err;       /* DCGR OPEN ERR */
    uint8_t dc_gr_amp_err;        /* DCGR AMP ERR */
    
    int8_t dis_charge_end;        /* 放电结束标记 用于大电容发电使用 */
    uint8_t judge_err_en;         /* 异常判断使能 */
    uint8_t gfi_delay_count;      /* GFI周期的清除gfi计数 */ 
    uint8_t sample_task_en;       /* 采样任务使能标志 */
    uint8_t mb_syn;               /* modbus同步标志 */
    uint8_t mb_break_send;        /* modbus终止发送并清空发送队列 */
    uint8_t mb_wait_ack;          /* modbus命令等待响应 1不需要回应的 0是需要回应 */
    uint8_t allow_dc_gr_fetch;    /* dc_gr允许访模块标志 */
    uint8_t high_err;             /* 上限报警标志 */
    uint8_t mb_master_timeout;    /* modbus主机超时标志 */
    uint8_t vol_err_count;        /* 电压异常报警计数 */
    uint8_t hight_err_count;        /* 上限报警计数 */
    uint32_t vol_rise_step_t;     /* 电压上升阶段时间计数 */
    uint8_t err_once;/* 失败报警只执行一次 */
    uint8_t test_led_flag;/* 测试灯闪烁标记 */
}TEST_FLAG;

enum{
    LOADING_NONE    = 0,
    LOADING_OUT     = 1,
    LOADING_IN      = 2,
};

typedef enum{
    TEST_NORMAL,/* 正常测试 */
    TEST_OFFSET = 1,/* 偏移测试标记 */
    TEST_BBD    = 2,/* BBD GETC测试标记 */
}CUR_TEST_STATUS;

//失败后启动控制
enum{
    FAIL_RE_START_FIRST = 1,//失败后从第1步启动
    FAIL_RE_START_NEXT = 2,//失败后从下一步启动
};

typedef struct{
	uint32_t sample_cycle;         /* 采样任务的采样周期 */
	uint32_t sample_rate;         /* 采样率 */
	uint8_t offset_mode;          /* 表示偏移测试模式 1单步 0连续 */
	int8_t bbd;                   /* bbd测试模式 */
	CUR_TEST_STATUS offset_bbd;   /* offset测试模式 */
	int8_t offset_next;           /* 表示进入下一个偏移测试 */
	uint8_t goto_menu_page;       /* 跳到指定的页 */
    uint8_t into_test_ui;         /* 标记进入测试界面 */
    uint8_t usb_loading_status;   /* 数据导入导出状态 */
    uint8_t usb_loading_advance;  /* 数据传输进度 */
    uint8_t calibration;          /* 进入校准界面的标志 */
    uint8_t recover_flag;         /* 恢复出厂设置标志 */
    uint8_t scan_module_flag;     /* 扫描模块界面标志 */
    
    uint8_t result_real_usb;      /* 结果实时传递给u盘 */
    
    uint16_t power_check;         /* 电源检查 */
    uint8_t leading_watch_dog;    /* 数据导出导入时看门狗 */
    uint8_t usb_con_st;           /* U盘连接状态 */
    uint8_t buzzer_flicker_en;    /* 蜂鸣器闪响开关 */
    uint8_t into_get_cap;         /* 进入获取电容界面并自动进行测试 */
    uint8_t into_get_offset;      /* 进入获取偏移界面并自动进行测试 */
	uint8_t auto_cal_st;		  /* 自动校准状态0不在自动校准状态 1在自动校准状态 */
    
	int8_t dc_gr_module_status;   /* CAL_RUNING CAL_STOPING */
	uint8_t res_list_num_count;   /* 结果列表中已经保存的结果数目 */
	uint8_t dc_gr_task_send_idle; /* 直流发送任务空闲标记 */
	uint8_t absorptance;/* IR吸收比开关 */
}APP_FLAG;

/* 滤波缓冲区 */
typedef struct{
#define SF_DEPTH            3     /* 平滑滤波的深度 太深会带来更多问题 最大6 */
	u32 sf_buf_u[3][SF_DEPTH];    /* 整型平滑滤波 */
	float af_buf_f[3];            /* 浮点限幅滤波 */
}F_BUF_STR;


/************************* 定义全局变量 ***************************/
#ifdef   _SERVE_TEST_GLOBALS
#define  _SERVE_TEST
#else
#define  _SERVE_TEST  extern
#endif

_SERVE_TEST	__IO TEST_FLAG	test_flag;   /* 定义全局标志给测试使用 */
_SERVE_TEST	__IO APP_FLAG	app_flag;    /* 定义其他全局标志给测试使用 */
_SERVE_TEST	F_BUF_STR	f_buf;	         /* 滤波缓冲 */

_SERVE_TEST uint32_t cpld_cmd;           /* cpld命令字 */
_SERVE_TEST uint32_t cpld_base;          /* cpld频率基数 */

/****************************************************************/
//结果实时传递队列
#define RES_QUEUE_MAX   5
_SERVE_TEST uint16_t result_r_queue[RES_QUEUE_MAX];
_SERVE_TEST uint8_t result_r_queue_h;/* 队列头 */
_SERVE_TEST uint8_t result_r_queue_t;/* 队列尾 */

/****************************************************************/

/* 存放AD采集的电压和电流 */
_SERVE_TEST uint16_t sample_vol;         /* 采样电压 */
_SERVE_TEST uint16_t sample_cur;         /* 采样电流 */
_SERVE_TEST float cur_ad_vol;            /* 采样电流 */
_SERVE_TEST uint16_t adc_value[2];       /* 采样电流 */

_SERVE_TEST uint8_t output_buf[10];      /* 输出采集值 */
_SERVE_TEST uint8_t loop_buf[10];        /* 回路采集值 */
_SERVE_TEST uint8_t real_buf[10];        /* 真实电流采集值 */
_SERVE_TEST uint8_t time_buf[10];        /* 测试时间 */

_SERVE_TEST float vol_ave;               /* 电压 */
_SERVE_TEST float cur_ave;               /* 电流 */
_SERVE_TEST float res_ave;               /* 电阻 */
_SERVE_TEST float real_ave;              /* 真实电流 */
_SERVE_TEST float cap_ave;               /* 电容 */
_SERVE_TEST uint16_t g_ad_dog;			 /* ad看门狗 */

_SERVE_TEST float err_vol_bak; /* 报警时电压值 */
_SERVE_TEST float err_cur_bak; /* 报警时电流值 */
_SERVE_TEST float err_res_bak; /* 报警时电阻备份 */
_SERVE_TEST float err_real_bak;/* 报警时真实电流备份 */
_SERVE_TEST float err_cap_bak; /* 报警时电容备份 */

_SERVE_TEST float ir_15s_res;			 /* IR测试 15s时的电阻值 */
_SERVE_TEST float ir_60s_res;			 /* IR测试 60s时的电阻值 */
_SERVE_TEST float ir_absorptance;		 /* IR测试吸收比 */
_SERVE_TEST uint32_t gear_up_limit;      /* ir自动换挡关闭后的上限值 */
_SERVE_TEST uint32_t g_ir_dly;           /* ir换档延时 */
_SERVE_TEST uint16_t g_plc_dly;          /* PLC延时关闭 */
_SERVE_TEST void (*test_irq_fun) (void);
_SERVE_TEST void (*cs99xx_test_fun) (void);

_SERVE_TEST uint32_t uart_err_num;       /* 出口通信失败的次数 */ //load_data

_SERVE_TEST	__IO uint32_t g_test_time;   /* 测试时间片 在定时器中断服务程序中被自加 */
_SERVE_TEST	__IO uint16_t g_dis_time;    /* 用于显示当前测试阶段的计时时间 */
_SERVE_TEST	uint32_t g_buzzer_time;	     /* 蜂鸣器响的长度 */
_SERVE_TEST	uint32_t g_cylinder_ctrl_t;	     /* 气缸控制状态延时 */

_SERVE_TEST uint16_t dly_t;              /* 输出延时 */
_SERVE_TEST uint16_t zeo_t;              /* 第〇阶段的累计时间 */
_SERVE_TEST uint16_t one_t;              /* 第一阶段的累计时间 */
_SERVE_TEST uint16_t two_t;              /* 第二阶段的累计时间 */
_SERVE_TEST uint16_t thr_t;              /* 第三阶段的累计时间 */
_SERVE_TEST uint16_t for_t;              /* 第四阶段的累计时间 */
_SERVE_TEST uint16_t fiv_t;              /* 第五阶段的累计时间 */
_SERVE_TEST uint16_t tes_t;              /* 当前步测试时间 */
_SERVE_TEST uint16_t ris_t;              /* 上升时间 */
_SERVE_TEST uint16_t sta_t;              /* 稳压时间 */
_SERVE_TEST uint16_t fal_t;              /* 下降时间 */
_SERVE_TEST uint16_t int_t;              /* 间隔时间 */
_SERVE_TEST uint16_t cha_t;              /* 缓变时间 */

_SERVE_TEST uint16_t cur_step;           /* 当前步的步号 */
_SERVE_TEST uint8_t cur_mode;            /* 当前步的模式 */
_SERVE_TEST uint8_t cur_offset_result;   /* 当前步的偏移结果 */
_SERVE_TEST uint8_t cur_getc_result;     /* 当前步的获取电容结果 */
_SERVE_TEST uint8_t cur_t_time;          /* 测试时间 */
_SERVE_TEST uint8_t cur_status;          /* 当前状态 */
_SERVE_TEST uint8_t cur_cylinder_ctrl_status;          /* 当前气缸控制状态定制机专用 */
_SERVE_TEST uint8_t cur_status_plc;      /* 当前状态 */
_SERVE_TEST uint8_t cur_plc_err_st;		 /* PLC报警状态 */
_SERVE_TEST uint8_t cur_comm_bps;        /* 当前的通信波特率 */
_SERVE_TEST uint16_t updata_comm_flag;    /* 更新通信标志 */

_SERVE_TEST uint16_t cur_vol;            /* 当前步的电压值 */
_SERVE_TEST float cur_cap;               /* 当前步的电容值 */
_SERVE_TEST uint16_t cur_open_ratio;     /* 当前步的开路报警值 */
_SERVE_TEST uint16_t cur_short_ratio;    /* 当前步的短路报警值 */
_SERVE_TEST uint8_t cur_auto;            /* 当前ir 自动换挡 */
_SERVE_TEST int8_t steps_con;            /* 步间连续 */
_SERVE_TEST int8_t steps_pass;           /* 步间PASS */
_SERVE_TEST uint8_t cur_gear;            /* 当前步电流档位 */
_SERVE_TEST uint8_t cur_gear_max;        /* 当前步电流档位最大值 */
_SERVE_TEST uint8_t cur_gear_min;        /* 当前步电流档位最小值 */
_SERVE_TEST uint8_t cur_soft_gear;       /* 当前IR软档位 */
_SERVE_TEST uint8_t vol_segment;         /* 电压段 0 10-500v 1 100-MAX */
_SERVE_TEST uint8_t contact_segment;	 /* 双触点变压器触点分段标记 0 第一段 1第二段 */
_SERVE_TEST uint8_t cur_segment;         /* 电流段 GR DV */
_SERVE_TEST uint8_t cur_gear_bak;        /* 档位电流档位备份值 */
_SERVE_TEST uint8_t cur_ir_dis_gear;     /* 自动档关闭时使用 */
_SERVE_TEST uint8_t cur_ir_dis_gear_0;   /* 自动档关闭时使用 */
_SERVE_TEST uint8_t cur_ir_dis_gear_1;   /* 自动档关闭时使用 */
_SERVE_TEST uint32_t g_ir_gear_hold; 	 /* 自动档档位保持时使用 */
_SERVE_TEST uint8_t cur_gear_comm;       /* 当前步电流档位 */
_SERVE_TEST uint8_t cur_real_gear_comm;  /* 当前步电流档位 */
_SERVE_TEST uint8_t cur_method;          /* GR测试方法 GR_V_CONT_MODEL GR_V_PULSE_MODEL */
_SERVE_TEST uint16_t res_ave_comm;       /* IR电阻为通信用 */
_SERVE_TEST uint16_t cur_ave_comm;       /* GR电流为通信用 */
_SERVE_TEST uint8_t cur_output_impedan;  /* DCW 输出阻抗 */
_SERVE_TEST uint16_t cur_intensity;      /* CC当前电流强度 */

_SERVE_TEST float cur_high;              /* 当前上限值 */
_SERVE_TEST float cur_low;               /* 当前下限值 */
_SERVE_TEST float cur_ir_rang_h;         /* 当前下限值 */
_SERVE_TEST uint16_t cur_charge_cur;     /* 当前充电电流 */
_SERVE_TEST uint16_t cur_real_cur;       /* 当前真实电流 */
_SERVE_TEST uint16_t cur_delay_t;        /* 延时报警时间 */
_SERVE_TEST uint8_t cur_auto;            /* 当前ir 自动换挡 */
_SERVE_TEST uint16_t cur_arc_gear;       /* 当前电弧等级 或电流模式下的电流 */
_SERVE_TEST uint16_t cur_frequency_gear; /* 输出频率档位选择 1-9 */
_SERVE_TEST uint16_t cur_frequency;      /* 输出频率 */
_SERVE_TEST TEST_PORT cur_port;          /* 当前的端口 */
_SERVE_TEST uint8_t cur_auto;            /* 当前ir 自动换挡 */

_SERVE_TEST int8_t steps_con;	         /* 步间连续 */
_SERVE_TEST int8_t steps_pass;           /* 步间PASS */
_SERVE_TEST uint16_t cur_offset;         /* 当前偏移电流或电阻 */
_SERVE_TEST uint16_t cur_real_offset;    /* 当前的真是电流偏移 */
_SERVE_TEST uint8_t cur_vol_gear;        /* 当前ir电压档位 0-9 */

_SERVE_TEST uint8_t err_charge;          /* 充电错误 */
_SERVE_TEST uint8_t err_real;            /* 真实电流错误 */

_SERVE_TEST float cur_dac_k;             /* 当前DAC校准系数k */
_SERVE_TEST float cur_dac_b;             /* 当前DAC校准系数b */
_SERVE_TEST float cur_adc_k;             /* 当前ADC校准系数k */
_SERVE_TEST float cur_adc_b;             /* 当前ADC校准系数b */
_SERVE_TEST float cur_adc_cur_k;         /* 当前 电流ADC校准系数k */
_SERVE_TEST float cur_arc_k;             /* 当前ARC报警校准系数k */
_SERVE_TEST float cur_res_k;             /* 当前IR电阻挡位的校准系数k */
// _SERVE_TEST float cur_ir_second_cal_k_1; /* 当前 IR二次校准系数 */
// _SERVE_TEST float cur_ir_second_cal_b_1; /* 当前 IR二次校准系数 */
// _SERVE_TEST float cur_ir_second_cal_k_2; /* 当前 IR二次校准系数 */
// _SERVE_TEST float cur_ir_second_cal_b_2; /* 当前 IR二次校准系数 */

_SERVE_TEST  float vol_ch_step;	/* 电压缓变步进值 */
_SERVE_TEST  float vol_ch_base;	/* 电压变化的起始值 */
_SERVE_TEST  float vol_ch_target;	/* 电压变化的目标 */



/******************************* 函数声明 *********************************/
extern void clear_test_flag(void);
extern int32_t dis_charge_remain_vol(void);
extern void record_exception_scene(void);
extern void recover_exception_scene(void);
extern void recover_exception_scene(void);
extern void close_test_timer(void);
extern void open_test_timer(void);
extern void install_test_irq_fun(void);
extern void count_vol_step_value(void);
extern void test_g_details(void);

extern void cur_cylinder_ctrl_stop(void);

extern void offset_state_dis(void);

extern void dis_cur_page(const int16_t n, const int16_t cur_page_num);
extern void startup(void);
extern void stop_test(void);
extern void test_pass(void);
extern void test_fail(void);
extern void ir_set_gear(const uint8_t gear, uint8_t soft_gear);
extern void bbd_auto_select_gear(void);
extern void bbd_load_gear(void);
extern void shift_cur_gear(uint8_t mode);
extern void exception_handling(int8_t errnum);

extern void init_sys_flag(void);
extern void count_dis_value(void);

extern void irq_stop_relay_motion(void);
extern void relay_motion(void);
extern void updata_time(const int8_t n, uint16_t time);

extern void test_status_dis(void);
extern void test_dis_err_data(void);
extern void test_dis(void);
extern int8_t offset_dis(const int8_t n);

extern void dis_offset_cur(const int8_t n, const float o_cur, const float o_real_cur);
extern void dis_test_ir_res(void);
extern void transform_test_ir_res(void);
extern void transform_test_ir_res_loop(void);
extern void dis_test_gr_cur(void);
extern void dis_test_gr_vol(void);
extern void transform_test_gr_vol(void);
extern u32 smoothing_filtering(const u8 sf_n,const u32 val);

extern void de_reset(void);
extern void test_reset(void);
extern void transform_test_vol_string(void);
extern void transform_test_loop_string(void);
extern void transform_test_time_string(uint16_t time);
extern void count_fall_vol_step_value(void);
extern void count_rise_vol_step_value(void);
extern void count_vol_ch_step(void);
extern void vol_change(void);
extern void vol_change_200hz(uint32_t time);
extern void vol_change_1000hz(uint32_t time);
extern uint8_t get_gr_gear(void);
extern void load_ratio(uint8_t mode);
extern void load_data(void);//test_ready
extern void cs99xx_big_cap_test(void);
extern void cs99xx_test(void);
extern void cs99xx_g_test(void);
extern void offset_test(void);
extern void confirm_vol_segment(void);

extern void set_sample_rate(uint32_t rate);

extern void test_irq(void);
extern void test_big_cap_irq(void);
extern void test_g_irq(void);

extern void open_hv(void);
extern void close_hv(void);

extern int32_t check_test_mode(NODE_STEP * p);

extern void syn_dis_offset_cur(const int8_t n, const float o_cur, const float o_real_cur);

extern void dis_offset_test_ports(const TEST_PORT *pport);
extern void serve_test_poll_task(void);
extern void wait_dc_gr_send_task_idle(void);

#endif //__SERVE_TEST_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
