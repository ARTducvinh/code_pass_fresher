#include "uart.h"
#include "hardware.h"
#include "stm32f4xx.h"
#include "core_cm4.h"
#include <stdbool.h>
#include <string.h>
#include "uart_line_queue.h"
#include "gsm_state.h"
#include "netif/ppp/pppos.h"
#include "ppp_net/ppp_connection.h"
#include <stdint.h>
#include <stdio.h>

#define UART1_RX_BUFFER_SIZE 256

uint8_t m_uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
volatile uint32_t m_old_uart1_dma_rx_pos = 0;
volatile bool rx_line_ready = false;

void uart_init_all(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->MODER |= ((2U << (9 * 2)) | (2U << (10 * 2)));
    GPIOA->AFR[1] &= ~((0xF << (4 * 1)) | (0xF << (4 * 2)));
    GPIOA->AFR[1] |= ((7U << (4 * 1)) | (7U << (4 * 2)));

    USART1->BRR = 16000000 / 115200;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2)));
    GPIOA->MODER |= ((2U << (2 * 2)) | (2U << (3 * 2)));
    GPIOA->AFR[0] &= ~((0xF << (4 * 2)) | (0xF << (4 * 3)));
    GPIOA->AFR[0] |= ((7U << (4 * 2)) | (7U << (4 * 3)));

    USART2->BRR = 16000000 / 115200;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    USART1->CR3 |= USART_CR3_DMAR;

    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN);

    DMA2_Stream2->PAR  = (uint32_t)&USART1->DR;
    DMA2_Stream2->M0AR = (uint32_t)m_uart1_rx_buffer;
    DMA2_Stream2->NDTR = UART1_RX_BUFFER_SIZE;

    DMA2_Stream2->CR =
        (4U << 25) |
        DMA_SxCR_MINC |
        DMA_SxCR_CIRC |
        (0 << 6) |
        (0 << 13) |
        (0 << 11) |
        (0 << 16);

    DMA2_Stream2->CR |= DMA_SxCR_EN;

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    uart_enable_uart1_idle_irq();
}

void uart_log(const char* msg)
{
    const char* p = msg;
    while (*p) {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = *p++;
    }
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = '\r';
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = '\n';
}

void uart1_send_raw(const uint8_t* raw, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = raw[i];
    }
    while (!(USART1->SR & USART_SR_TC));
}

void gsm_hw_layer_uart_fill_rx(uint8_t *data, uint32_t length)
{
    static uint8_t line_buf[UART_LINE_MAX_LEN];
    static uint16_t line_idx = 0;

    for (uint32_t i = 0; i < length; i++) {
        if (line_idx < UART_LINE_MAX_LEN - 1) {
            line_buf[line_idx++] = data[i];
            if (data[i] == '\n' || data[i] == '\r') {
                if (line_idx > 1) {
                    line_buf[line_idx] = 0;
                    uart_line_queue_push((char*)line_buf);
                }
                line_idx = 0;
            }
        } else {
            line_idx = 0;
        }
    }
}

void uart1_poll(void)
{
    if (!rx_line_ready) {
        return;
    }
    rx_line_ready = false;

    uint32_t pos = UART1_RX_BUFFER_SIZE - DMA2_Stream2->NDTR;

    if (pos == m_old_uart1_dma_rx_pos) {
        return;
    }

    if (gsm_ppp_mode && ppp != NULL) {
        if (pos > m_old_uart1_dma_rx_pos) {
            pppos_input(ppp, &m_uart1_rx_buffer[m_old_uart1_dma_rx_pos], pos - m_old_uart1_dma_rx_pos);
        } else {
            pppos_input(ppp, &m_uart1_rx_buffer[m_old_uart1_dma_rx_pos], UART1_RX_BUFFER_SIZE - m_old_uart1_dma_rx_pos);
            if (pos > 0) {
                pppos_input(ppp, &m_uart1_rx_buffer[0], pos);
            }
        }
    } else {
        if (pos > m_old_uart1_dma_rx_pos) {
            gsm_hw_layer_uart_fill_rx(&m_uart1_rx_buffer[m_old_uart1_dma_rx_pos], pos - m_old_uart1_dma_rx_pos);
        } else {
            gsm_hw_layer_uart_fill_rx(&m_uart1_rx_buffer[m_old_uart1_dma_rx_pos], UART1_RX_BUFFER_SIZE - m_old_uart1_dma_rx_pos);
            if (pos > 0) {
                gsm_hw_layer_uart_fill_rx(&m_uart1_rx_buffer[0], pos);
            }
        }
    }
    m_old_uart1_dma_rx_pos = pos;
    if (m_old_uart1_dma_rx_pos == UART1_RX_BUFFER_SIZE)
        m_old_uart1_dma_rx_pos = 0;
}

void DMA2_Stream2_IRQHandler(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF2) {
        DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
        rx_line_ready = true;
    }
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_IDLE) {
        volatile uint32_t tmp;
        tmp = USART1->SR;
        tmp = USART1->DR;
        (void)tmp;
        rx_line_ready = true;
    }
}

void uart_enable_uart1_idle_irq(void)
{
    USART1->CR1 |= USART_CR1_IDLEIE;
    NVIC_EnableIRQ(USART1_IRQn);
}

void uart_disable_uart1_irq(void)
{
    USART1->CR1 &= ~USART_CR1_IDLEIE;
    NVIC_DisableIRQ(USART1_IRQn);
    uart_log("UART1 IRQ disabled");
}

void restart_dma2_stream2(void)
{
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN);

    DMA2_Stream2->PAR  = (uint32_t)&USART1->DR;
    DMA2_Stream2->M0AR = (uint32_t)m_uart1_rx_buffer;
    DMA2_Stream2->NDTR = UART1_RX_BUFFER_SIZE;

    DMA2_Stream2->CR =
        (4U << 25) |
        DMA_SxCR_MINC |
        DMA_SxCR_CIRC |
        (0 << 6) |
        (0 << 13) |
        (0 << 11) |
        (0 << 16);

    DMA2_Stream2->CR |= DMA_SxCR_EN;
}
