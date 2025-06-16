#include "middle/led_logic.h"
#include "hardware/led.h"
#include "hardware/timer.h"

void led_logic_blink_200_200(void) {
    static uint32_t last_tick = 0;
    static uint8_t led_state = 0;
    uint32_t now = timer2_get_tick();

    if (now < last_tick) last_tick = now;

    if (led_state) {
        if (now - last_tick >= 200) {
            led_4g_off();
            led_state = 0;
            last_tick = now;
        }
    } else {
        if (now - last_tick >= 200) {
            led_4g_on();
            led_state = 1;
            last_tick = now;
        }
    }
}

void led_logic_blink_200_500_200_2000(void) {
    static uint32_t last_tick = 0;
    static uint8_t phase = 0;
    uint32_t now = timer2_get_tick();

    if (now < last_tick) {
        last_tick = now;
        phase = 0;
    }

    switch (phase) {
        case 0:
            led_4g_on();
            if (now - last_tick >= 200) {
                last_tick = now;
                phase = 1;
            }
            break;
        case 1:
            led_4g_off();
            if (now - last_tick >= 500) {
                last_tick = now;
                phase = 2;
            }
            break;
        case 2:
            led_4g_on();
            if (now - last_tick >= 200) {
                last_tick = now;
                phase = 3;
            }
            break;
        case 3:
            led_4g_off();
            if (now - last_tick >= 2000) {
                last_tick = now;
                phase = 0;
            }
            break;
    }
}