
#include "stm32f10x.h"
#include "cs99xx_plc.h"
#include "serve_test.h"
#include "MC14094.h"
#include "cs99xx_relay_motion.h"

void plc_delay_off(void)
{
    if(g_plc_dly)
    {
        if(--g_plc_dly == 0)
        {
			plc_signal_cmd(PLC_SIG_RESET);
            LED_FAIL = LED_OFF;
        }
    }
}

/* 向PLC发送测试信号 */
void plc_signal_cmd(uint8_t cmd)
{
    if(sys_par.plc_on_off == PLC_OFF)
    {
        return;
    }
    
    switch(cmd)
    {
		case PLC_SIG_EXIT:
			cur_status_plc = PLC_NO_READY;
			cur_plc_err_st = PLC_NO_ERR;
            MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_PASS, RELAY_OFF);
			break;
        case PLC_SIG_TESTING:
			cur_status_plc = PLC_TESTING;
			cur_plc_err_st = PLC_NO_ERR;
            MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_ON);
            MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_PASS, RELAY_OFF);
            break;
        case PLC_SIG_RESET:
			cur_status_plc = PLC_READY;
			cur_plc_err_st = PLC_NO_ERR;
            MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_PASS, RELAY_OFF);
            break;
        case PLC_SIG_PASS:
			cur_status_plc = PLC_PASS;
			cur_plc_err_st = PLC_NO_ERR;
            g_plc_dly = g_cur_file->hold_pass_time*100;/* 如果pass保持时间为0的话就保持pass信号 */
            
			if(OVER)
			{
				MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
			}
			
			if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
			{
				MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
			}
			
            MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, RELAY_OFF);
            MC14094_CMD(MC14094_A, MC14094_PLC_PASS, RELAY_ON);
            break;
        case PLC_SIG_FAIL:
			cur_status_plc = PLC_FAIL;
            /* 向PLC发送测试失败的信号 */
//             cancel_plc_test_signal();
			if(sys_par.fail_mode == FAIL_MODE_CON || sys_par.fail_mode == FAIL_MODE_FPDFC)
			{
				MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_ON);
			}
			else
			{
				MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
			}
			
			if(cur_step == g_cur_file->total)
			{
				MC14094_CMD(MC14094_A, MC14094_PLC_TEST, RELAY_OFF);
			}
			
            MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, RELAY_ON);
            MC14094_CMD(MC14094_A, MC14094_PLC_PASS, RELAY_OFF);
            break;
    }
    
    MC14094_Updata();/* 向4094发送数据 更新继电器状态 */
}

void cancel_plc_test_signal(void)
{
    if(sys_par.plc_signal != TEST_HOLD)
    {
        MC14094_CMD(MC14094_A,  MC14094_PLC_TEST,  RELAY_OFF);
    }
}

