#include "led.h"
#include "timer.h"
#include "button.h"

extern volatile uint8_t led_4g_mode;

int main(void)
{
    timer2_init_1ms();
    led_init();
    button_init();

    while (1)
    {
        if (led_4g_mode == 0) {
            // Chế độ toggle: không làm gì, LED điều khiển trực tiếp trong ngắt nút bấm
        } else if (led_4g_mode == 1) {
            led_4g_blink_200_200();
        } else if (led_4g_mode == 2) {
            led_4g_blink_200_500_200_2000();
        }
    }
}