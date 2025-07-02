#include <stdint.h>
#include "gsm.h"
#include "gsm_commands.h"
#include "uart.h"
#include "gsm_parser.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "uart_line_queue.h"
#include "main.h"
#include "gsm_state.h"

// Hàm gửi lệnh AT dựa trên chỉ số lệnh trong gsm_commands
void send_gsm_command_by_index(int idx) {
    if (idx < 0 || idx >= gsm_commands_count) return;
    uart1_send_raw((uint8_t *)gsm_commands[idx].syntax, strlen(gsm_commands[idx].syntax));
    uart1_send_raw((uint8_t *)"\r\n", 2);
}

// Hàm gửi lần lượt các lệnh, chỉ gửi tiếp khi phản hồi thành công
void send_all_gsm_commands_with_check(void)
{
    static int current_command_index = 0;
    static uint32_t start_tick = 0;
    static uint32_t last_send_tick = 0;
    static bool waiting_for_response = false;
    char response[256];

    if (current_command_index >= gsm_commands_count) {
        return; 
    }

    if (!waiting_for_response) {
        start_tick = timer2_get_tick();
        last_send_tick = start_tick - 5000;
        waiting_for_response = true;
    }

    uint32_t wait_time = gsm_commands[current_command_index].max_response_time_ms;
    if ((timer2_get_tick() - start_tick) < wait_time) {
        if ((timer2_get_tick() - last_send_tick) >= 3000) { 
            send_gsm_command_by_index(current_command_index);
            last_send_tick = timer2_get_tick();
        }
        while (uart_line_queue_pop(response)) {
            if (check_gsm_response_by_index(current_command_index, response)) {
                waiting_for_response = false;
                current_command_index++;
                return; 
            }
        }
    } else {
        current_device_state = DEVICE_STATE_GSM_ERROR;
        char log_buf[64];
        snprintf(log_buf, sizeof(log_buf), "Loi khi gui lenh case %d: %s", current_command_index, gsm_commands[current_command_index].command);
        uart_log(log_buf);
        waiting_for_response = false;
        current_command_index = gsm_commands_count;
    }
}