#include "stm32f4xx.h"
#include "led.h"

void led_init(void)
{
    // Configure PB9 (LED 4G) as output
    GPIOB->MODER &= ~(3U << (9 * 2));
    GPIOB->MODER |=  (1U << (9 * 2));
    GPIOB->OTYPER &= ~(1U << 9);
    GPIOB->OSPEEDR |= (3U << (9 * 2));
    GPIOB->PUPDR &= ~(3U << (9 * 2));
    GPIOB->BSRRH = (1U << 9);

    // Configure PB4 (LED GREEN NETWORK) as output
    GPIOB->MODER &= ~(3U << (4 * 2));
    GPIOB->MODER |=  (1U << (4 * 2));
    GPIOB->OTYPER &= ~(1U << 4);
    GPIOB->OSPEEDR |= (3U << (4 * 2));
    GPIOB->PUPDR &= ~(3U << (4 * 2));
    GPIOB->BSRRH = (1U << 4);

    // Configure PB5 (LED BLUE STATEMACHINE) as output
    GPIOB->MODER &= ~(3U << (5 * 2));
    GPIOB->MODER |=  (1U << (5 * 2));
    GPIOB->OTYPER &= ~(1U << 5);
    GPIOB->OSPEEDR |= (3U << (5 * 2));
    GPIOB->PUPDR &= ~(3U << (5 * 2));
    GPIOB->BSRRH = (1U << 5);
}

void led_4g_on(void) {
    GPIOB->BSRRL = (1U << 9);
}

void led_4g_off(void) {
    GPIOB->BSRRH = (1U << 9);
}

void led_4g_toggle(uint8_t value) {
    if (value)
        led_4g_on();
    else
        led_4g_off();
}

void led_green_network_on(void) {
    GPIOB->BSRRL = (1U << 4);
}

void led_green_network_off(void) {
    GPIOB->BSRRH = (1U << 4);
}

void led_green_network_toggle(uint8_t value) {
    if (value)
        led_green_network_on();
    else
        led_green_network_off();
}

void led_blue_statemachine_on(void) {
    GPIOB->BSRRL = (1U << 5);
}

void led_blue_statemachine_off(void) {
    GPIOB->BSRRH = (1U << 5);
}

void led_blue_statemachine_toggle(uint8_t value) {
    if (value)
        led_blue_statemachine_on();
    else
        led_blue_statemachine_off();
}