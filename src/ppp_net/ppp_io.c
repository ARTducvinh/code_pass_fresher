#include "ppp_io.h"
#include "uart.h"

u32_t ppp_io_output_cb(ppp_pcb *pcb, const void *data, u32_t len, void *ctx)
{
    char debug_msg[50];
    snprintf(debug_msg, sizeof(debug_msg), "PPP output: len=%lu", len);
    uart_log(debug_msg);

    uart1_send_raw((const uint8_t *)data, len); 
    return len;
}

void ppp_io_input(ppp_pcb *ppp, const u8_t *data, u32_t len)
{
    char debug_msg[50];
    snprintf(debug_msg, sizeof(debug_msg), "PPP input: len=%lu", len);
    uart_log(debug_msg);

    if (ppp) {
        pppos_input(ppp, data, len); 
    } else {
        uart_log("PPP input: PPP is NULL");
    }
}