/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_type_config.h
 * 摘  要  ：型号配置文件，包含了所有型号功能的配置，每个机型都有一个配置函数对应
 * 当前版本：V0.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */

#ifndef	__CS99XX_TYPE_CONFIG_H__
#define __CS99XX_TYPE_CONFIG_H__


void config_all_init(void)
{
	type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
	type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.ports_ctrl_type = MC14094_CTRL;
    type_spe.transformer_point = SINGLE_POINT;/* 单触点变压器 */
    type_spe.transformer_type = TRANS_5kV;/* 变压器类型 */
    
    type_spe.hz_type = HZ_TYPE_CONT;/* 频率设置为连续的 */
    type_spe.gr_hz_type = HZ_TYPE_GRADE;/* 频率设置为连续的 */
    type_spe.g_mode_en = ENABLE;/* 支持G模式 */
    type_spe.comm_check_en = ENABLE;/* 使能通信开机检查 */
    type_spe.contact_segment_point = 5000;/* 双触点变压器切换变压器触头的电压点 */
    type_spe.vol_segment_point = 500;/* 换挡分界点500V */
	type_spe.rise_time_min_value = 0;/* 上升时间的最小值 */
	
    type_spe.def_fail_mode = __STOP /**/| __HALT | __CON | __RESTART | __NEXT | __FPDFC/**/;
    
    /* 静音功能使能 苏州分公司定制 */
    type_spe.silent_en = MUTE_EN;
    
    type_spe.plc_signal_en = ENABLE;
    
    type_spe.interface_en = ENABLE;
    type_spe.interface_config = INTERFACE_PLC | INTERFACE_COMM /*| INTERFACE_USB_OUT | INTERFACE_USB_IN*/;
	
	type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;//ACW_20kV;//
	type_spe.acw_vol_h = 5000;	/* 5kV */
	type_spe.acw_vol_l = 50;/* 电压下限50v */
	type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
	type_spe.acw_real = ENABLE;/* 无真实电流 */
	type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA | _AC_20mA;
    type_spe.real_cur_format = CUR_FORMAT;/* 将真实电流转换成电流来显示 */
	
	type_spe.dcw_vol_range = DCW_6kV;//DCW_10kV;//DCW_20kV;//
	type_spe.dcw_vol_h = 6000;	/* 6kV */
	type_spe.dcw_vol_l = 50;/* 电压下限50v */
	type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    type_spe.dcw_big_cap_en = DISABLE;/* 单直流大电容测试 */
	
	type_spe.ir_vol_range = IR_1kV;
	type_spe.ir_vol_h = 1000; /* 1KV */
	type_spe.ir_vol_l = 50; /* 50v */
	type_spe.ir_res_h = 10*1000;	/* 10G */
	type_spe.ir_res_l = 1;	/* 1M */
	type_spe.ir_short_int_en = 0;/* IR短路中断使能 当电压高于1k 后就需要打开ir短路中断 */
	
	type_spe.gr_cur_unit = CUR_U_A;
	type_spe.gr_cur_decs = 2;
	type_spe.gr_cur_h = 3200;   /* 32A */
	type_spe.gr_cur_l = 300;    /* 3A */
    type_spe.gr_res_h = 5100;   /* 电阻上限510.0mohm */
    type_spe.gr_res_l = 0;      /* 电阻上限0.0mohm */
	type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
	type_spe.gr_dc_en = 0;      /* GR为直流模式 */
	type_spe.gr_cur_divide_gear_en = 0;/* 关闭电流分档采样 */
    type_spe.gr_max_cur_res_rang = 150;/* 150mΩ */
    type_spe.one_dc_module_max_cur = 3200;/* 32A */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA |*/ /*_AC_50mA | _AC_100mA |*/ _AC_200mA;
    type_spe.cc_vol_l = 500;/* 500V */
    type_spe.cc_vol_h = 500;/* 500V */
    
    type_spe.bbd_vol_h = 5000;/* BBD电压上限值 5000V */
    type_spe.bbd_vol_l = 100;/* BBD电压下限默认值100V */
    
	init_speciality();
}
void config_debug_type(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR/* | __BBD | __CC*/;
    type_spe.port_num = PORT_NUM8;//PORT_NUM4;//PORT_NUM0;//PORT_NUM14;//
    type_spe.def_fail_mode = __STOP /**/| __HALT | __CON /**/| __RESTART | __NEXT | __FPDFC;
    
    type_spe.plc_signal_en = ENABLE;/* 系统菜单项中已经没有PLC信号的设置，但PLC信号默认打开的且不用更改 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_l = 50;	/* 500V */
    type_spe.ir_res_h = 100*1000.0-1;	/* 100Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_cur_unit = CUR_U_A;
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = DISABLE;/* GR压降模式 1打开 0关闭 */
    type_spe.gr_cur_divide_gear_en = DISABLE;/* GR压降模式 电流分为两个档位 100mA 1000mA */
    type_spe.gr_vol_gear = GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;//GR_DC_12V;//
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9919AX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 9999Mohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9919BX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    type_spe.port_num = PORT_NUM4;//PORT_NUM0;//PORT_NUM8;//
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_5kV;//IR_1kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 9999Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9919B_4(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM4;//PORT_NUM0;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;
    type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_5kV;//IR_1kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 9999Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9929B_4(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM4;//PORT_NUM0;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_res_h = 5*1000.0;	/* 5Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9933G_4(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_amp_type = GR_AMP_40A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923G(void)
{
    type_spe.mode_en = /* __ACW |*/ __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_amp_type = GR_AMP_40A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923ECG(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR/* | __BBD | __CC*/;
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//PORT_NUM14;//
    type_spe.def_fail_mode = __STOP /**/| __HALT | __CON /**/| __RESTART | __NEXT | __FPDFC;
    type_spe.transformer_type = TRANS_10kV;// TRANS_1kV;// TRANS_2kV;//TRANS_5kV;//TRANS_20kV;/* 变压器类型 */
    
    type_spe.plc_signal_en = ENABLE;/* 系统菜单项中已经没有PLC信号的设置，但PLC信号默认打开的且不用更改 */
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_5kV;// ACW_20kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV; // DCW_20kV;//
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_1kV;//IR_5kV;//IR_10kV;//type_spe.ir_vol_h = 2500;	/* 2500V */
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_cur_unit = CUR_U_A;
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
	type_spe.gr_cur_h = 6000;   /* 60A */
	type_spe.gr_cur_l = 300;    /* 3A */
    type_spe.one_dc_module_max_cur = 3000;/* 30A */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = DISABLE;/* GR压降模式 1打开 0关闭 */
    type_spe.gr_cur_divide_gear_en = DISABLE;/* GR压降模式 电流分为两个档位 100mA 1000mA */
    type_spe.gr_vol_gear = GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;//GR_DC_12V;//
    type_spe.gr_max_cur_res_rang = 105;/* 105mΩ */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
    
	init_speciality();
}
void config_CS9923ECG_1(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR/* | __BBD | __CC*/;
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//PORT_NUM14;//
    type_spe.def_fail_mode = __STOP /**/| __HALT | __CON /**/| __RESTART | __NEXT | __FPDFC;
    type_spe.transformer_type = TRANS_10kV;// TRANS_1kV;// TRANS_2kV;//TRANS_5kV;//TRANS_20kV;/* 变压器类型 */
    
    type_spe.plc_signal_en = ENABLE;/* 系统菜单项中已经没有PLC信号的设置，但PLC信号默认打开的且不用更改 */
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_5kV;// ACW_20kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV; // DCW_20kV;//
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_5kV;//IR_2_5kV;//IR_1kV;//IR_10kV;//type_spe.ir_vol_h = 2500;	/* 2500V */
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_cur_unit = CUR_U_A;
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
	type_spe.gr_cur_h = 6000;   /* 60A */
	type_spe.gr_cur_l = 300;    /* 3A */
    type_spe.one_dc_module_max_cur = 3000;/* 30A */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = DISABLE;/* GR压降模式 1打开 0关闭 */
    type_spe.gr_cur_divide_gear_en = DISABLE;/* GR压降模式 电流分为两个档位 100mA 1000mA */
    type_spe.gr_vol_gear = GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;//GR_DC_12V;//
    type_spe.gr_max_cur_res_rang = 105;/* 105mΩ */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;

	init_speciality();
}
void config_CS9939X(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num =PORT_NUM8;// PORT_NUM4;//PORT_NUM0;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9933EG_1(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_5kV;//IR_10kV;//IR_1kV;//
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9933EG_2(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_5kV;//IR_2_5kV;//IR_10kV;//IR_1kV;//
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9914AM(void)
{
    type_spe.mode_en = /**/ __ACW /*| __DCW | __IR | __GR | __BBD | __CC*/;
    type_spe.transformer_point = DOUBLE_POINT;
	
	type_spe.contact_segment_point = 2500;/* 分段电压 2.5kv */
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA */| _AC_200mA/**/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9914BM(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW/* | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA */| _AC_200mA/**/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9917CX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW/* | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;//HZ_TYPE_GRADE;//
    
    type_spe.acw_vol_range = ACW_10kV;//ACW_5kV;//  ACW_20kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9917DX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW/* | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_vol_range = ACW_10kV;//ACW_5kV;//  ACW_20kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9929AX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;//
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ /**/;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;		/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
	init_speciality();
}

void config_CS9929AX_CC(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/ | __CC;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;//
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ /**/;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;		/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
	init_speciality();
}

void config_CS9929AX_F(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;//ACW_10kV;//ACW_20kV;//
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ /**/;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;//DCW_10kV;//DCW_20kV;//
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;		/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 9999Mohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
	init_speciality();
}
void config_CS9929AX_T(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR /*| __GR | __BBD*/ | __CC;
    
    type_spe.port_num = PORT_NUM8;//PORT_NUM0;//PORT_NUM4;//PORT_NUM14;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;//
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10 * 1000.0;/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9929BX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM4;//PORT_NUM0;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922BX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
	type_spe.ir_vol_range = IR_1kV;
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000;	/* 10Gohm */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}

void config_CS9922LB(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.transformer_type = TRANS_750V;/* 变压器类型 */
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    type_spe.acw_vol_range = ACW_500V;// ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_vol_l = 10;	/* 10V */
    
    type_spe.dcw_vol_range = DCW_500V;// DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    type_spe.dcw_vol_l = 10;	/* 10V */
    
	type_spe.ir_vol_range = IR_750V;
	init_speciality();
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
}
void config_CS9922BXJ(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
	type_spe.ir_vol_range = IR_1kV;
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000;	/* 10Gohm */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}

void config_CS9922BX_1(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
	type_spe.ir_vol_range = IR_1_5kV;
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}

void config_CS9922BX_DZ(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
	type_spe.ir_vol_range = IR_1kV;
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000*10;	/* 10Gohm 带一个小数位 */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}

void config_CS9922CX(void)
{
    type_spe.mode_en =  __ACW /*| __DCW */| __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_60A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6000;	/* 60A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9933X(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
//深圳市西思特科技有限公司定制
void config_RH8400AS(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
	
	type_spe.acw_vol_h = 6500;	/* 6.500kV */
}
void config_CS9933X_100(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA */| _AC_100mA /*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9912AX(void)
{
	init_speciality();
}
void config_CS9912BX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9912BX_L(void)
{
    type_spe.mode_en =  __ACW | __DCW /*| __IR | __GR*/ | __BBD /*| __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9912BX_T(void)
{
    type_spe.mode_en = /* __ACW |*/ __DCW /*| __IR | __GR | __BBD | __CC*/;
    type_spe.transformer_point = DOUBLE_POINT;/* 双触点变压器 */
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.vol_segment_point = 200;/* 换挡分界点200V */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_2kV;// DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    type_spe.dcw_vol_l = 20;	/* 20V */
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9913AX(void)
{
    type_spe.mode_en = /**/ __ACW /*| __DCW | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9913BX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA /*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9950X(void)
{
	init_speciality();
}
void config_CS9950CX(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR |*/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA /*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_40A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9950DX(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR |*/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA /*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6200;	/* 62A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9950ED(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR |*/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA /*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 12000;	/* 120A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.one_dc_module_max_cur = 2000;//4000;/* 40A */
    type_spe.gr_max_cur_res_rang = 120;/* 50mΩ */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
/* CS9933G+ */
void config_CS9933G_(void)
{
    type_spe.mode_en =  __ACW | __DCW | __IR |/**/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA *//*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 100*1000.0;	/* 100Gohm */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 7000;	/* 70A */
    type_spe.gr_cur_l = 500;	/* 5A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.one_dc_module_max_cur = 2000;//4000;/* 40A */
    type_spe.gr_max_cur_res_rang = 110;/* 110mΩ */
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922DX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM4;//PORT_NUM8;//
    type_spe.hz_type = HZ_TYPE_CONT;//HZ_TYPE_GRADE;//
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA /*|_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000.0;	/* 10Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6200;	/* 62A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9922EX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;//
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10 * 1000;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}

void config_CS9922EXJ(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;//
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
   
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10 * 1000;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}
void config_CS9914AX(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
	init_speciality();
}
void config_CS9914D(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    type_spe.real_cur_format = CAP_FORMAT;/* 将真实电流转换成电容来显示 */
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
	init_speciality();
}
void config_CS9914AX_1(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_real = DISABLE;
    type_spe.acw_vol_range = ACW_500V;//ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    
    init_speciality();
    type_spe.acw_vol_h = 500;	/* 1KV */
    type_spe.acw_vol_l = 10;	/* 10V */
}

void config_CS9914BX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
	init_speciality();
}

void config_CS9914CX_C(void)
{
    type_spe.mode_en = /* __ACW | */__DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
	type_spe.dcw_big_cap_en = ENABLE;
    type_spe.dcw_output_impedan_opt_index = OUTPUT_IMPEDAN_OPT_2;/* 大电容测试输出阻抗选项 */
    
    init_speciality();
}
void config_CS9914DX(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA */| _AC_100mA /*| _AC_200mA */;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA */| _DC_20mA |_DC_50mA /* | _DC_100mA */;
	type_spe.dcw_big_cap_en = ENABLE;
    type_spe.dcw_output_impedan_opt_index = OUTPUT_IMPEDAN_OPT_2;/* 大电容测试输出阻抗选项 */
    
    init_speciality();
}
void config_CS9929CX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM8;//PORT_NUM0;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6200;	/* 62A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922FX(void)
{
    type_spe.mode_en = /**/ __ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA */| _AC_100mA /*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA |_DC_50mA  /*| _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 100;	/* 100V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_62A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 6200;	/* 62A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9921BX(void)
{
    type_spe.mode_en =  __ACW | __DCW /*| __IR */| __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9912Y(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA  | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9914Y(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA */| _AC_100mA /*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA |_DC_50mA  /*| _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922CX_1(void)
{
	init_speciality();
}
void config_CS9929EX(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM14;//PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9929EX_CC(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD*/ | __CC;
    
    type_spe.port_num = PORT_NUM14;//PORT_NUM0;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9919G(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM8;//PORT_NUM14;//PORT_NUM0;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA *//*|_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922G(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922G_1(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR/* | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA*/ | _AC_100mA /*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA | _DC_50mA /*| _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9922G_2(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /*| _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_1kV;//IR_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 2500;	/* 2.5KV */
    type_spe.ir_vol_l = 500;	/* 50V */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS2676CH(void)
{
    type_spe.mode_en =  /*__ACW | __DCW |*/ __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_1kV;//IR_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 2500;	/* 2.5KV */
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS2676CH_1(void)
{
    type_spe.mode_en =  /*__ACW | __DCW |*/ __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_5kV;//IR_2_5kV;//IR_1kV;//IR_10kV;//
    type_spe.ir_vol_l = 500; /* 500V */
    type_spe.ir_res_l = 5; /* 5Mohm */
    type_spe.ir_res_h = 50 * 1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9927LB(void)
{
    type_spe.mode_en =  __ACW /*| __DCW | __IR | __GR*/ | __BBD /*| __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.bbd_vol_h = 100;
    type_spe.bbd_vol_l = 100;/* 电压下限50V */
    
    type_spe.acw_vol_range = ACW_500V;//ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA *//*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_1kV;//IR_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 2500;	/* 2.5KV */
    type_spe.ir_vol_l = 500;	/* 50V */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	
    init_speciality();
}
void config_CS9933G(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR | __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz_type = HZ_TYPE_GRADE;
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9950CG(void)
{
    type_spe.mode_en =  /*__ACW | __DCW | __IR |*/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9950CG_1(void)
{
    type_spe.mode_en =  /*__ACW | __DCW | __IR |*/ __GR /*| __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9914G(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = _AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA*/ | _AC_100mA /*| _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA | _DC_50mA /*| _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9919A(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 无真实电流 */
    type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
	init_speciality();
}

void config_CS9919B(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM4;/* 测试端口数 0 4 8 */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 无真实电流 */
    type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
	init_speciality();
}

void config_CS9929A(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
	type_spe.ir_vol_range = IR_1kV;//IR_2_5kV;//IR_5kV;//
    type_spe.ir_vol_h = 1000; /* 1KV */
    type_spe.ir_vol_l = 250;  /* 250V */
    type_spe.ir_res_h = 5*1000.0;	/* 5Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    
    type_spe.interface_en = ENABLE;
	init_speciality();
}

void config_CS9929B(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM4;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5000;	/* 5000Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
	init_speciality();
}
void config_CS9922M(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear =  _AC_200uA | _AC_2mA /*|  _AC_10mA */| _AC_20mA /* | _AC_50mA | _AC_100mA */| _AC_200mA /**/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA /*| _DC_20mA |_DC_50mA*/ /* | _DC_100mA */;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 10*1000;	/* 10Gohm */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
	init_speciality();
}
void config_CS9915AX(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_10mA /*| _AC_20mA | _AC_50mA */;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
	init_speciality();
}
void config_CS9916AX(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    /* 真实电流 */
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA */;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
	init_speciality();
}
void config_CS9916BX(void)
{
    type_spe.mode_en = /* __ACW | */__DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    
    type_spe.dcw_vol_range =  /* DCW_6kV */ DCW_10kV /* DCW_20kV */;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA*/;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
	init_speciality();
}

void config_CS9917AX(void)
{
    type_spe.mode_en = /* */__ACW /*| __DCW | __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /**/;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
	init_speciality();
}
void config_CS9917A_8(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.ports_ctrl_type = CPLD_CTRL;
    
    
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA /**/;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
    type_spe.dcw_vol_range =  /* DCW_6kV */ DCW_10kV /* DCW_20kV */;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA*/;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
    
	init_speciality();
}
void config_CS9919D(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.ports_ctrl_type = CPLD_CTRL;
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /**/| _AC_10mA  /*| _AC_20mA | _AC_50mA*/;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
    type_spe.dcw_vol_range =  /* DCW_6kV */ DCW_10kV /* DCW_20kV */;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA | _DC_20mA*/;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
    
	init_speciality();
}

void config_CS9917BX(void)
{
    type_spe.mode_en = /* __ACW |*/ __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = DISABLE;/* 关闭G模式 */
    
    
    type_spe.acw_vol_range = ACW_10kV; //*ACW_5kV; // ACW_20kV; //
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA ;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
    type_spe.dcw_vol_range =  DCW_10kV; // DCW_6kV; //  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
    type_spe.dcw_big_cap_en = ENABLE;
    
	init_speciality();
}
void config_CS9920A(void)
{
    type_spe.mode_en =  /**/__ACW /*| __DCW | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_20kV;// ACW_5kV;// ACW_10kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**//**/_AC_200uA | _AC_2mA | _AC_10mA /*| _AC_20mA | _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9930A_T(void)
{
    type_spe.mode_en =  /**/__ACW /*| __DCW | __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_30kV;//ACW_20kV;// ACW_5kV;// ACW_10kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**//**/_AC_200uA | _AC_2mA | _AC_10mA /*| _AC_20mA | _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9920B(void)
{
    type_spe.mode_en =  /*__ACW | */__DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
// 	type_spe.rise_time_min_value = 3;/* 上升时间的最小值 */
    
    type_spe.acw_vol_range = ACW_20kV;// ACW_5kV;// ACW_10kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**//**/_AC_200uA | _AC_2mA | _AC_10mA /*| _AC_20mA | _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_20kV;// DCW_6kV;// DCW_10kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923CG(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR | __GR/* | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_vol_l = 500;	/* 500V */
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_res_l = 5;/* 5Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = ENABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 500;	/* 5A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923G_1(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.transformer_type = TRANS_10kV;// TRANS_1kV;// TRANS_2kV;//TRANS_5kV;//TRANS_20kV;/* 变压器类型 */
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_vol_l = 500;	/* 500V */
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_2_5kV;//IR_10kV;//IR_5kV;//IR_1kV;//
	init_speciality();
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
}
void config_CS9923G_2(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.transformer_type = TRANS_10kV;// TRANS_1kV;// TRANS_2kV;//TRANS_5kV;//TRANS_20kV;/* 变压器类型 */
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_5kV;//IR_10kV;//IR_1kV;//IR_2_5kV;//
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_l = 5;	/* 5Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}
void config_CS9923G_3(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.transformer_type = TRANS_10kV;// TRANS_1kV;// TRANS_2kV;//TRANS_5kV;//TRANS_20kV;/* 变压器类型 */
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_10kV;//IR_5kV;//IR_1kV;//IR_2_5kV;//
    type_spe.ir_vol_h = 10000 - 1;	/* 10.00kV */
    type_spe.ir_vol_l = 5000;	/* 5000V */
    type_spe.ir_res_l = 500;	/* 500Mohm */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9923_5(void)
{
    type_spe.mode_en =  /**/__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_10kV;//DCW_6kV;//  DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA/* | _DC_20mA |_DC_50mA | _DC_100mA */;
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9923A(void)
{
	init_speciality();
}
void config_CS9933EG(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD*/;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1kV */
	type_spe.ir_res_h = 50*1000;	/* 50G */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_decs = 2;
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 300;	/* 3A */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 0;/* GR压降模式 1打开 0关闭 */
	init_speciality();
}

void config_CS9933ET(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD*/;
    type_spe.port_num = PORT_NUM0;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1kV */
	type_spe.ir_res_h = 50*1000;	/* 50G */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_decs = 2;
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 300;	/* 3A */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
	type_spe.gr_max_cur_res_rang = 110;/* 110mΩ */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 0;/* GR压降模式 1打开 0关闭 */
    
    /* 通信开关 */
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9933ET_1(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD*/;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_1_5kV;
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000;	/* 50G */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_decs = 2;
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 300;	/* 3A */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
	type_spe.gr_max_cur_res_rang = 110;/* 110mΩ */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 0;/* GR压降模式 1打开 0关闭 */
	init_speciality();
}
void config_CS9933ET_2(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD*/;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_2_5kV;
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000;	/* 50G */
    type_spe.ir_res_l = 5;	/* 5M */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_decs = 2;
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 300;	/* 3A */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
	type_spe.gr_max_cur_res_rang = 110;/* 110mΩ */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 0;/* GR压降模式 1打开 0关闭 */
	init_speciality();
}
void config_CS9933ET_3(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR | __GR /*| __BBD*/;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_5kV;
    type_spe.ir_vol_l = 500;	/* 500V */
    type_spe.ir_res_h = 50*1000;	/* 50G */
    type_spe.ir_res_l = 5;	/* 5M */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.gr_cur_decs = 2;
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 300;	/* 3A */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.one_dc_module_max_cur = 4000;/* 40A */
	type_spe.gr_max_cur_res_rang = 110;/* 110mΩ */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 0;/* GR压降模式 1打开 0关闭 */
	init_speciality();
}
void config_CS9922H_50(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM4;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ | _AC_250HZ | _AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_range = IR_5kV;
    type_spe.ir_vol_h = 5000; /* 5kV */
    
    type_spe.gr_cur_h = 3000; /* 30A */
	init_speciality();
}
void config_CS9914CX(void)
{
    type_spe.mode_en =  /*__ACW |*/ __DCW /*| __IR | __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM0;//PORT_NUM14;//PORT_NUM8;//PORT_NUM4;//
    type_spe.hz_type = HZ_TYPE_CONT;
    
    type_spe.acw_vol_range = ACW_10kV;// ACW_20kV;// ACW_5kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA*/ | _AC_20mA /*| _AC_50mA | _AC_100mA | _AC_200mA*/;
    
    type_spe.dcw_vol_range = DCW_6kV;//DCW_10kV;//DCW_20kV;//
    type_spe.dcw_gear = _DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA |_DC_50mA/* | _DC_100mA */;
    type_spe.contact_segment_point = 6000;/* PWM功放切换变压器触头的电压点 */
    
    type_spe.ir_vol_range = IR_1kV;//IR_5kV;//IR_2_5kV;//IR_10kV;//
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 50*1000.0;	/* 50Gohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.gr_amp_type = GR_AMP_32A;
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.cc_gear = /*_AC_200uA | _AC_2mA*/ /*| _AC_10mA | _AC_20mA *//*| _AC_50mA | _AC_100mA |*/ _AC_200mA;
	init_speciality();
}

void config_CS9906B(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR | */__GR /*| __BBD*/;
    
    type_spe.gr_cur_h = 4000;	/* 40A */
    type_spe.gr_cur_l = 100;	/* 100mA */
    type_spe.gr_res_h = 5100;	/* 电阻上限510.0mohm */
    type_spe.gr_res_l = 0;	/* 压降仪电压上限 0mohm */
    type_spe.gr_cur_unit = 0;	/* GR输出电流的单位是A */
    
    type_spe.gr_cur_unit = CUR_U_mA;
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 1;/* GR压降模式 1打开 0关闭 */
    type_spe.gr_vol_gear = GR_DC_20mV | GR_DC_200mV | GR_DC_2000mV;
	init_speciality();
}
void config_CS9929A_8(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5*1000.0;	/* 5Gohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9929AX_8(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/ | __CC;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5*1000.0;	/* 5000Mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_VTC9929(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/ | __CC;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5*1000.0;	/* 5000Mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_TSK7630(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD | __CC*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 10 * 1000.0;	/* 10Gohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9919A_8(void)
{
    type_spe.mode_en = /* */__ACW | __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA |*/ _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA | */_DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5*1000.0;	/* 5000Mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 0;/* GR为直流模式 */
    type_spe.ir_short_int_en = 0;/* IR短路中断使能 */
    
    type_spe.interface_en = DISABLE;
    
	init_speciality();
}

void config_CS9917B(void)
{	init_speciality();
}
void config_CS9906B_K(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR | */__GR /*| __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_10kV;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_10kV;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 100*1000 - 1;	/* 100*1000Mohm */
    
    type_spe.gr_cur_h =	5000;	/* 500mA */
    type_spe.gr_cur_l = 1000;	/* 100mA */
    type_spe.gr_res_h = 1200;	/* 压降仪电压上限 12v */
    type_spe.gr_res_l = 0;	/* 压降仪电压上限 0v */
    
    /* GR 电流单位 mA 1位小数 */
    type_spe.gr_cur_unit = CUR_U_mA;
    type_spe.gr_cur_decs = POINT_1/*POINT_0  */;
    
    /* GR 电阻单位 OHM 小数位不确定 */
    type_spe.gr_res_unit = RES_U_OHM/* GR_RES_U_MOHM */;
    
    /* GR 电压点位 V 2位小数 */
    type_spe.gr_vol_unit = VOL_U_V/* GR_VOL_U_MV */;
    type_spe.gr_vol_decs = POINT_2;
    
// 			type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 1;/* GR压降模式 1打开 0关闭 */
// 			type_spe.gr_vol_pulse_mode_en = 1;/* gr压降测试模式 1脉冲 0连续 */
    type_spe.gr_vol_gear = GR_DC_12V;/* 该型号只有一个电压档位 12v */
    
    type_spe.offset_fun_en = OFFSET_OFF;
    
    /* 通信开关 */
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9906A(void)
{
    type_spe.mode_en = /* __ACW | __DCW | __IR | */__GR /*| __BBD*/;
    
    type_spe.port_num = PORT_NUM8;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_10kV;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_10kV;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 50;	/* 50V */
    type_spe.ir_res_h = 100*1000 - 1;	/* 100*1000Mohm */
    
    type_spe.gr_cur_h =	10000-1;	/* 1000mA */
    type_spe.gr_cur_l = 100;	/* 10mA */
    type_spe.gr_res_h = 1200;	/* 压降仪电压上限 12v */
    type_spe.gr_res_l = 0;	/* 压降仪电压上限 0v */
    
    /* GR 电流单位 mA 1位小数 */
    type_spe.gr_cur_unit = CUR_U_mA;
    type_spe.gr_cur_decs = POINT_1/* POINT_0  */;
    
    type_spe.gr_dis_opt = GR_DIS_VOL /*| GR_DIS_RES*/;
    
    /* GR 电阻单位 OHM 小数位不确定 */
    type_spe.gr_res_unit = RES_U_OHM/* GR_RES_U_MOHM */;
    
    /* GR 电压点位 V 2位小数 */
    type_spe.gr_vol_unit = VOL_U_V/* GR_VOL_U_MV */;
    type_spe.gr_vol_decs = POINT_2;
    
// 			type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = 1;/* GR为直流模式 */
    type_spe.gr_vol_drop_mode = 1;/* GR压降模式 1打开 0关闭 */
    type_spe.gr_cur_divide_gear_en = 1;/* GR压降模式 电流分为两个档位 100mA 1000mA */
// 			type_spe.gr_vol_pulse_mode_en = 1;/* gr压降测试模式 1脉冲 0连续 */
    type_spe.gr_vol_gear = GR_DC_12V;/* 该型号只有一个电压档位 12v */
    
    type_spe.offset_fun_en = OFFSET_OFF;
    
    /* 通信开关 */
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9929A_4(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM4;/* 测试端口数 0 4 8 */
    type_spe.hz_type = HZ_TYPE_GRADE;
    
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.acw_real = DISABLE;/* 真实电流开关 */
    type_spe.acw_gear = /*_AC_200uA | */_AC_2mA | _AC_20mA;
    type_spe.dcw_gear = /*_DC_2uA | _DC_20uA | _DC_200uA |*/ _DC_2mA | _DC_10mA;
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;// 
    
    type_spe.ir_vol_h = 1000;	/* 1KV */
    type_spe.ir_vol_l = 250;	/* 250V */
    type_spe.ir_res_h = 5000;	/* 5000Mohm */
    type_spe.gr_cur_h = 3200;	/* 32A */
    
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
	init_speciality();
}
void config_CS9917CX_20(void)
{
    type_spe.mode_en = /* __ACW |*/ __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = DISABLE;/* 关闭G模式 */
    
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA ;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
    type_spe.dcw_vol_range =  /* DCW_6kV */ DCW_10kV /* DCW_20kV */;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
    type_spe.dcw_big_cap_en = ENABLE;
    
    type_spe.ir_vol_h = 1000;   /* 1KV */
    type_spe.ir_vol_l = 50;     /* 50V */
    type_spe.ir_res_h = 100*1000-1;   /* 100Gohm */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9917CX_C(void)
{
    type_spe.mode_en = /* __ACW |*/ __DCW /*| __IR | __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = DISABLE;/* 关闭G模式 */
    
    type_spe.acw_vol_range = /*ACW_5kV */ ACW_10kV /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA | _AC_50mA ;
    type_spe.acw_vol_l = 500;/* 电压下限500v */
    
    type_spe.dcw_vol_range =  /* DCW_6kV */ DCW_10kV /* DCW_20kV */;
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA /*| _DC_10mA*/ | _DC_20mA;
    type_spe.dcw_vol_l = 500;/* 电压下限500v */
    type_spe.dcw_big_cap_en = ENABLE;
    
    type_spe.ir_vol_h = 1000;   /* 1KV */
    type_spe.ir_vol_l = 50;     /* 50V */
    type_spe.ir_res_h = 100*1000-1;   /* 100Gohm */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}
void config_CS9924_200(void)
{
    type_spe.mode_en = /**/ __ACW/* | __DCW | __IR | __GR | __BBD*/;
    type_spe.transformer_point = DOUBLE_POINT;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = ENABLE;/* 关闭G模式 */
    
    type_spe.acw_vol_range = ACW_1kV /*ACW_5kV  ACW_10kV*/ /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    /* 无真实电流 */
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA */| _AC_200mA;
    type_spe.acw_vol_l = 50;/* 电压下限50v */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}

void config_CS9924(void)
{
    type_spe.mode_en = /**/ __ACW/* | __DCW | __IR | __GR | __BBD*/;
    type_spe.transformer_point = DOUBLE_POINT;
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = ENABLE;/* 关闭G模式 */
    
    type_spe.acw_vol_range = ACW_1kV /*ACW_5kV  ACW_10kV*/ /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    /* 无真实电流 */
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA */| _AC_200mA;
    type_spe.acw_vol_l = 50;/* 电压下限50v */
    
    type_spe.interface_en = ENABLE;
    
	init_speciality();
}

void config_CS9914AM_H(void)
{
    type_spe.mode_en = /**/ __ACW/* | __DCW | __IR | __GR | __BBD | __CC*/;
    type_spe.transformer_point = DOUBLE_POINT;/* 双触点变压器 */
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    type_spe.g_mode_en = ENABLE;/* 关闭G模式 */
    
    type_spe.acw_vol_range = ACW_1kV /*ACW_5kV  ACW_10kV*/ /*ACW_20kV */;
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ | _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ/* */;
    /* 无真实电流 */
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA /*| _AC_10mA */| _AC_20mA /*| _AC_50mA | _AC_100mA */| _AC_200mA;
    type_spe.acw_vol_l = 50;/* 电压下限50v */
    
    type_spe.interface_en = ENABLE;

	init_speciality();
}


void config_default(void)
{
    type_spe.mode_en = /* */__ACW | __DCW | __IR /*| __GR | __BBD*/;
    
    type_spe.port_num = PORT_NUM0;/* 测试端口数 0 4 8 */
    
    type_spe.acw_vol_range = ACW_5kV;// ACW_10kV;// ACW_20kV;// 
    type_spe.acw_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    /* 真实电流开关 */
    type_spe.acw_gear = /**/_AC_200uA | _AC_2mA | _AC_20mA;
    
    type_spe.dcw_vol_range = DCW_6kV;// DCW_10kV;// DCW_20kV;//
    type_spe.dcw_gear = /**/_DC_2uA | _DC_20uA | _DC_200uA | _DC_2mA | _DC_10mA;
    
    type_spe.ir_vol_h = 1000; /* 1KV */
    type_spe.ir_vol_l = 50;	  /* 50V */
    type_spe.ir_res_h = 100*1000-1; /* 100*1000Mohm */
    type_spe.ir_short_int_en = DISABLE;/* IR短路中断使能 */
    
    type_spe.gr_cur_h = 3200; /* 32A */
    type_spe.gr_hz = _AC_50HZ | _AC_60HZ /*| _AC_100HZ | _AC_150HZ | _AC_200HZ| _AC_250HZ |_AC_300HZ |  _AC_350HZ | _AC_400HZ */;
    type_spe.gr_dc_en = DISABLE;/* GR为直流模式 */
	init_speciality();
}

#endif //__CS99XX_TYPE_CONFIG_H__
