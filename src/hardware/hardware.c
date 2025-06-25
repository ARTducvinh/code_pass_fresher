#include "hardware.h"
#include "led.h"
#include "button.h"
#include "uart.h"
#include "timer.h"
#include "stm32f4xx.h"

void hardware_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; // <-- Bật clock cho DMA2

    timer2_init_1ms();
    led_init();
    button_init();
}
uint32_t sys_get_tick_ms(void) {
}

uint32_t button_read(uint32_t pin) {
}

void button_config(uint32_t pin) {
}

void hw_led_config(uint8_t pin) {
}

void hw_led_write(uint8_t pin, uint8_t state) {
}

void switch_init(void) {
}

uint8_t switch_read(uint8_t index) {
}
