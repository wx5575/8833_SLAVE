#ifndef __COM_SEND_CTRL_H
#define __COM_SEND_CTRL_H

// #include "bsp.h"
#include "Com_Server.h"
#include "stm32f10x.h"

typedef enum {
/*  
    作用 ：主机发送启动信号
    状态 ：停止或者复位状态
    参数 ：无
    描述 ：开始采集信号
    返回 ：成功 No Error    失败对应的失败信息
*/
    TEST_START = 1,     
 
/*
    作用 ：主机发送停止信号停止运行
    状态 ：运行状态
    参数 ：无
    描述 ：指示灯、PLC点亮
    返回 ：成功 No Error    失败对应的失败信息
*/
    TEST_STOP  ,        
    
/*
    作用 ：主机发送复位信号
    状态 ：运行状态或停止状态
    参数 ：无
    描述 ：熄灭指示灯、PLC
    返回 ：成功 No Error    失败对应的失败信息
*/
    TEST_RESET ,        

/*
    作用 ：主机发送校准参数
    状态 ：复位状态
    参数 ：十路各自的校准值 20mA参数格式（float型）
    描述 ：修改每一路的校准参数  
    返回 ：成功 No Error    失败对应的失败信息
*/
    SET_20mA_CALIB ,  

/*
    作用 ：主机发送校准参数
    状态 ：复位状态
    参数 ：十路各自的校准值 20mA参数格式（float型）
    描述 ：修改每一路的校准参数  
    返回 ：成功 No Error    失败对应的失败信息
*/
    SET_2mA_CALIB ,  

/*
    作用 ：读取测试结果
    状态 ：机器必须处于运行状态
    参数 ：无
    描述 ：读取测试结果
    返回 ：成功 测试结果    或者失败对应的失败信息
*/
    READ_RESULT,

/*
    作用 ：设置电流上下限
    状态 ：复位状态
    参数 ：测试电流的上下限(float)
    描述 ：测试之前修改电流的上下限
    返回 ：成功 No Error    失败对应的失败信息
*/
    SET_LIMIT,
	
/*
	作用：读取模块的ADC数据
    状态 ：机器必须处于运行状态
    参数 ：无
    描述 ：读取测试过程中ADC结果（校准时使用）
    返回 ：成功 测 ADC结果    或者失败对应的失败信息	
*/	
	READ_ADC_VAL,
	
	SET_2mA_OFF_SET,
	
	SET_20mA_OFF_SET,
    
}CTRL_CMD_LIST;

extern void Write_Cmd(CTRL_CMD_LIST cmd);

extern void Send_Dispose(void);
extern void Send_Rear_Dispose(void);
extern void Write_char_data(uint8_t *data, uint8_t num);

extern void Comm_OK(void);




#endif
