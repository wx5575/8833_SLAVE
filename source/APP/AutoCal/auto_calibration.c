/*
 * Copyright(c) 2016,南京长盛仪器
 * All rights reserved
 * 文件名称：auto_calibration.c
 * 摘  要  ：自动校准系统
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#define AUTO_CALIBRATE_GLOBALS

#include "stm32f10x.h"
#include "GUI_Draw.h"
#include "auto_calibration.h"
#include "calibration.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "DAC_VREF.h"
#include "app.h"
#include "keyboard.h"
#include "auto_cal_server.h"
#include <string.h>
#include "auto_cal_comm.h"
#include "MC14094.h"
#include "dc_module.h"
#include "cs99xx_collect.h"


