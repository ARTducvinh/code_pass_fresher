#include "uart.h"
#include "hardware.h"
#include "stm32f4xx.h"
#include <string.h>

void uart_init(void)
{
    // PA2 (TX), PA3 (RX) alternate function
    OS_PC_UART_GPIO_PORT->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    OS_PC_UART_GPIO_PORT->MODER |=  ((2U << (2*2)) | (2U << (3*2)));
    OS_PC_UART_GPIO_PORT->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    OS_PC_UART_GPIO_PORT->AFR[0] |=  ((7U << (4*2)) | (7U << (4*3))); // AF7 for USART2

    OS_PC_UART_INSTANCE->BRR = 16000000/115200;
    OS_PC_UART_INSTANCE->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void uart_log(const char* msg)
{
    const char* p = msg;
    while (*p) {
        while (!(OS_PC_UART_INSTANCE->SR & USART_SR_TXE));
        OS_PC_UART_INSTANCE->DR = *p++;
    }
    // Gửi ký tự xuống dòng
    while (!(OS_PC_UART_INSTANCE->SR & USART_SR_TXE));
    OS_PC_UART_INSTANCE->DR = '\r';
    while (!(OS_PC_UART_INSTANCE->SR & USART_SR_TXE));
    OS_PC_UART_INSTANCE->DR = '\n';
}