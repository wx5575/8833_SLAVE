
#define MODULE_GLOBALS
#include "module.h"
#include "app.h"
#include "keyboard.h"
#include "GUI_Draw.h"
#include "dc_module.h"
#include "serve_test.h"
#include "string.h"
#include "cs99xx_mem_api.h"
#include "mb_master.h"

#define MD_ONE_PAGE_NUM 5


void dis_one_module(MODULE_INFO *p, uint8_t n)
{
}

void dis_one_page_modules(MODULE_INFO *p, uint8_t base, uint8_t num)
{
}

extern void start_scan_module(MODULE_INFO *p, uint8_t *num)
{
}

/* 自动扫描模块 */
extern void auto_scan_module(void)
{
}
extern void auto_scan_modules(void)
{
	auto_scan_module();
}

void scan_modules(void)
{
}


/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

