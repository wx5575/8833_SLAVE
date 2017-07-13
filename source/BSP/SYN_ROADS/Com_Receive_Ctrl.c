#include    "Com_Receive_Ctrl.h"
#include	"cs99xx_config.h"
#include 	"serve_test.h"
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
#include    "spi_flash.h"


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

#include 	"SysTemConfig.h"

#if SYNCHRO_ROADS_EN>0

#define     DATA_PLACE          (CMD_PLACE + 1)
#define     Reult_data          (4 + 10 * 2)
#define     Result_Data_Num     ( Reult_data + DATA_PLACE )
#define		Return_adc			(10 * 2)
#define		Return_Adc_Num		(Return_adc + DATA_PLACE)


typedef enum {
    No_Error = 1,           /*指令执行完成*/
    Execute_Not_Allowed,    /*当前状态无法执行该语句*/
    Check_Error,            /*校验失败*/
    Param_Error,            /*参数错误*/

    Return_Data,            /*返回结果为参数*/
	Return_ADC_DATA,
	START_OK,
}Return_Type;

Result_Inf result_inf; /* 测试结果信息 */
uint16_t syn_adc_val[INPUT_NUM];/* 测试的ADC数据 */

static Single_Switch check_data_ok(uint8_t * receive_data, uint8_t data_num)
{
	Single_Switch status;
	uint16_t receive_crc ,crc_data ;

	receive_crc = ( receive_data[data_num - 1]<< 8  )|(receive_data[data_num - 2]);

	crc_data = read_check_data(receive_data,  data_num -2 );  

	if(receive_crc == crc_data) {
		status = YES;
	}
	else {
		status = NO;
	}
	
	return status;
}

static Return_Type read_cmd(uint8_t * receive_data)
{
    return (Return_Type)receive_data[CMD_PLACE];
}

static void Get_Data(uint8_t * receive_data, uint8_t data_num)
{
    uint8_t i;
    uint16_t temp_i[INPUT_NUM];

    if(data_num == Result_Data_Num) {
        result_inf.Road_Msg.road_int_msg   = char_uint32(receive_data + PARAM_PLACE);
		
		if(result_inf.Road_Msg.road_int_msg ) 
		{
			result_inf.Road_Msg.road_int_msg = result_inf.Road_Msg.road_int_msg ;
		}
        
        for(i = 0; i < INPUT_NUM; i++) {
            temp_i[i] = char_to_uint16(receive_data + PARAM_PLACE + i * 2 + 4);

//             if(temp_i[i] < 0 ) {
//                 /*数据不能小于0*/
//                 return;
//             }
//             else
				if(temp_i[i] > R_I_MAX) {
                /*数据不能超过接收数据的最大限制*/
                return ;
            }
        }

        for(i = 0; i < INPUT_NUM; i++) {
            result_inf.Road_current[i] = temp_i[i]; 
        }    
    }
}

static void Get_ADC_Data(uint8_t * receive_data, uint8_t data_num)
{
    uint8_t i;

    if(data_num == Return_Adc_Num) {
		for(i = 0; i < INPUT_NUM; i++) {
			syn_adc_val[i] = char_to_uint16(receive_data + PARAM_PLACE + (i * 2)); 
		}            
    }
}

static void cmd_Dispose(Return_Type cmd, uint8_t * receive_data, uint8_t data_num)
{
    switch(cmd) {
        case No_Error:
            /*通知发送队列，本次问答正确*/
			test_flag.syn_com_ok = 1;
            Comm_OK();
        break;
		
        case Execute_Not_Allowed:
            
        break;
		
        case Check_Error:
            /*数据重新发送*/
//             Send_Data();
        break;
		
        case Param_Error:
		
        break;
		
        case Return_Data:
			test_flag.syn_com_ok = 1;
            Get_Data(receive_data, data_num);
            Comm_OK();
        break;
		
		case Return_ADC_DATA:
			test_flag.syn_com_ok = 1;
			Get_ADC_Data(receive_data, data_num);
			Comm_OK();
		break;
		
		case START_OK:
			test_flag.syn_com_ok = 1;
			Comm_OK();
		break;
    }
}



// #include "General.h"

int32_t Receive_data_Dispose(uint8_t * receive_data, uint8_t data_num)
{
	static int n = 0;
	static int m = 0;
	
    Return_Type cmd;
	
    if(check_data_ok(receive_data, data_num) == YES)
	{
		n++;
        data_num -= 2;    //减去校准参数位
        cmd = read_cmd(receive_data);
        cmd_Dispose(cmd, receive_data, data_num);
		
		return 0;
    }
	/* 数据 检查出错 重新发送 */
	else
	{
// 		Send_Data();
		m++;
		return -1;
	}
}


#endif





