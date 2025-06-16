#include "middle/logger.h"
#include "hardware/uart.h"

void logger_init(void) {
    uart_init();
}

void logger_log(const char* msg) {
    uart_log(msg);
}