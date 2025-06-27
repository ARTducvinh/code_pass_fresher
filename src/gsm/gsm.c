#include <stdint.h>
#include "gsm.h"
#include "gsm_commands.h"
#include "uart.h"
#include "gsm_parser.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "uart_line_queue.h"// định nghĩa tạm thời cho biến ppp_mode để xác định trạng thái 

// Hàm gửi lệnh AT dựa trên chỉ số lệnh trong gsm_commands
void send_gsm_command_by_index(int idx) {
    if (idx < 0 || idx >= gsm_commands_count) return;
    uart1_send_raw((uint8_t *)gsm_commands[idx].syntax, strlen(gsm_commands[idx].syntax));
    uart1_send_raw((uint8_t *)"\r\n", 2);
}

// Hàm gửi lần lượt các lệnh, chỉ gửi tiếp khi phản hồi thành công
void send_all_gsm_commands_with_check(void) {
    char response[256];

    for (int i = 0; i < gsm_commands_count; i++) { 
        char log_buf[64];
        snprintf(log_buf, sizeof(log_buf), "Dang gui lenh case %d: %s", i, gsm_commands[i].command);
        //uart_log(log_buf);

        uint32_t start_tick = timer2_get_tick();
        uint32_t wait_time = gsm_commands[i].max_response_time_ms;
        bool ok = false;
        uint32_t last_send_tick = start_tick - 5000;

        while ((timer2_get_tick() - start_tick) < wait_time) {
            if ((timer2_get_tick() - last_send_tick) >= 5000) {
                send_gsm_command_by_index(i);
                last_send_tick = timer2_get_tick();
            }
            while (uart_line_queue_pop(response)) {
                if (check_gsm_response_by_index(i, response)) {
                    ok = true;
                    break;
                }
            }
            if (ok) break;
            delay_ms(1);
        }
        if (!ok) {
            snprintf(log_buf, sizeof(log_buf), "Loi khi gui lenh case %d: %s", i, gsm_commands[i].command);
            uart_log(log_buf);
            break;
        }
    }
}