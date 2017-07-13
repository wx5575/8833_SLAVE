
#ifndef __DC_MODULE_BRIDGE_H__
#define __DC_MODULE_BRIDGE_H__


extern void set_cal_data_dc_module_bridge(uint8_t slave_addr);
extern void get_cal_data_dc_module_bridge(uint8_t slave_addr);
extern void set_dac_value_dc_module_bridge(uint8_t slave_addr, uint16_t da_value);
extern void set_start_dc_module_bridge(uint8_t slave_addr);
extern void set_stop_dc_module_bridge(uint8_t slave_addr);
extern void get_test_fetch_dc_module_bridge(uint8_t slave_addr);
extern void get_test_fetch_short_dc_module_bridge(uint8_t slave_addr);
extern void set_vol_gear_dc_module_bridge(uint8_t slave_addr, uint16_t gear);
extern void set_test_method_dc_module_bridge(uint8_t slave_addr, uint16_t gear);
extern void get_info_dc_module_bridge(uint8_t slave_addr);
extern void set_dc_gr_module_output_da(uint8_t slave_addr, uint16_t vol);
extern void set_data_dc_module_bridge(uint8_t slave_addr);

#endif //__DC_MODULE_BRIDGE_H__

