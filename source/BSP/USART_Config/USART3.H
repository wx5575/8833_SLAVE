

#ifndef __usart3_H__
#define __usart3_H__

#include "stm32f10x.h"


extern void usart3_close(void);
extern void usart3_clear_receive_count(void);
extern uint32_t usart3_sub_timeout(void);
extern uint32_t usart3_get_timeout(void);
extern void usart3_judge_timout(void);
extern void usart3_reset_timeout(void);
extern uint8_t * usart3_get_send_buf(void);
extern void usart3_clear_receive_data_count(void);
extern uint32_t usart3_get_receive_data_count(void);
extern uint8_t* usart3_get_receive_data(void);
extern uint8_t usart3_get_send_status(void);
extern void usart3_send_data(uint8_t *data, uint32_t len);
extern void usart3_resend_data(void);
extern void usart3_config(uint32_t baud_rate);
extern uint8_t get_usart3_receive_over_flag(void);
extern void clear_usart3_receive_over_flag(void);



#endif //__usart3_H__



