#include "middle/button_logic.h"
#include "hardware/button.h"
#include "hardware/timer.h"
#include "hardware/led.h"
#include "hardware/uart.h"

#define DEBOUNCE_TIME     50      // ms
#define SHORT_PRESS_MAX   5000    // < 5s
#define LONG_PRESS_1      5000    // 5-10s
#define LONG_PRESS_2      10000   // > 10s

volatile uint8_t led_4g_mode = 0;
volatile uint8_t led_4g_state = 0;

static uint32_t press_time = 0;
static uint32_t release_time = 0;
static uint8_t button_handled = 0;
static uint8_t last_state = 1;
static uint32_t last_debounce_tick = 0;

void button_logic_init(void) {
    button_init();
}

void button_logic_update(void) {
    uint8_t state = button_read_pin();
    uint32_t now = timer2_get_tick();

    if (state != last_state) {
        last_debounce_tick = now;
        last_state = state;
    }

    if ((now - last_debounce_tick) > DEBOUNCE_TIME) {
        if (state == 0 && !button_handled) { // Nhấn
            press_time = now;
            button_handled = 1;
        }
        if (state == 1 && button_handled) { // Nhả
            release_time = now;
            uint32_t hold_time = release_time - press_time;
            if (hold_time < SHORT_PRESS_MAX) {
                led_4g_state ^= 1;
                led_4g_toggle(led_4g_state);
                led_4g_mode = 0;
                uart_log("Button: Short press, mode 0");
            }
            button_handled = 0;
        }
    }
}

void button_logic_process(void) {
    if (last_state == 0 && button_handled) { // Đang giữ nút
        uint32_t now = timer2_get_tick();
        uint32_t hold_time = now - press_time;
        if (hold_time >= LONG_PRESS_2 && led_4g_mode != 2) {
            led_4g_mode = 2;
            uart_log("Button: Long press >10s, mode 2");
        } else if (hold_time >= LONG_PRESS_1 && hold_time < LONG_PRESS_2 && led_4g_mode != 1) {
            led_4g_mode = 1;
            uart_log("Button: Long press 5-10s, mode 1");
        }
    }
}