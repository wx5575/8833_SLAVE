#ifndef __SYSTEM_CONFIG_H
#define __SYSTEM_CONFIG_H



//#include "Schedul_cfg.h" 
#include "stdint.h"
// #include "General.h"
#include "cs99xx_struct.h"

typedef enum {ErrorType = 0, SuccessType = !ErrorType} ErrorStatusType;

                         
#define     PORT_MAX_NUM                    ( 16 )

typedef enum { UN_CONNECT = 0,REMOTE_CONTROL, LOCAL_CONTROL,  }CONTROL_STATUS;

enum {
ACW_MODE = 0,
DCW_MODE = 1,
IR_MODE  = 2,
GR_MODE  = 3,
BBD_MODE = 7,
};

#define     STEP_TIME_MAX                   ( 9999 )        /*步骤相关时间的设置的最大值*/

/*本设备不可直接读取数据，数据需要通过下发给从机后反馈给上位机*/
//#define SELF_GET_DATA               

/*单接地开关*/
//#define     SINGLE_GND        

/*ModBus接收数据结束时间 5mS*/
#define  MODBUS_RECEIVE_OVER_TIME           ( 5 )

// /*所有任务优先级配置*/

// #define MODBUS_CLIENT_REFRESH_TASK_PRIO     ( TASK_PRIO_0 )
// #define MODBUS_SERVER_REFRESH_TASK_PRIO     ( TASK_PRIO_1 )
// #define MODBUS_CLIENT_DISPOSE_TASK_PRIO     ( TASK_PRIO_2 )
// #define MODBUS_SERVER_DISPOSE_TASK_PRIO     ( TASK_PRIO_3 )
// #define MODBUS_CLIENT_SEND_TASK_PRIO        ( TASK_PRIO_4 )
// #define MODBUS_SERVER_SEND_TASK_PRIO        ( TASK_PRIO_5 )

// #define LED_TASK_PRIO                       ( TASK_PRIO_63 )
// #define LED1_TASK_PRIO                       ( TASK_PRIO_62 )
    

#define     FILE_NAME_MAX_SIZE              ( 14 )



#ifndef SINGLE_GND
typedef enum {G_TYPE, N_TYPE, FILE_WORK_MODE_TYPE_ERROR}FILE_WORK_MODE_TYPE;
typedef enum {SCALE_TYPE, CURRENT_TYPE, FILE_ARC_TYPE_ERROR}FILE_ARC_TYPE;
typedef enum {WHOLE_TYPE, SINGLE_TYPE, FILE_DUT_TYPE_ERROR}FILE_DUT_TYPE;

#endif

#define     FILE_NUM_MAX    ( 30 )          /*文件编号最大值*/
#define     FILE_NUM_MIN    ( 1 )           /*文件编号最小值*/

#define     FILE_NAME_CAPITAL               /*文件名允许大写字母输入*/
#define     FILE_NAME_LOWER_CASE            /*文件名允许小写字母输入*/
#define     FILE_NAME_FIGURE                /*文件名允许数字输入*/

/*
写入文件的时候文件的记录号为0，测试步骤的文件记录号不为1——STEP_MAX，
以区分写入的是文件还是测试步骤
*/
#define     FILE_NOTE_NUM       ( 0 )
#define     STEP_MAX            ( 9999 )
#define     TIME_MAX            ( 9999 )
#define     FILE_NAME_LEN       ( 16 )

//#define     STEP_MAX            ( 99 )

typedef struct {
    uint16_t file_num;                      /*文件编号*/
    uint16_t step_sum;                      /*总步骤数*/
    uint8_t file_name[FILE_NAME_LEN];       /*文件名*/
    uint16_t pass_keep_time;                /*PASS信号保持时间*/
    uint16_t pass_beep_time;                /*PASS信号蜂鸣器时间*/
#ifndef SINGLE_GND
    FILE_WORK_MODE_TYPE mode_type;          /*工作模式*/
    FILE_ARC_TYPE arc_type;                 /*电弧侦测模式*/
#endif

}FILE_MSG;

typedef enum {
    START_STATUS = 1  ,
    STOP_STATUS       ,
	PASS_STATUS		  ,
    RESET_STATUS      ,
}TestStatusTypedef;

typedef enum {
	ST_VOL_RISE     = 0,
	ST_TESTING      = 1,
	ST_VOL_FALL     = 2,
	ST_INTER_WAIT   = 3,
	ST_WAIT         = 4,
	ST_PASS         = 5,
	ST_STOP         = 6,            /* 复位状态 */
	ST_ERR_H        = 7,
	ST_ERR_L        = 8,
	ST_ERR_SHORT    = 9,
	ST_ERR_VOL_ABNORMAL = 10,       /* 电压异常 */
	ST_ERR_ARC      = 11,
	ST_ERR_GFI      = 12,
	ST_ERR_FAIL     = 13,
	ST_ERR_REAL     = 14,           /* 真实电流报警 */
	ST_ERR_CHAR     = 15,           /* 充电报警 */
	ST_ERR_GEAR     = 16,           /* 量程报警 / 档位报警 */
	ST_ERR_AMP      = 17,
	ST_OUTPUT_DELAY = 18,
	ST_ERR_OPEN     = 19,           /* 开路报警 */
}TestingStatusTypedef;  /*运行时状态*/

typedef enum{
    CY_TESTING      = 1,
    CY_STOP,
    CY_WAIT,
    CY_OVER,                           /* 测试结束气缸控制 */
}CYLINDER_TEST_STATUS;/* 气缸控制状态 */


enum{
	UPDATE_PLC_SW 	            = 1<<0,
	UPDATE_COMM_SW	            = 1<<1,
	UPDATE_COMM_BPS	            = 1<<2,
    
	UPDATE_LEAD_IN_DATA_USB	    = 1<<3,
	UPDATE_LEAD_OUT_DATA_USB    = 1<<4,
    
    RESET_INTERFACE_MCU         = 1<<5,//复位接口板的单片机
    
    RESULT_LEAD_OUT_USB         = 1<<6,//结果导出到U盘
    
    READ_ONE_RESULT_USB         = 1<<7,//当测试完一条后就让通信板读走写入U盘
    
    CHECK_USB_CONNECT           = 1<<8,//检查U盘是否连接
};

typedef struct {
    uint16_t v_test;                        /*测试电压*/
    uint8_t current_grade;                  /*电流档位*/
    uint8_t real_gear;                      /* 真实的电流档位 */
    uint16_t i_test;                        /*测试电流*/   
}W_MSG;                                     /*耐压属性*/

typedef struct {
    W_MSG w_msg;                            /*耐压基本属性*/
    uint8_t real_current_butten;            /*真实电流开关*/
    uint16_t real_current;                  /*真实电流*/                                                    
}ACW_MSG;                                   /*直流耐压属相*/

typedef struct {
    W_MSG w_msg;                            /*耐压基本属性*/
}DCW_MSG;

typedef struct {
    uint16_t v_test;                        /*测试电压*/
    uint8_t res_grade;                      /*电阻档位*/ 
    uint16_t iso_res;                       /*绝缘电阻*/           
}IR_MSG;

typedef struct {
    uint16_t i_test;                        /*测试电流*/
    uint16_t res_gnd;                       /*接地电阻*/
}GR_MSG;

typedef struct {
    uint8_t step;                           /*测试步骤*/
    uint8_t test_mode;                      /*测试模式*/
    uint16_t test_time;                     /*测试时间*/
    uint8_t test_status;                    /*测试状态*/

    union {
        ACW_MSG acw_msg;
        DCW_MSG dcw_msg;
        IR_MSG ir_msg;
        GR_MSG gr_msg;
    }STATUS_MSG;
}TEST_FETCH_MSG;                            /*测试信息*/


typedef struct{
    uint8_t buf[36];/* TEST_FETCH */
    uint32_t plc_status;/* plc状态 */
}TEST_FETCH_T;

typedef struct {
    uint16_t test_voltage;                  /*测试电压*/
    uint8_t current_grade;                  /*电流档位*/
    uint8_t real_gear;                      /* 真实的电流档位 为了区别所有档位特加入的 wangxin 2015.08.10 */
    uint16_t i_test;                        /*测试电流*/ 
    uint16_t real_current;                  /*真实电流*/  
}W_Result_Msg;

typedef struct {
    uint16_t test_voltage;                  /*测试电压*/
    uint8_t res_grade;                      /*电阻档位*/
    uint16_t iso_res;                       /*绝缘电阻*/ 
    uint16_t real_current;                  /*真实电流*/  
}IR_Result_Msg;

typedef struct {
    uint16_t test_current;                  /*测试电流*/
    uint16_t res_gnd;                       /*接地电阻*/
    uint16_t real_current;                  /*真实电流*/
}GR_Result_Msg;

typedef struct {
    uint8_t target_name[FILE_NAME_LEN];     /*被测件名*/
    uint8_t test_file_name[FILE_NAME_LEN];  /*测试文件名称*/
    uint16_t test_num;                      /*本次测试的步骤编号*/
    uint16_t test_all_num;                  /*总测试步骤数*/
    uint8_t test_file_mode;                 /*测试文件的工作模式*/
    uint8_t test_mote_mode;                 /*测试步骤的工作模式*/
    
    union {
        W_Result_Msg w_msg;
        IR_Result_Msg ir_msg;
        GR_Result_Msg gr_msg;
    }status_msg;
    
    uint16_t test_time;                     /*测试时间*/      
    uint8_t test_result;                    /*测试结果*/
}RESULT_FETCH_MSG;

typedef struct {
    uint16_t i_high;                        /*电流上限*/
    uint16_t i_low;                         /*电流下限*/
    uint16_t arc;                           /* 电弧侦测 */
    uint16_t arc_type;                      /* 电弧侦测模式 */
    uint16_t upper_time;                    /*上升时间*/
    uint16_t drop_time;                     /*下降时间*/      
    
    uint16_t test_port;                     /*测试端口*/ 
}STEP_W_MSG;

typedef struct {
    STEP_W_MSG w_msg;
    uint16_t out_freq;                      /*输出频率*/     
}STEP_ACW_MSG;

typedef struct {
    STEP_W_MSG w_msg;                        
    uint16_t i_charge;                      /*充电电流*/
    uint16_t delay_time;                    /*延时时间*/ 
}STEP_DCW_MSG;

typedef struct {
    uint8_t auto_change_grade;              /*自动换档*/
    uint16_t res_high;                      /*电阻上限*/
    uint16_t res_low;                       /*电阻下限*/
    uint16_t upper_time;                    /*上升时间*/
    uint16_t delay_time;                    /*延时时间*/
    uint16_t test_port;                     /*测试端口*/
}STEP_IR_MSG;

typedef struct {
    uint16_t res_high;                      /*电阻上限*/
    uint16_t res_low;                       /*电阻下限*/ 
    uint16_t out_freq;                      /*输出频率*/
}STEP_GR_MSG;

typedef struct {
    TEST_FETCH_MSG test_msg;

    union {
        STEP_ACW_MSG acw_msg;
        STEP_DCW_MSG dcw_msg;
        STEP_IR_MSG  ir_msg;
        STEP_GR_MSG  gr_msg;  
    }step_status_msg;

    uint16_t test_time;                     /*测试时间*/ 
    uint16_t step_pass;                     /*步间pass*/
    uint16_t space_time;                    /*间隔时间*/
    uint16_t step_continue;                 /*步间连续*/

}STEP_MSG_TYPEDEF;


typedef struct {
    uint8_t year;                           //0~99年的后两位
    uint8_t month;                          //1~12月份
    uint8_t day;                            //1~31日期
    uint8_t hour;                           //0~23小时
    uint8_t minute;                         //0~59分钟
    uint8_t second;                         //0~59分钟
}SYS_TIME;

typedef struct {  
    uint16_t file_num;                      /*本次测试的步骤编号*/
    uint8_t file_name[FILE_NAME_LEN];       /*测试文件名称*/
    uint16_t step_sum;                      /*总步骤数*/
  
    uint8_t mode_type;                      /*测试文件的工作模式*/
    uint16_t pass_keep_time;                /*PASS信号保持时间*/
    uint16_t pass_beep_time;                /*PASS信号蜂鸣器时间*/
 
#ifndef SINGLE_GND
    FILE_ARC_TYPE arc_type;                 /*电弧侦测模式*/
#endif

}CATALOG_FILE;

#define	PASS_WORD_LEN		(8)

/* 密码结构体 */
typedef struct {
	char old[PASS_WORD_LEN+1];
	char new[PASS_WORD_LEN+1];
}PASS_WORD_T;

/* 被测品名 */
typedef struct {
	char name[FILE_NAME_LEN];
}RESULT_DUT_T;

typedef struct {
	char idn_data[50];
} IDN_MSG;


#endif
