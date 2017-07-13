/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：keyboard.h
 * 摘  要  ：主程序
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#define KEY_START	0XFFF0
#define KEY_STOP	0XFFF1
#define KEY_0	    0XFFF7	//1111 0111
#define KEY_1	    0XFFEF	//1110 1111
#define KEY_2	    0XFFDF	//1101 1111
#define KEY_3	    0XFFBF	//1011 1111
#define KEY_4	    0XFF7F	//0111 1111
#define KEY_5	    0XFFFE	//1111 1110
#define KEY_6	    0XFFFD	//1111 1101
#define KEY_7	    0XFFFB	//1111 1011
#define KEY_8	    0XF7FF	//1111 0111
#define KEY_9	    0XEFFF	//1110 1111
#define KEY_EXIT	0XDFFF  //1101 1111
#define KEY_ENTER	0XBFFF  //1011 1111
#define KEY_F1		0X7FFF  //0111 1111
#define KEY_F2		0XFEFF  //1111 1110
#define KEY_F3		0XFDFF  //1111 1101
#define KEY_F4		0XFBFF  //1111 1011
#define KEY_NONE	0XFFFF

#define KEY_UP		KEY_F1
#define KEY_DOWN	KEY_F2
#define KEY_SHIFT	KEY_F3
#define KEY_BACK	KEY_F4

#define KEY_SET		KEY_UP
#define KEY_MENU	KEY_DOWN
#define KEY_TEST	KEY_SHIFT
#define KEY_HELP	KEY_BACK
#define KEY_SHORTCUT	KEY_SHIFT

#define KEY_INSERT	KEY_UP
#define KEY_DEL		KEY_DOWN
#define KEY_MORE	KEY_F3

#define KEY_ADVANCE KEY_UP
#define KEY_RETREAT KEY_DOWN//retreat
#define KEY_SWAP	KEY_SHIFT

#define KEY_SAVE	KEY_F1
#define KEY_NEW		KEY_F2

#define KEY_SHIFT_L KEY_F1
#define KEY_SHIFT_R	KEY_F2
#define KEY_FILE_DEL		KEY_F3

#define KEY_READ	KEY_F2
#define KEY_DEL_FILE		KEY_F1
#define KEY_EDIT_FILE		KEY_F2

#define KEY_DEL_PW	KEY_F3

/* 测试界面中使用 */
#define KEY_TEST_MEM	KEY_F1
#define KEY_TEST_SET	KEY_F2
#define KEY_TEST_MORE	KEY_F3
#define KEY_TEST_RES	KEY_F2

/* 结果界面使用 */
#define KEY_RES_TRANS	KEY_F1
#define KEY_RES_JUMP	KEY_F2

#define KEY_RES_CLEAR	KEY_F1
#define KEY_RES_CENSUS	KEY_F2


#define KEY_DELAY_NORMAL	(100)
#define KEY_DELAY_CAL		(100)

#ifdef   KEY_GLOBALS
#define  KEY_EXT
#else
#define  KEY_EXT  extern
#endif

KEY_EXT int16_t key_buzzer_time;/* 按键蜂鸣长度 */

#define BUTTON_FILTER_TIME 	(12)		/* 消抖时间 */
#define BUTTON_LONG_TIME 	(100)		/* 持续1秒，认为长按事件 */

typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t Count;			/* 滤波器计数器 */
	uint8_t FilterTime;		/* 滤波时间(最大255,表示2550ms) */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint16_t State;			/* 按键当前状态（按下还是弹起） */
	uint16_t KeyCodeUp;		/* 按键弹起的键值代码, 0表示不检测按键弹起 */
	uint16_t KeyCodeDown;	/* 按键按下的键值代码, 0表示不检测按键按下 */
	uint16_t KeyCodeLong;	/* 按键长按的键值代码, 0表示不检测长按 */
}KEY_T;

extern void init_keyboard(void);
extern uint16_t scan_keyboard(void);
extern void report_key_value(void);

#endif //__KEYBOARD_H__

/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE****/
