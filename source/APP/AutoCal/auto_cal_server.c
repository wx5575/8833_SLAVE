
#define AUTO_CAL_SERVER_GLOBALS
#include "stm32f10x.h"
#include "calibration.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "DAC_VREF.h"
#include "app.h"
#include "auto_calibration.h"
#include "auto_cal_server.h"
#include "string.h"
#include "auto_cal_comm.h"

uint8_t *cal_mode_pool[][2]={
	{"0","ACV"},//交流电压
	{"1","ACI"},//交流电流
	{"2","DCV"},//直流电压
	{"3","DCI"},//直流电流
	{"4","IRR"},//电阻
	{"5","GRI"},//接地电流
};





