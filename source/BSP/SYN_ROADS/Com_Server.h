#ifndef __COM_SERVER_H
#define __COM_SERVER_H

// #include "bsp.h"
#include "Com_Send_Ctrl.h"
#include "my_tools.h"
#include "Com_Receive_Ctrl.h"
#include "stm32f10x.h"
#include "Com_Server.h"

typedef enum {
    GOOD,
    HIGH_ERROR,
    LOW_ERROR,
	SHORT_ERROR,
}CURRENT_ERROR_TYPE;
#define INPUT_NUM           ( 10 )

typedef struct {
    CURRENT_ERROR_TYPE Road0 :2;
    CURRENT_ERROR_TYPE Road1 :2;
    CURRENT_ERROR_TYPE Road2 :2;
    CURRENT_ERROR_TYPE Road3 :2;
    CURRENT_ERROR_TYPE Road4 :2;
    CURRENT_ERROR_TYPE Road5 :2;
    CURRENT_ERROR_TYPE Road6 :2;
    CURRENT_ERROR_TYPE Road7 :2;
    CURRENT_ERROR_TYPE Road8 :2;
    CURRENT_ERROR_TYPE Road9 :2;
}Road_Error_Msg;

typedef struct {
    uint16_t Road_current[INPUT_NUM];
    
    union {
        Road_Error_Msg  road_msg;
        uint32_t road_int_msg;
    }Road_Msg;

}Result_Inf;

extern Result_Inf result_inf;

#define     CMD_PLACE       (0)

#define		PARAM_PLACE		(CMD_PLACE + 1)

#define		I_MAX			(10000)

#define		R_I_MAX			(1.2*I_MAX)

/*
    供给外部使用
*/



/*需要在主进程中扫描*/
extern int32_t Syn_Scan_usart(void);

/*调用后开始测试*/
extern void Syn_Start_Test(void);

/*调用后停止测试*/
extern void Syn_Stop_Test(void);

/*调用后复位测试*/
extern void Syn_Reset_Test(void);

/*调用后设置校准参数*/
extern void Syn_Set_2mACalib(float * calib_data); 

/*调用后设置校准参数*/
extern void Syn_Set_20mACalib(float * calib_data); 

/*调用后读取模块的电流结果*/
extern void Syn_Read_Result(void);

/*调用后读取模块的电ADC结果*/
extern void Syn_Read_Adc_Val(void);

/*调用后设置上下限*/
/*上下限数据以mA为单位*/
extern void Syn_Set_limit_f(float high_limit, float low_limit);

/*调用后设置上下限*/
/*上下限数据以 10uA 为单位*/
extern void Syn_Set_limit_i(uint16_t high_limit, uint16_t low_limit);


/*
    通信内部使用数据和函数
*/

// /*单选结果*/
// typedef enum {
//     YES,
//     NO,
// }Single_Switch;


extern uint8_t over_data[2]; /*字符数组中存放结束符*/

/*获取校验值*/
uint16_t read_check_data(uint8_t * data, uint8_t data_num);

void Send_Data(void);
#endif
