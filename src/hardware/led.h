#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void led_init(void);
void led_4g_toggle(uint8_t value);
void led_4g_blink_200_200(void);
void led_4g_blink_200_500_200_2000(void);
void led_4g_on(void);
void led_4g_off(void);

#ifdef __cplusplus
}
#endif

#endif