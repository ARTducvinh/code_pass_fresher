#include "stm32f4xx.h"
#include "led.h"
#include "timer.h"

volatile uint32_t button_press_time = 0;
volatile uint8_t button_holding = 0;
volatile uint8_t led_4g_mode = 0; // 0: toggle, 1: blink 200/200, 2: blink 200/500/200/2000
volatile uint8_t led_4g_state = 0;

void button_init(void)
{
    // Bật clock cho GPIOA và SYSCFG
    RCC->AHB1ENR |= (1U << 0);
    RCC->APB2ENR |= (1U << 14);
    GPIOA->MODER &= ~(3U << (0 * 2));
    GPIOA->PUPDR &= ~(3U << (0 * 2));
    GPIOA->PUPDR |=  (1U << (0 * 2));
    SYSCFG->EXTICR[0] &= ~(0xF << 0);
    EXTI->IMR |= (1U << 0);
    EXTI->FTSR |= (1U << 0);
    EXTI->RTSR &= ~(1U << 0);
    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << 0)) {
        if ((GPIOA->IDR & (1U << 0)) == 0) {
            button_press_time = timer2_get_tick();
            button_holding = 1;
        } else {
            if (button_holding) {
                uint32_t duration_ms = timer2_get_tick() - button_press_time;
                if (duration_ms < 5000) {
                    led_4g_state = !led_4g_state;
                    led_4g_toggle(led_4g_state);
                    led_4g_mode = 0;
                } else if (duration_ms < 10000) {
                    led_4g_mode = 1;
                } else {
                    led_4g_mode = 2;
                }
                button_holding = 0;
            }
        }
        EXTI->PR = (1U << 0); 
    }
}