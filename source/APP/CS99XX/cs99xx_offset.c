/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_offset.c
 * 摘  要  ：偏移测试功能
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
#include	"cs99xx_type.h"
#include    "math.h"


/*************************** 外部变量 **************************/

/*
 * 函数名：offset_sel_mode
 * 描述  ：偏移测试分两种模式：连续，单次 该函数就是选择的
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
//static int8_t offset_sel_mode(void)
//{
//    return 0;
//}
void dis_offset_test_ports(const TEST_PORT *pport)
{
}
/*
 * 函数名：offset_dis_step
 * 描述  ：画出一步的数据信息
 * 输入  ：n 传入第几行
 * 输出  ：无
 * 返回  ：无
 */
//static void offset_dis_step(const uint8_t n)
//{
//}
/*
 * 函数名：dis_one_step
 * 描述  ：画出一步的偏移测试信息
 * 输入  ：p 传入当前步的指针 ， n 位置信息
 * 输出  ：无
 * 返回  ：无
// */
//static void dis_one_offset_step(const int8_t n)
//{
//}

/*
 * 函数名：draw_offset_one_page
 * 描述  ：画出一页的偏移测试信息
 * 输入  ：p 传入当前页的第一个步的指针
 * 输出  ：无
 * 返回  ：无
 */
//static void draw_offset_one_page(NODE_STEP *p)
//{
//}

/*
 * 函数名：sys_offset
 * 描述  ：系统偏移功能
 * 输入  ：const int8_t w 标记调用函数
 * 输出  ：无
 * 返回  ：无
 */
void sys_offset(uint32_t w)
{
}


/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
