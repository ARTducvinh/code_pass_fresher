#include "button.h"

void button_init(void)
{
    // Cấu hình input pull-up
    uint8_t pin = 0;
    for (uint8_t i = 0; i < 16; ++i) {
        if (BUTTON_PIN & (1U << i)) { pin = i; break; }
    }
    BUTTON_PORT->MODER &= ~(3U << (pin * 2));
    BUTTON_PORT->PUPDR &= ~(3U << (pin * 2));
    BUTTON_PORT->PUPDR |=  (1U << (pin * 2)); // Pull-up
}

uint8_t button_read_pin(void)
{
    return GPIO_ReadInputDataBit(BUTTON_PORT, BUTTON_PIN);
}