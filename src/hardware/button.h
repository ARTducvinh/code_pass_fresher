#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint32_t button_press_time;
extern volatile uint8_t button_holding;
extern volatile uint8_t led_4g_mode;
extern volatile uint8_t led_4g_state;

void button_init(void);

#ifdef __cplusplus
}
#endif

#endif