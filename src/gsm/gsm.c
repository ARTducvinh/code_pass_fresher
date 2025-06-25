#include <stdint.h>
#include "gsm.h"
#include "gsm_state.h"
#include "gsm_parser.h"
#include <string.h>
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "hardware/led.h"
#include "middle/led_logic.h"
#include "hardware/timer.h" // Đảm bảo bạn có hàm delay_ms

#define GSM_POWER_GPIO_PORT GPIOA
#define GSM_POWER_PIN       8

gsm_state_t current_state = GSM_STATE_POWER_OFF;
bool ppp_connected = false;

extern uint8_t rx_buffer[128];
extern volatile bool rx_line_ready;

static const char* gsm_get_response(void) {
    if (rx_line_ready) {
        rx_line_ready = false;
        extern void uart_log(const char* msg);
        uart_log("GSM RX (raw):");
        uart_log((const char*)rx_buffer);
        static char parsed_line[128];
        strncpy(parsed_line, (const char*)rx_buffer, sizeof(parsed_line) - 1);
        parsed_line[sizeof(parsed_line) - 1] = '\0';
        // Nếu nhận được CONNECT thì set ppp_connected = true và chuyển sang PPP data mode
        if (strstr((const char*)rx_buffer, "CONNECT") != NULL) {
            ppp_connected = true;
            uart_set_ppp_mode(true); // Bổ sung: chuyển UART sang chế độ PPP data
        }
        memset(rx_buffer, 0, sizeof(rx_buffer));
        return parsed_line;
    }
    return NULL;
}

void gsm_init(void)
{
    GSM_POWER_GPIO_PORT->MODER &= ~(3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->MODER |=  (1U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->OTYPER &= ~(1U << GSM_POWER_PIN);
    GSM_POWER_GPIO_PORT->OSPEEDR |= (3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->PUPDR &= ~(3U << (GSM_POWER_PIN * 2));
    GSM_POWER_GPIO_PORT->ODR |= (1U << GSM_POWER_PIN);

    current_state = GSM_STATE_POWER_ON;
    ppp_connected = false;
}

void gsm_send_command(const char* cmd)
{
    usart1_hw_uart_send_raw((const uint8_t*)cmd, strlen(cmd));
}

gsm_state_t gsm_get_state(void)
{
    return current_state;
}

bool gsm_is_ppp_mode(void)
{
    return (current_state == GSM_STATE_PPP_MODE);
}

void gsm_set_error(void)
{
    current_state = GSM_STATE_ERROR;
}

// Hàm cập nhật trạng thái LED, chỉ gọi khi trạng thái GSM thay đổi
static void gsm_update_led_mode(gsm_state_t state) {
    switch (state) {
        case GSM_STATE_ERROR:
            led_4g_mode = LED_4G_BLINK_200_500_200_2000;
            break;
        case GSM_STATE_POWER_ON:
        case GSM_STATE_WAIT_READY:
        case GSM_STATE_AT_OK:
        case GSM_STATE_SIGNAL_OK:
        case GSM_STATE_APN_SET:
            led_4g_mode = LED_4G_BLINK_200_200;
            break;
        case GSM_STATE_DIALING:
        case GSM_STATE_PPP_MODE:
            led_4g_mode = LED_4G_ON;
            break;
        default:
            led_4g_mode = LED_4G_OFF;
            break;
    }
}

void gsm_process(void)
{
    static uint32_t last_cmd_tick = 0;
    static gsm_state_t last_state = GSM_STATE_POWER_OFF;
    static int retry_count = 0;
    const uint32_t GSM_CMD_TIMEOUT = 2000;
    const int GSM_CMD_MAX_RETRY = 3;

    const char* resp = gsm_get_response();

    static gsm_state_t prev_led_state = GSM_STATE_POWER_OFF;
    if (current_state != prev_led_state) {
        gsm_update_led_mode(current_state);
        prev_led_state = current_state;
    }

    // Chỉ bỏ qua gửi lệnh và kiểm tra lỗi ở DIALING/PPP_MODE
    bool skip_cmd = (current_state == GSM_STATE_DIALING || current_state == GSM_STATE_PPP_MODE);

    if (!skip_cmd && resp) {
        gsm_state_t new_state = gsm_state_handle_response(current_state, resp);
        if (new_state == current_state) {
            if (gsm_parser_is_sim_error(resp) || gsm_parser_is_error(resp)) {
                current_state = GSM_STATE_ERROR;
                return;
            }
        } else {
            current_state = new_state;
            retry_count = 0;
            last_cmd_tick = timer2_get_tick();
        }
    }

    if (!skip_cmd) {
        if (current_state != last_state) {
            switch (current_state) {
                case GSM_STATE_POWER_ON:
                    gsm_send_command("ATE0\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_WAIT_READY:
                    gsm_send_command("AT+CPIN?\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_AT_OK:
                    gsm_send_command("AT+CSQ\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_SIGNAL_OK:
                    gsm_send_command("AT+CGDCONT=1,\"IP\",\"m-internet\"\r\n");
                    uart_log("GSM: APN da duoc cau hinh");
                    delay_ms(200);
                    // Gửi lệnh attach GPRS, chỉ log, không kiểm tra phản hồi để tránh treo
                    gsm_send_command("AT+CGATT=1\r\n");
                    uart_log("GSM: da gui AT+CGATT=1 (khong kiem tra phan hoi)");
                    delay_ms(200);
                    break;
                case GSM_STATE_APN_SET:
                    gsm_send_command("AT+CGACT=1,1\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_DIALING:
                    gsm_send_command("ATD*99***1#\r\n");
                    delay_ms(200);
                    break;
                default:
                    break;
            }
            last_cmd_tick = timer2_get_tick();
            retry_count = 0;
            last_state = current_state;
        } else if (timer2_get_tick() - last_cmd_tick > GSM_CMD_TIMEOUT && retry_count < GSM_CMD_MAX_RETRY) {
            switch (current_state) {
                case GSM_STATE_POWER_ON:
                    gsm_send_command("ATE0\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_WAIT_READY:
                    gsm_send_command("AT+CPIN?\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_AT_OK:
                    gsm_send_command("AT+CSQ\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_SIGNAL_OK:
                    gsm_send_command("AT+CGDCONT=1,\"IP\",\"m-internet\"\r\n");
                    uart_log("GSM: APN da duoc cau hinh");
                    delay_ms(200);
                    // Gửi lệnh attach GPRS khi retry, chỉ log, không kiểm tra phản hồi
                    gsm_send_command("AT+CGATT=1\r\n");
                    uart_log("GSM: da gui AT+CGATT=1 (retry, khong kiem tra phan hoi)");
                    delay_ms(200);
                    break;
                case GSM_STATE_APN_SET:
                    gsm_send_command("AT+CGACT=1,1\r\n");
                    delay_ms(200);
                    break;
                case GSM_STATE_DIALING:
                    gsm_send_command("ATD*99***1#\r\n");
                    delay_ms(200);
                    break;
                default:
                    break;
            }
            last_cmd_tick = timer2_get_tick();
            retry_count++;
        }

        if (retry_count >= GSM_CMD_MAX_RETRY) {
            current_state = GSM_STATE_ERROR;
            retry_count = 0;
        }
    }
}