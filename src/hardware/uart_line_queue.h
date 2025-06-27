#ifndef UART_LINE_QUEUE_H
#define UART_LINE_QUEUE_H

#include <stdbool.h>
#include <stdint.h>

#define UART_LINE_QUEUE_SIZE 8
#define UART_LINE_MAX_LEN 128

#ifdef __cplusplus
extern "C" {
#endif

void uart_line_queue_init(void);
bool uart_line_queue_push(const char *line);
bool uart_line_queue_pop(char *out_line);
uint8_t uart_line_queue_count_get(void);

#ifdef __cplusplus
}
#endif

#endif // UART_LINE_QUEUE_H
