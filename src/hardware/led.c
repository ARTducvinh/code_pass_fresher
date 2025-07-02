#include "stm32f4xx.h"
#include "led.h"
#include "hardware.h"

void led_init(void)
{
    // Configure LED 4G (Green Network) as output
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = LED_4G_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_4G_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LED_STT_PIN;
    GPIO_Init(LED_STT_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = LED_STT_SWITCH_PIN;
    GPIO_Init(LED_STT_SWITCH_PORT, &GPIO_InitStruct);
}

void led_4g_on(void) {
    GPIO_SetBits(LED_4G_PORT, LED_4G_PIN);
}

void led_4g_off(void) {
    GPIO_ResetBits(LED_4G_PORT, LED_4G_PIN);
}

void led_4g_toggle(uint8_t value) {
    if (value)
        led_4g_on();
    else
        led_4g_off();
}

void led_stt_on(void) {
    GPIO_SetBits(LED_STT_PORT, LED_STT_PIN);
}

void led_stt_off(void) {
    GPIO_ResetBits(LED_STT_PORT, LED_STT_PIN);
}

void led_stt_toggle(uint8_t value) {
    if (value)
        led_stt_on();
    else
        led_stt_off();
}

void led_stt_switch_on(void) {
    GPIO_SetBits(LED_STT_SWITCH_PORT, LED_STT_SWITCH_PIN);
}

void led_stt_switch_off(void) {
    GPIO_ResetBits(LED_STT_SWITCH_PORT, LED_STT_SWITCH_PIN);
}
