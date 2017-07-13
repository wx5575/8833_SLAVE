
#ifndef __MB_DC_MODULE_H__
#define __MB_DC_MODULE_H__



extern void m_dc_module_cal_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_dac_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_start_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_stop_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_test_fetch_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_test_fetch_short_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_vol_gear_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_test_method_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_info_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);
extern void m_dc_module_set_data_dispose(uint8_t slove_num, uint16_t cmd, uint16_t *data);



#endif //__MB_DC_MODULE_H__
