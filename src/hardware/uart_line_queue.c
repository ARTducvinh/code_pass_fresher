#include "uart_line_queue.h"
#include <string.h>

static char uart_line_queue[UART_LINE_QUEUE_SIZE][UART_LINE_MAX_LEN];
static volatile uint8_t uart_line_queue_head = 0;
static volatile uint8_t uart_line_queue_tail = 0;
static volatile uint8_t uart_line_queue_count = 0;

void uart_line_queue_init(void) {
    uart_line_queue_head = 0;
    uart_line_queue_tail = 0;
    uart_line_queue_count = 0;
}

// Push a line into the queue, returns true if success, false if full
bool uart_line_queue_push(const char *line) {
    if (uart_line_queue_count >= UART_LINE_QUEUE_SIZE) return false;
    strncpy(uart_line_queue[uart_line_queue_head], line, UART_LINE_MAX_LEN - 1);
    uart_line_queue[uart_line_queue_head][UART_LINE_MAX_LEN - 1] = '\0';
    uart_line_queue_head = (uart_line_queue_head + 1) % UART_LINE_QUEUE_SIZE;
    uart_line_queue_count++;
    return true;
}

// Pop a line from the queue, returns true if a line was available
bool uart_line_queue_pop(char *out_line) {
    if (uart_line_queue_count == 0) return false;
    strncpy(out_line, uart_line_queue[uart_line_queue_tail], UART_LINE_MAX_LEN);
    uart_line_queue_tail = (uart_line_queue_tail + 1) % UART_LINE_QUEUE_SIZE;
    uart_line_queue_count--;
    return true;
}

uint8_t uart_line_queue_count_get(void) {
    return uart_line_queue_count;
}
