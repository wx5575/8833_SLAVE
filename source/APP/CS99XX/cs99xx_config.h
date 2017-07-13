/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_config.h
 * 摘  要  ：仪器功能配置文件，在这里可以选择一些需要的功能，如多路同步等
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __CS99XX_CONFIG_H__
#define __CS99XX_CONFIG_H__



#define CUSTOM_TYPE_EN  0 /* 定制机开关 */


/************** 测试仪的配置 *****************************/



/************************************************/


#if CUSTOM_TYPE_EN>0
// #define LICHUN_DZ   //上海立春精密仪器定制机
// #define SUZHOU_MUTE
// #define VTC9929_DZ
// #define TSK7630_DZ //CS9929A-8改制 去除长盛标识 绝缘电阻 1M-10G
// #define SUZHOU_MUTE
#define RH8400AS_DZ
#endif


/*******************************************************/


#endif	//__CS99XX_CONFIG_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
