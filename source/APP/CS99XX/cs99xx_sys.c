/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_sys.c
 * 摘  要  ：系统参数设置管理
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"RTC_Config.h"
#include    "cs99xx_type.h"

void set_float_sys_par(void *psys)
{
}
void set_either_or_sys_par(void *psys)
{
}

/*
 * 函数名：set_option_sys_par
 * 描述  ：设置系统多选参数
 * 输入  ：n 标记当前是偏移还是测试 0 测试 1偏移
 * 输出  ：无
 * 返回  ：无
 */
void set_option_sys_par(void *psys)
{
}
void set_plc_option_sys_par(void *psys)
{
}

void set_int_sys_par(void *pun)
{
}

/*
 * 函数名：prompt_for_check_data
 * 描述  ：当检验人员成功更新检验时间时弹出OK提示窗 每次开机仅能修改一次
 * 输入  ：无
 * 输出  ：无
 */
void prompt_for_check_data(void)
{
}

void sys_check_date(void)
{
}

void sys_sys(uint32_t w)
{
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
