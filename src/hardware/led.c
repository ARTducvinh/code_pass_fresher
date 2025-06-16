#include "stm32f4xx.h"
#include "led.h"

void led_init(void)
{
    // Configure PB4 as output
    GPIOB->MODER &= ~(3U << (4 * 2));
    GPIOB->MODER |=  (1U << (4 * 2));

    GPIOB->OTYPER &= ~(1U << 4);
    GPIOB->OSPEEDR |= (3U << (4 * 2));
    GPIOB->PUPDR &= ~(3U << (4 * 2));
    GPIOB->BSRRH = (1U << 4);
}

void led_4g_on(void) {
    GPIOB->BSRRL = (1U << 4);
}

void led_4g_off(void) {
    GPIOB->BSRRH = (1U << 4);
}

void led_4g_toggle(uint8_t value) {
    if (value)
        led_4g_on();
    else
        led_4g_off();
}