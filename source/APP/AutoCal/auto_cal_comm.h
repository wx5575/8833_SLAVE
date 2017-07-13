
#ifndef __AUTO_CAL_COMM_H__
#define AUTO_CAL_COMM_H__

typedef enum{
AUTOCAL_AC_V, //交流电压
AUTOCAL_AC_A, //交流电流
AUTOCAL_DC_V, //直流电压
AUTOCAL_DC_A, //直流电流
AUTOCAL_RES , //电阻
AUTOCAL_GR_A, //接地电流
}CAL_MODE_T;
extern uint8_t *cal_mode_pool[][2];

//从机状态码
typedef enum{
	AUTOCAL_WAIT_CONNECT,	//未进入校准界面
	AUTOCAL_CONNECTED,		//已进入校准界面
	AUTOCAL_WAIT_DATA,		//等待校准数据
	AUTOCAL_OUTPUTING,		//正在输出
	AUTOCAL_SAVEDATA,		//保存数据
	AUTOCAL_WAIT_OUTPUT,	//等待输出
	AUTOCAL_WAIT_STOP,		//等待停止
}TESTER_ST;

void enter_auto_cal_ui(void);
void exit_auto_cal_ui(void);
uint8_t *get_cur_point_mode(void);
void get_auto_cal_status(uint16_t*data);
uint8_t *get_cur_point_range(uint8_t *buf);
void auto_cal_start_output(void);
void auto_cal_stop_output(void);
void set_auto_cal_measure(uint64_t measure);
void get_auto_cal_points(uint16_t *data);
int16_t load_auto_cal_point(uint16_t data);

#endif //AUTO_CAL_COMM_H__
