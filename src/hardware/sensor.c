#include "sensor.h"
#include "hardware.h"
#include "stm32f4xx.h"

void sensor_init(void) {
    SENSOR_PORT->MODER &= ~(3U << (4 * 2)); 
    SENSOR_PORT->PUPDR &= ~(3U << (4 * 2));
    SENSOR_PORT->PUPDR |=  (1U << (4 * 2)); 

    // PA4 (ADC1_IN4) analog mode
    // SENSOR_PORT->MODER |= (3U << (4 * 2)); 
    // SENSOR_PORT->PUPDR &= ~(3U << (4 * 2)); 
    // ADC1->CR2 = 0; 
    // ADC1->SQR3 = 4; 
    // ADC1->SMPR2 |= (7U << (3 * 4)); 
    // ADC1->CR2 |= ADC_CR2_ADON; 

    //ngắt nếu dùng
    // NVIC_SetPriority(EXTI4_IRQn, 2);
    // NVIC_EnableIRQ(EXTI4_IRQn);
}

uint8_t sensor_read(void) {
    return (SENSOR_PORT->IDR & SENSOR_PIN) ? 1 : 0;
}

// code cho analog sensor
// uint16_t sensor_read(void) {
//     ADC1->CR2 |= ADC_CR2_SWSTART; 
//     while (!(ADC1->SR & ADC_SR_EOC)); 
//     return (uint16_t)ADC1->DR; 
// }

// Hàm callback chưa có
__weak void sensor_exti_callback(void) {
}

// Trình phục vụ ngắt EXTI4 đơn giản
void EXTI4_IRQHandler(void) {
    if (EXTI->PR & (1U << 4)) {
        EXTI->PR = (1U << 4);
        sensor_exti_callback();
    }
}