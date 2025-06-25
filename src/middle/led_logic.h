#ifndef __LED_LOGIC_H__
#define __LED_LOGIC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LED_4G_OFF = 0,
    LED_4G_ON,
    LED_4G_BLINK_200_200,
    LED_4G_BLINK_200_500_200_2000
} led_4g_mode_t;

extern volatile led_4g_mode_t led_4g_mode;

void led_logic_blink_200_200(void);
void led_logic_blink_200_500_200_2000(void);
void led_4g_update_logic(void);

#ifdef __cplusplus
}
#endif

#endif