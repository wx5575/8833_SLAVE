#ifndef __CS99XX_VREF_H__
#define __CS99XX_VREF_H__

#include "stm32f10x.h"

extern void test_vref(uint16_t cur_h);

#define		AC_SHORT_DIV		(5)
/* DA默认的短路输出基准 */
#define		SHORT_V_AC_1MA		(250)
#define 	SHORT_V_AC_2MA		(400)
#define 	SHORT_V_AC_5MA		(900)
#define 	SHORT_V_AC_8MA		(1300)
#define 	SHORT_V_AC_10MA		(1700)
#define 	SHORT_V_AC_15MA		(2700)
#define 	SHORT_V_AC_20MA		(3200)

#define		SHORT_V_DC_1MA		(300)
#define 	SHORT_V_DC_2MA		(400)
#define		SHORT_V_DC_5MA		(870)
#define		SHORT_V_DC_8MA		(1391)
#define 	SHORT_V_DC_10MA		(1739)
#define 	SHORT_V_DC_15MA		(2584)
#define 	SHORT_V_DC_20MA		(2684)

/* 电弧侦测基准 */
#define		ARC_BASE			(560.0)

#define		ARC_AC_DATUM_9			(1.2		* ARC_BASE)	/* 最灵敏 */
#define		ARC_AC_DATUM_8			(2.05		* ARC_BASE)
#define		ARC_AC_DATUM_7			(3.10		* ARC_BASE)
#define		ARC_AC_DATUM_6			(5.7		* ARC_BASE)
#define 	ARC_AC_DATUM_5			(6.0		* ARC_BASE)
#define 	ARC_AC_DATUM_4			(6.2		* ARC_BASE)
#define 	ARC_AC_DATUM_3			(6.3		* ARC_BASE)
#define 	ARC_AC_DATUM_2			(6.42857	* ARC_BASE)
#define 	ARC_AC_DATUM_1			(7.142857	* ARC_BASE)	/* 最不灵敏 */


#define		ARC_DC_DATUM_9			(2.75		* ARC_BASE)	/* 最灵敏 */
#define		ARC_DC_DATUM_8			(2.85		* ARC_BASE)
#define		ARC_DC_DATUM_7			(3.10		* ARC_BASE)
#define		ARC_DC_DATUM_6			(5.7		* ARC_BASE)
#define 	ARC_DC_DATUM_5			(6.0		* ARC_BASE)
#define 	ARC_DC_DATUM_4			(6.2		* ARC_BASE)
#define 	ARC_DC_DATUM_3			(6.3		* ARC_BASE)
#define 	ARC_DC_DATUM_2			(6.42857	* ARC_BASE)
#define 	ARC_DC_DATUM_1			(7.142857	* ARC_BASE)	/* 最不灵敏 */



#endif //__CS99XX_VREF_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
