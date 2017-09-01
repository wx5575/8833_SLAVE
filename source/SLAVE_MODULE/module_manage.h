/**
  ******************************************************************************
  * @file    module_manage.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理，硬件上有四路串口，使用485通信就可实现每路串口最多挂接15个模块
  ******************************************************************************
  */
#ifndef __MODULE_MANAGE_H__
#define __MODULE_MANAGE_H__


#include "cs99xx_struct.h"
#include "com_comm.h"

#define FRAME_HEAD_SIZE    4  ///<帧头的字节个数
#define CRC_LEN             2  ///<CRC校验字节个数

/**
  * @brief  系统错误码定义
  */
typedef enum{
    CS_ERR_NONE = 0x1000,///<正常
    CS_ERR_FILE_NO_EXIST,///<文件不存在
    CS_ERR_FILE_NUM_INVALID,///<文件编号无效
    CS_ERR_FILE_NAME_INVALID,///<文件名无效
    CS_ERR_STRING_TOO_LONG,///<字符串太长
    CS_ERR_DATE_STR_TOO_LONG,///<日期字符串太长
    CS_ERR_DATA_OUT_OF_RANGE,///<数据越界
    CS_ERR_INDEX_INVALID,///<索引值无效
    CS_ERR_KEY_VALUE_INVALID,///<键值无效
    CS_ERR_ELE_INDEX_INVALID,///<对象索引无效
    CS_ERR_TEST_MODE_INVALTD,///<测试模式非法
    
    CS_ERR_COM_BUSY,///<串口忙
    CS_ERR_SEND_SUCCESS,///<发送成功
    CS_ERR_SEND_FAIL,///<发送失败
    CS_ERR_COMM_TROUBLE,///<通信故障或从机不存在
    CS_ERR_COMM_ADDR_INVALTD,///<通信地址无效
}CS_ERR;

typedef enum{
    COMM_ST_NO_ERROR        = 0,    //<执行成功
    COMM_ST_SYNTAX_ERROR    = 102,  //<语法错误
    COMM_ST_NOT_EXE         = 105,  //<指令执行不允许
    COMM_ST_PAR_INVALID     = 108,  //<接收到指令不允许的参数
    COMM_ST_MISSING_PAR     = 109,  //<指令串中遗漏了参数
    COMM_ST_UNDEFINED       = 113,  //<未定义的指令
    COMM_ST_PAR_TYPE_ERROR  = 120,  //<参数类型错误
    COMM_ST_DATA_INVALID    = 151,  //<不允许的字符串参数
    COMM_ST_TIME_OUT        = 152,  //<指令执行超时
    COMM_ST_OUT_OF_RANGE    = 222,  //<参数值超出允许范围
    COMM_ST_SEND_TOO_FAST   = 110,  //<指令发送间隔时间短
    COMM_ST_CRC_ERROR       = 119,  //<指令CRC校验错误
}COMM_ST;


typedef struct{
    char name[20]; ///< 模块名称
    char version[10];///< 模块版本
    uint32_t id;///< 模块ID
}MODULE_INF;

typedef struct{
    COM_NUM com_num;///<串口编号
    MODULE_INF module_inf;///<模块信息
    uint8_t buf[512];///<通信缓冲区
}ROAD_INF;
/**
  * @brief  管理所有路的信息为了加快通过路号对各模块地址的查找将所有的有效地址依次存入
  *         这个结构中
  */
typedef struct{
    uint8_t road_buf[64];///< road地址缓冲 第0个元素放第1路的地址1-15 17-31 33-47 49-63
    uint8_t count;///< 当前的总路数
}ROADS_FLAG;
/**
  * @brief  同步测试端口信息
  */
typedef struct{
    COM_STRUCT *com;
}SYN_TEST_PORT_INF;

typedef enum{
    CS_FALSE,
    CS_TRUE,
}CS_BOOL;

#pragma pack(1)
typedef struct{
    uint8_t addr;
    uint16_t fun_code;
    uint8_t st;
    uint8_t data[512];
}FRAME_T;
#pragma pack()

typedef struct{
    uint8_t index;///<索引值
    uint8_t mode[10];///<校准点的模式
    uint8_t point[10];///<校准点
    CS_UNIT_ENUM unit;///<单位
    uint8_t decs;///<小数点个数
    uint8_t lon;///<总长度包含小数点
    uint32_t mul_power;///<倍率 例如:电压单位0.1V 倍率就为10
}CAL_POINT_INF;

typedef struct{
    uint8_t work_st;///<工作状态，0不参与工作，1参与工作
    uint8_t vol[10];///<电压值
    uint8_t cur[10];///<电流值
    uint8_t time[10];///<时间
    uint8_t status;///<测试状态
    uint8_t mode[5];///<测试模式
    uint8_t step[8];///<测试步骤
    CS_UNIT_ENUM vol_unit;///<电压单位
    CS_UNIT_ENUM cur_unit;///<电流/电阻单位
    CS_UNIT_ENUM real_unit;///<真实电流等单位
    uint8_t usable;///<数据可以使用标记
}COMM_TEST_DATA;

#ifdef   MODULE_GLOBALS
#define  MODULE_EXT
#else
#define  MODULE_EXT  extern
#endif

MODULE_EXT ROAD_INF    *road_inf_pool;
MODULE_EXT ROADS_FLAG  roads_flag;
MODULE_EXT SYN_TEST_PORT_INF   syn_test_port[8];///最多8路同步测试
MODULE_EXT uint8_t g_module_num;

extern COM_STRUCT com1;

extern MODULE_INF module;

extern void com_comm_status_machine(COM_STRUCT *com);

extern void com_receive_dispose(COM_STRUCT *com, uint8_t *frame, uint32_t len);
extern CS_ERR com_module_connect(uint8_t addr);

extern void init_module_manage_env(void);
extern void module_comm_task(void);

#endif //__MODULE_MANAGE_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
