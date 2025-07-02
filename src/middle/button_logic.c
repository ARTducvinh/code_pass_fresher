#include "button_logic.h"
#include "hardware/timer.h"
#include "uart.h"
#include "hardware/button.h"
#include "main.h"
#include "mqtt_response_handler.h"

typedef enum {
    BUTTON_IDLE,
    BUTTON_PRESSED_WAIT_RELEASE
} button_state_t;

static button_state_t button_state = BUTTON_IDLE;
static uint32_t button_press_time = 0;

static void toggle_switch_state(void) {
    if (switch_state == swich_on) {
        switch_state = swich_off;
    } else {
        switch_state = swich_on;
    }
}

void button_logic_update(void)
{
    uint32_t now = timer2_get_tick();

    switch (button_state)
    {
        case BUTTON_IDLE:
            if (g_button_pressed_flag)
            {
                g_button_pressed_flag = 0;
                button_press_time = now;
                button_state = BUTTON_PRESSED_WAIT_RELEASE;
            }
            break;

        case BUTTON_PRESSED_WAIT_RELEASE:
            if (g_button_released_flag)
            {
                g_button_released_flag = 0;
                uint32_t duration = now - button_press_time;

                if (duration < 5000) {
                    toggle_switch_state();
                    mqtt_publish_switch_state();
                }
                else if (duration < 10000) {
                    uart_log("5<s < 10s");
                }
                else {
                    uart_log("10s");
                }

                button_state = BUTTON_IDLE;
            }
            break;
    }
}

