
#ifndef __CS99XX_PLC_H__
#define __CS99XX_PLC_H__

enum{
    PLC_SIG_TESTING,/* PLC发送正在测试信号 */
    PLC_SIG_RESET,/* PLC清除所有信号 */
    PLC_SIG_PASS,/* PLC发送PASS信息号 */
    PLC_SIG_FAIL,/* PLC发送FAIL信号 */
	PLC_SIG_EXIT,/* PLC退出测试界面 */
};

/* PLC状态 */
enum{
    PLC_NO_ERR,
	PLC_HIGH_FAIL,
	PLC_LOW_FAIL,
	
    PLC_NO_READY,
	PLC_READY,
	PLC_TESTING,
	PLC_PASS,
	PLC_FAIL,
};

extern void plc_delay_off(void);
extern void plc_signal_cmd(uint8_t cmd);
extern void cancel_plc_test_signal(void);

#endif //__CS99XX_PLC_H__
