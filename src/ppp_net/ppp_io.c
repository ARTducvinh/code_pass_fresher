#include "ppp_io.h"
#include "uart.h"
#include <string.h>

// Hàm này được LwIP gọi để gửi dữ liệu PPP ra UART
u32_t ppp_io_output_cb(ppp_pcb *pcb, const u8_t *data, u32_t len, void *ctx)
{
    usart1_hw_uart_send_raw(data, len); // Gửi ra UART GSM
    return len;
}

// Hàm này bạn cần gọi khi nhận được dữ liệu từ UART GSM (ví dụ trong hàm nhận UART)
void ppp_io_input(ppp_pcb *ppp, const u8_t *data, u32_t len)
{
    if (ppp) {
        pppos_input(ppp, data, len); // SỬA: dùng API chuẩn của lwIP
    }
}
