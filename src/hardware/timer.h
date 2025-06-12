#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

void timer2_init_1ms(void);
uint32_t timer2_get_tick(void);

#ifdef __cplusplus
}
#endif

#endif