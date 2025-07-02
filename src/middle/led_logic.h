#ifndef LED_LOGIC_H
#define LED_LOGIC_H

#include <stdint.h>
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void led_4g_blink_200_200(void);
void led_4g_blink_200_500_200_2000(void);

void led_stt_blink_200_200(void);
void led_stt_blink_200_500_200_2000(void);

void led_update_by_device_state(void);

uint8_t led_stt_switch_read_state(void);  
//void led_stt_switch_set_state(uint8_t state);
void led_stt_switch_control(void);

#ifdef __cplusplus
}
#endif

#endif /* LED_LOGIC_H */