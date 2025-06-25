#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_init_all(void);
void uart_log(const char* msg);
void usart1_control(bool enable);
void usart1_hw_uart_send_raw(const uint8_t* raw, uint32_t length);
void usart1_start_dma_rx(void);
void uart_poll_dma_rx();
void uart_enable_usart1_idle_irq(void);
void gsm_hw_layer_uart_fill_rx(uint8_t *data, uint32_t length);
void uart_set_ppp_mode(bool enable);

extern volatile bool rx_line_ready;
extern uint8_t rx_buffer[128];
extern uint8_t tx_buffer[128];

#ifdef __cplusplus
}
#endif

#endif