#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"

void led_init(void)
{
    RCC->AHB1ENR |= (1U << 1); // Enable GPIOB clock

    // Configure PB4 as output
    GPIOB->MODER &= ~(3U << (4 * 2));
    GPIOB->MODER |=  (1U << (4 * 2));

    GPIOB->OTYPER &= ~(1U << 4);
    GPIOB->OSPEEDR |= (3U << (4 * 2));
    GPIOB->PUPDR &= ~(3U << (4 * 2));
    
}

void led_4g_on(void) {
    GPIOB->BSRRL = (1U << 4); // Reset PB4 (LED ON nếu LED cực âm về GND)
}

void led_4g_off(void) {
    GPIOB->BSRRH = (1U << 4); // Set PB4 (LED OFF)
}

// Toggle PB4
void led_4g_toggle(uint8_t value) {
    if (value)
        led_4g_on();
    else
        led_4g_off();
}

// Blink PB4: 200ms ON, 200ms OFF
void led_4g_blink_200_200(void) {
    static uint32_t last_tick = 0;
    static uint8_t led_state = 0;
    uint32_t now = timer2_get_tick();

    if (now < last_tick) last_tick = now; // chống tràn

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

// Blink PB4: 200ms ON, 500ms OFF, 200ms ON, 2000ms OFF
void led_4g_blink_200_500_200_2000(void) {
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