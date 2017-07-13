#include "Com_Server.h"
#include "USART.h"
#include "usart2.h"
#include "Com_Receive_Ctrl.h"
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

#if SYNCHRO_ROADS_EN>0

extern int32_t Receive_data_Dispose(uint8_t * receive_data, uint8_t data_num);
/*结束符*/
uint8_t over_data[] = {0x0D, 0x0A};
// typedef enum { NO, YES}Single_Switch;

static uint8_t syn_receive_data[BUFFER_SIZE];

Single_Switch Receive_Over(uint8_t data_num)
{
    if((syn_receive_data[data_num - 2] == over_data[0]) &&
            ((syn_receive_data[data_num - 1] == over_data[1])))  {
        return YES;
    }
    else if((syn_receive_data[data_num - 2] == '\0') &&
            (syn_receive_data[data_num - 1] == '0')) {
 		Clear_Usart_data();
		return NO;
	}
	else {
        return NO;
    }
}

static uint16_t CrcCal(uint8_t receive_data , uint16_t crc_poly, uint16_t crc_data)
{
    uint8_t i;
    receive_data *= 2;

    for(i = 8; i > 0; i--) {
        receive_data /= 2;
        if((receive_data ^ crc_data) & 1 ) {
            crc_data = (crc_data / 2) ^ crc_poly;       
        }
        else {
            crc_data /= 2; 
        }
    }

    return crc_data;
}

uint16_t read_check_data(uint8_t * data, uint8_t data_num)
{
    uint16_t crc_data = 0xffff;
    uint8_t i;
    const uint16_t CHECK_POLY = 0xa001;
    
    for(i = 0; i< data_num ; i++) {
        crc_data = CrcCal(data[i] , CHECK_POLY , crc_data );
    }  
	
    return crc_data ;
}
int32_t find_cmd_end(char ch1, char ch2)
{
	return (ch1==0x0D && ch2==0x0A);
}

int32_t Syn_Scan_usart(void)
{
// 	if(uart5_timeout == 0)
// 	{
// 		if(receive_ok == SET)
// 		{
// 			cur_point = !cur_point;
// 			syn_data_num[cur_point] = 0;
// 			return Receive_data_Dispose(syn_receive_buf[!cur_point], syn_data_num[!cur_point] - sizeof(over_data));
// 		}
// 		receive_ok = RESET;
// 	}
// 	return -1;
	
	
	if(receive_ok == SET)
	{
		receive_ok = RESET;
// 		memcpy(syn_receive_data, receive_buf, receive_data_num);
// 		data_num = receive_data_num;
// 		receive_data_num = 0;
// 		data_pos = (cur_point+1)%3;
// 		memset(syn_receive_buf[cur_point], 0, syn_data_num[cur_point]);
		
		
//		return Receive_data_Dispose(syn_receive_buf[data_pos], syn_data_num[data_pos] - sizeof(over_data));
 		return Receive_data_Dispose(syn_receive_buf[!cur_point], syn_data_num[!cur_point] - sizeof(over_data));
	}
	return -1;
}

/*调用后开始测试*/
void Syn_Start_Test(void)
{
    Write_Cmd(TEST_START);
    Send_Rear_Dispose();
}

/*调用后停止测试*/
void Syn_Stop_Test(void)
{
    Write_Cmd(TEST_STOP);
    Send_Rear_Dispose();
}

/*调用后复位测试*/
void Syn_Reset_Test(void)
{
    Write_Cmd(TEST_RESET);
    Send_Rear_Dispose();
}

/*调用后设置校准参数*/
void Syn_Set_20mACalib(float * calib_data)
{
    uint8_t calib_char[100];    
    uint8_t i;

    Write_Cmd(SET_20mA_CALIB);

    for(i = 0; i < INPUT_NUM; i++ ) {
        float_to_char(calib_data[i], calib_char + (i * 4) );
    }

    Write_char_data(calib_char, 4 * INPUT_NUM);


    Send_Dispose();
}

/*调用后设置校准参数*/
void Syn_Set_2mACalib(float * calib_data)
{
    uint8_t calib_char[100];    
    uint8_t i;

    Write_Cmd(SET_2mA_CALIB);

    for(i = 0; i < INPUT_NUM; i++ ) {
        float_to_char(calib_data[i], calib_char + (i * 4) );
    }

    Write_char_data(calib_char, 4 * INPUT_NUM);
	
    Send_Dispose();
}

void Syn_Set_20mAOffSet(uint16_t * OffSet_data)
{
    uint8_t off_set_char[100];    
    uint8_t i;

    Write_Cmd(SET_20mA_OFF_SET);

    for(i = 0; i < INPUT_NUM; i++ ) {
        uint16_to_char(OffSet_data[i], off_set_char + (i * 2) );
    }

    Write_char_data(off_set_char, 2 * INPUT_NUM);


    Send_Dispose();
}

void Syn_Set_2mAOffSet(uint16_t * OffSet_data)
{
    uint8_t off_set_char[100];
    uint8_t i;

    Write_Cmd(SET_2mA_OFF_SET);

    for(i = 0; i < INPUT_NUM; i++ ) {
        float_to_char(OffSet_data[i], off_set_char + (i * 2) );
    }

    Write_char_data(off_set_char, 2* INPUT_NUM);


    Send_Dispose();
}

/*调用后读取模块的电流结果*/
void Syn_Read_Result(void)
{
    Write_Cmd(READ_RESULT);
    Send_Dispose();
}

/*调用后读取模块的ADC结果*/
void Syn_Read_Adc_Val(void)
{
    Write_Cmd(READ_ADC_VAL);
    Send_Dispose();
}

/*调用后设置上下限*/
/*上下限数据以mA为单位*/
void Syn_Set_limit_f(float high_limit, float low_limit)
{
    uint8_t limit_char[100];

    Write_Cmd(SET_LIMIT);

    float_to_char(high_limit, limit_char);
    float_to_char(low_limit , limit_char + 4);
    
    Write_char_data(limit_char, 8);

    Send_Dispose();
}


/*调用后设置上下限*/
/*上下限数据以 10uA 为单位*/
void Syn_Set_limit_i(uint16_t high_limit, uint16_t low_limit)
{
    float high_f,low_f;

    high_f = 1.0 * high_limit / 100;
    low_f  = 1.0 * low_limit  / 100;

    Syn_Set_limit_f(high_f, low_f);
}



void syn_usart_cmd(const uint8_t n)
{
// 	int32_t flag = 0;
	
	switch(n)
	{
		case SYN_START:
			Syn_Start_Test();
			break;
		case SYN_STOP:
			Syn_Stop_Test();
			break;
		case SYN_RESET:
			Syn_Reset_Test();
			break;
		case SYN_CUR:
			Syn_Read_Result();
			break;
		case SYN_ADC:
			Syn_Read_Adc_Val();
			break;
		case SYN_CAL:
// 			if(flag == 0)
// 			{
// 				Syn_Set_2mACalib(ratio_acw.syn_adc_2mA);
// 			}
// 			else
// 			{
// 				Syn_Set_20mACalib(ratio_acw.syn_adc_20mA);
// 			}
			break;
		case SYN_CAL_2mA:
			Syn_Set_2mACalib(ratio_acw.syn_adc_2mA);
			break;
		case SYN_CAL_20mA:
			Syn_Set_20mACalib(ratio_acw.syn_adc_20mA);
			break;
		case SYN_LIMIT:
			Syn_Set_limit_f(syn_roads_upper[0], syn_roads_lower[0]);
			break;
	}
}



#endif

