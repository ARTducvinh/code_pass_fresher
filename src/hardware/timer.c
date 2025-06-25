#include "timer.h"
#include "stm32f4xx.h"

static volatile uint32_t timer2_tick = 0;

void timer2_init_1ms(void)
{
    TIM2->PSC = 15;      // 16MHz / (15+1) = 1MHz
    TIM2->ARR = 999;     // 1MHz / (999+1) = 1kHz (1ms)
    TIM2->EGR = TIM_EGR_UG;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) // Update interrupt flag
    {
        TIM2->SR &= ~TIM_SR_UIF; // Clear flag
        timer2_tick++;
    }
}

uint32_t timer2_get_tick(void)
{
    return timer2_tick;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = timer2_get_tick();
    while ((timer2_get_tick() - start) < ms) {
    }
}
