
#ifndef __TIMER5_CONFIG_H__
#define __TIMER5_CONFIG_H__


#ifndef NULL
#define NULL 0
#endif

extern void init_timer5_callback(void(*fun)(void));
extern void TIM5_it_init(u16 arr,u16 psc);

#endif //__TIMER5_CONFIG_H__

