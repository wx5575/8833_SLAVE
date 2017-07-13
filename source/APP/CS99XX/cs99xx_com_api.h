#ifndef CS99XX_COM_API_H__
#define CS99XX_COM_API_H__


extern uint16_t get_used_res_num(void);
extern uint16_t get_pass_res_num(void);
extern uint16_t get_res_string_size(void);

extern uint32_t get_step_used_flag_size(void);
extern uint32_t get_group_table_size(void);
extern uint32_t get_group_total_step(uint16_t note_num);
extern void change_result_to_string(const RESULT_STR *res, RESULT_STRING_STR* res_str, uint16_t n);

#endif //CS99XX_COM_API_H__
