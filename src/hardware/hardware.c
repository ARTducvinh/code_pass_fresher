#include "hardware.h"
#include "led.h"
#include "button.h"
#include "uart.h"
#include "timer.h"
#include "stm32f4xx.h"
#include "uart_line_queue.h"

#define GSM_POWER_GPIO_PORT GPIOA
#define GSM_POWER_PIN       8
void hardware_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_SYSCFGEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    timer2_init_1ms();
    uart_line_queue_init();
    uart_init_all();
    led_init();
    button_init();
    gsm_module_power_cycle();
}
uint32_t sys_get_tick_ms(void) {
    return 0; 
}

uint32_t button_read(uint32_t pin) {
    return 0; 
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
    return 0; 
}

void gsm_module_power_cycle(void) {
    GSM_POWER_GPIO_PORT->MODER &= ~(3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->MODER |=  (1U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->OTYPER &= ~(1U << GSM_POWER_PIN);
    GSM_POWER_GPIO_PORT->OSPEEDR |= (3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->PUPDR &= ~(3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->ODR &= ~(1U << GSM_POWER_PIN);
    delay_ms(2000); 
    GSM_POWER_GPIO_PORT->ODR |= (1U << GSM_POWER_PIN); 
}
