#include "DC_GR_Server.h"
// #include "General.h"
#include 	"stdlib.h"
#include	<stdio.h>
#include 	<math.h>
#include	<string.h>

#include    "ucos_ii.h"
#include    "stm32f10x.h"
#include	"STM32_RCC_Config.h"
#include 	"STM32_GPIO_Config.h"
#include 	"STM32_FSMC.H"

#include	"BSP_config.h"
#include 	"LCD24064.H"
#include 	"GUI_Draw.h"
#include	"Font_DIY.h"
#include	"keyboard.h"
// #include    "spi_flash.h"


#include	"RTC_Config.h"
#include	"Timer_Config.h"
#include 	"MC14094.h"
#include 	"Timer_Config.h"
#include 	"STM32_ADC.H"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include	"i2c_ee.h"
#include	"ADC_DMA.h"


#include	"cs99xx_config.h"
#include	"app_cfg.h"
#include 	"app.h"
#include	"irq.h"
#include    "cs99xx_result.h"
#include	"cs99xx_struct.h"
#include	"cs99xx_parameter.h"
#include    "cs99xx_mem_api.h"
#include 	"cs99xx_eep_manage.h"
#include    "cs99xx_flash_manage.h"
#include 	"serve_test.h"
#include	"cs99xx_exception.h"
#include	"cs99xx_collect.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include	"calibration.h"

#include	"USART.h"
#include	"usart2.h"
#include	"DC_GR_Server.h"
#include	"Com_Server.h"

static TEST_PARAM test_param;
RECEIVE_DATA receive_data;
TEST_STATUS test_status;

FlagStatus CmdReceiveFlag;

/*********************************************
测试参数读写模块
*********************************************/

/*赋值校准系数*/
static void Set_Calib_Dispose(void);

void WriteCalibParam(CALIB_MSG * WriteCalib)
{
    test_param.CalibMsg.KV = WriteCalib->KV ;
    test_param.CalibMsg.KI = WriteCalib->KI ;
    test_param.CalibMsg.KDac = WriteCalib->KDac ;
    test_param.CalibMsg.BV = WriteCalib->BV ;
    test_param.CalibMsg.BI = WriteCalib->BI ;
    test_param.CalibMsg.BDac = WriteCalib->BDac ;

    Set_Calib_Dispose();
}

/*写入测试时间*/
void WriteTestTimeParam(uint32_t WriteTime)
{
    test_param.TestTime = WriteTime;
}

/*写入测试电流 单位mA*/
void WriteTestCurrentParam(uint32_t WriteCurrent)
{
    test_param.test_current = WriteCurrent;
}


/*赋值所有的参数*/
void WriteAllParam(TEST_PARAM * WriteParam)
{
    WriteCalibParam(&(WriteParam->CalibMsg));
    test_param.TestTime = WriteParam->TestTime;
    test_param.test_current = WriteParam->test_current;
}

double ReadTestCurrentParam(void)
{
    return test_param.test_current; 
}

uint32_t ReadTestTimeParam(void)
{
    return test_param.TestTime;
}

CALIB_MSG * ReadCalibParam(void)
{
    return & test_param.CalibMsg;
}


/*********************************************
测试控制模块
*********************************************/
/*启动测试*/
void Start_Dispose(void)
{
    test_status = TEST_RUN; 
    Send_Test_Start(test_param.test_current); 
}


/*停止测试*/
void Stop_Dispsoe(void)
{
   Send_Test_Stop();
}

void Ask_Data(void)
{
    Send_Ask_Result();
}

void Set_Vol_Gear_Dispose(uint8_t vol_gear)
{
	Send_Vol_Gear(vol_gear);
}

/*设置模块测试时间*/
void Set_Time_Dispose(uint32_t Time)
{
    test_param.TestTime = Time;
    Send_Test_Time(test_param.TestTime);
}

/*设置模块的校准参数*/
extern void Send_Calib( CALIB_MSG * WriteCalib);
static void Set_Calib_Dispose(void)
{
    Send_Calib(&(test_param.CalibMsg));
}

/*向下位机发送校准指令*/
void Test_Calib_Mode(uint32_t CalibCurrent)
{
    test_status = TEST_RUN;
    Send_Test_Calib(CalibCurrent);
}

/*
    控制端处理函数，其余地方进行重构
*/
// void DC_GR_TestDataDispsoe(RECEIVE_DATA * receive_data)
// {
// }

void DC_GR_ErrorDispose(void)
{
}

void DC_GR_TestOverDispsoe(void)
{
}

/*接收到下位机发送的数据*/
int32_t ReceiveCmdScan(void)
{
    if(CmdReceiveFlag == SET) {
        CmdReceiveFlag = RESET;
        return ReceiveCmdDispose((char*)ReceiveCmdData);
    }
	return -1;
}


void ReceiveTestDataDispose(char *cmd_data)
{
    char Receive_data[3][20];
    const uint8_t R_Val_Num = 0;
    const uint8_t V_Val_Num = 1;
    const uint8_t I_Val_Num = 2;
	
    SplitCmdData (cmd_data , Receive_data , 3);
	
    receive_data.R_Val = StrToInt(Receive_data[R_Val_Num]);
    receive_data.V_Val = StrToInt(Receive_data[V_Val_Num]);
    receive_data.I_Val = StrToInt(Receive_data[I_Val_Num]);
// 	test_flag.uart_ok = 1;
//     DC_GR_TestDataDispsoe(&receive_data);
}
#include 	"serve_test.h"
#include "app.h"
void ReceiveOpenErrorDispose(void)
{
    test_status = TEST_ERROR;
	
	test_flag.dc_gr_open_err = ERR_OPEN;
// 	test_flag.uart_ok = 1;
	
//     DC_GR_ErrorDispose();
}

void ReceiveTestOverDispose(char *cmd_data)
{
    test_status = TEST_EMPTY; 
    DC_GR_TestOverDispsoe();  
}



int32_t dcgr_communication_check(void)
{
	int32_t i = 0;
	int32_t err = 0;
	
	for(i = 0;i < 5; i++)
	{
		Ask_Data();
		OSTimeDlyHMSM(0,0,0,50);
		if(SET == CmdReceiveFlag)
		{
			break;
		}
		
// 		if(0 == strcmp(ReceiveCmdData,"OK#"))
// 		{
// 			break;
// 		}
	}
	
	if(i == 5)
	{
		err = -1;
	}
	
	return err;
}

// void dc_gr_usart_cmd(const uint8_t n)
// {
// 	switch(n)
// 	{
// 		case DC_GR_START:
// 			Start_Dispose();
// 			break;
// 		case DC_GR_STOP:
// 			Stop_Dispsoe();
// 			break;
// 		case DC_GR_READ_DATA:
// 			Ask_Data();
// 			break;
// 		case DC_GR_CAL_START:
// 			Test_Calib_Mode(cur_vol);
// 			break;
// 		case DC_GR_CUR_START:
// 			if(cur_vol > 0)
// 			{
// 				Test_Calib_Mode(cur_vol*ratio_gr.dac_k[cur_gear]+ratio_gr.dac_b[cur_gear]);/* 启动直流模块 */
// 			}
// 			else
// 			{
// 				Test_Calib_Mode(cur_vol);/* 启动直流模块 */
// 			}
// 			break;
// 		case SET_DC_GR_VOL_GEAR:
// 			Set_Vol_Gear_Dispose(cur_gear);
// 			break;
// 		case SET_DC_GR_TEST_METHOD:
// 			Send_Test_Method(cur_method);
// 			break;
// 	}
// }


