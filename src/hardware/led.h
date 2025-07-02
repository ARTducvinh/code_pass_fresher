#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void led_init(void);
void led_4g_on(void);
void led_4g_off(void);
void led_4g_toggle(uint8_t value);

void led_stt_on(void);
void led_stt_off(void);
void led_stt_toggle(uint8_t value);

void led_stt_switch_on(void);
void led_stt_switch_off(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H__ */