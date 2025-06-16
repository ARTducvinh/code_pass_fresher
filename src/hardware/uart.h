#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void uart_init(void);
void uart_log(const char* msg);

#ifdef __cplusplus
}
#endif

#endif