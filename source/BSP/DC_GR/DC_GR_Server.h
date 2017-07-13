#ifndef __DC_GR_SERVER_H
#define __DC_GR_SERVER_H

#include "stm32f10x_conf.h"
#include "USART.h"
#include "CMD_Dispose.h"
#include "common_tools.h"


typedef struct {
    double KV;
    double KI;
    double KDac;
	
    double BV;
    double BI;
    double BDac;
} CALIB_MSG;

typedef struct {
    CALIB_MSG CalibMsg;
    uint32_t TestTime;      //mS为单位
    uint32_t test_current;    //测试电流
} TEST_PARAM;

typedef struct {
    uint32_t I_Val;
    uint32_t V_Val;
    uint32_t R_Val;
}RECEIVE_DATA;

typedef enum {
    TEST_RUN,
    TEST_EMPTY,
    TEST_ERROR
}TEST_STATUS;

extern FlagStatus CmdReceiveFlag;

extern TEST_STATUS test_status;

extern RECEIVE_DATA receive_data;


/*参数读写模块*/
void WriteCalibParam(CALIB_MSG * WriteCalib);
void WriteTestTimeParam(uint32_t WriteTime);
void WriteTestCurrentParam(uint32_t WriteCurrent);
void WriteAllParam(TEST_PARAM * WriteParam);
double ReadTestCurrentParam(void);
uint32_t ReadTestTimeParam(void);
CALIB_MSG * ReadCalibParam(void);

#define DC_GR_START			(1)
#define DC_GR_STOP			(2)
#define DC_GR_READ_DATA		(3)
#define DC_GR_CUR_START		(4)
#define DC_GR_CAL_START		(5)
#define SET_DC_GR_VOL_GEAR	(6)
#define SET_DC_GR_TEST_METHOD	(7)

void Start_Dispose(void);
void Stop_Dispsoe(void);                                                  
void Set_Time_Dispose(uint32_t time);
void Ask_Data(void);

void Set_Vol_Gear_Dispose(uint8_t vol_gear);

//void Set_Calib_Dispose(void);

void Test_Calib_Mode(uint32_t CalibCurrent);

int32_t ReceiveCmdScan(void);

void ReceiveTestDataDispose(char *cmd_data);
void ReceiveTestOverDispose(char *cmd_data);
void ReceiveOpenErrorDispose(void);

// void dc_gr_usart_cmd(const uint8_t n);


#endif
