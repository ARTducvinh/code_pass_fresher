#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Khởi tạo UART
void uart_init_all(void);

// Gửi log qua UART
void uart_log(const char* msg);

// Gửi dữ liệu thô qua USART1
void uart1_send_raw(const uint8_t* raw, uint32_t length);

// Bật ngắt IDLE cho USART1
void uart_enable_uart1_idle_irq(void);

// Xử lý dữ liệu nhận được từ UART
void gsm_hw_layer_uart_fill_rx(uint8_t *data, uint32_t length);

void uart_disable_uart1_irq(void);

void usart1_dma_rx_check(void);

void uart1_poll(void);

void uart_log_hex(const uint8_t* data, uint32_t length);

void restart_dma2_stream2(void);
// Biến toàn cục
extern volatile bool rx_line_ready;
extern uint8_t rx_buffer[128];extern uint8_t rx_buffer[128];
extern volatile bool gsm_ppp_mode;

#ifdef __cplusplus
}
#endif

#endif