
#ifndef __TIMER5_CONFIG_H__
#define __TIMER5_CONFIG_H__


#ifndef NULL
#define NULL 0
#endif

typedef void (*TIM_SERVER_FUN)(void);
extern void init_timer5_callback(void(*fun)(void));
extern void TIM5_it_init(u16 arr,u16 psc);
extern uint8_t register_tim5_server_fun(TIM_SERVER_FUN fun);

#endif //__TIMER5_CONFIG_H__

