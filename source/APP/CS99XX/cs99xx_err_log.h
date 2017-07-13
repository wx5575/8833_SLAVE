/*
 * Copyright(c) 2015,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_err_log.h
 * 摘  要  ：错误日志相关的接口
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */
#ifndef __CS99XX_ERR_LOG_H__
#define __CS99XX_ERR_LOG_H__


#include <stdio.h>

/* 记录程序异常 该宏不能实现为函数 原因不言自明 因此不可滥用 */
#define RECORD_EXCEP(info)  \
        {   \
            sprintf(g_excep_t.excep_buf, "%s %d %s", __FUNCTION__, __LINE__, (info));   \
            updata_excep_info(&g_excep_t); \
        }

/* 给s赋值前先检查下t是否是合法的浮点数，再赋值 */
#define SAFE_FLOAT_VOLU_CAL(s , t) \
        if(!isnan(t) && !isinf(t)) \
        { \
            s = t; \
        } \
        else \
        { \
            s = 1; \
        }
//             RECORD_EXCEP("cal_ratio err"); \
        
typedef struct{
    char excep_buf[40];                 /* 保存异常现场 */
    uint32_t excep_time;                /* 异常发生的时间 */
}EXCEP_INFO_STR;//记录异常结构
#ifdef   _99xxERR_LOG_GLOBALS
#define  _99xxERR_LOG_EXT
#else
#define  _99xxERR_LOG_EXT  extern
#endif


_99xxERR_LOG_EXT EXCEP_INFO_STR         g_excep_t;


void updata_excep_info(EXCEP_INFO_STR *p_excep);
void clear_excep_info(void);


#endif //__CS99XX_ERR_LOG_H__

/******************* (C) COPYRIGHT 2015 长盛仪器 *****END OF FILE****/
