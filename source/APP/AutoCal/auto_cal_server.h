
#ifndef __AUTO_CAL_SERVER_H__
#define __AUTO_CAL_SERVER_H__



typedef struct{
    ELEMENT_INFO auto_cal_ui_ele[12];
    uint8_t ele_str[12][10];
}AUTO_ELE_INFO;


enum{
    UI_ELE_NUM,
    UI_ELE_MODE,
    UI_ELE_OPT,
    UI_ELE_DA,
    UI_ELE_AD_VOL,
    UI_ELE_AD_CUR,
    UI_ELE_GEAR,
    UI_ELE_POINT,
    UI_ELE_VOL,
    UI_ELE_CUR,
    UI_ELE_RES,
    UI_ELE_MEASURE,
    UI_ELE_END,
};

#ifdef   AUTO_CAL_SERVER_GLOBALS
#define  AUTO_CAL_SERVER_EXT
#else
#define  AUTO_CAL_SERVER_EXT  extern
#endif

AUTO_CAL_SERVER_EXT AUTO_ELE_INFO *g_auto_ele_info;

extern void get_res_cal_point_info(uint8_t mode, CAL_BASE_INFO* point, uint8_t n);
extern void get_vol_cal_point_info(uint8_t mode, CAL_BASE_INFO* point, uint8_t n);
extern void init_ui_ele_info(ELEMENT_INFO *ele, uint8_t buf[12][10]);
//extern void draw_auto_cal_ui_ele(const GUI_FONT * pNewFont, ELEMENT_INFO* p);
extern void transform_auto_cal_num_to_string(uint8_t *buf, uint16_t num);
extern void transform_auto_cal_da_ad_to_string(uint8_t *buf, uint16_t value);
extern void transform_auto_cal_gear_to_string(uint8_t *buf, uint16_t value);
extern void get_cur_cal_point_info(uint8_t mode, AUTO_CAL_CUR* point, uint8_t gear);
extern void transform_auto_cal_res_to_string(uint8_t *buf, uint64_t value, uint8_t mode, uint8_t gear);
extern void transform_auto_cal_cur_to_string(uint8_t *buf, uint16_t value, uint8_t mode, uint8_t gear);
extern void transform_auto_cal_vol_to_string(uint8_t *buf, uint16_t value);
extern void clear_ui_ele(void);
extern void dis_cal_point_base_info(void);
extern void dis_cal_ele(uint16_t ad_v, uint16_t ad_i, uint16_t vol);
extern void dis_gr_measure(float vol);

#endif //__AUTO_CAL_SERVER_H__
