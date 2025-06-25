#include "gsm_state.h"
#include "gsm_parser.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

// Hàm chuyển enum sang chuỗi (có thể đặt ở file header nếu dùng chung)
static const char* gsm_state_str(gsm_state_t state) {
    switch (state) {
        case GSM_STATE_POWER_OFF:      return "POWER_OFF";
        case GSM_STATE_POWER_ON:       return "POWER_ON";
        case GSM_STATE_WAIT_READY:     return "WAIT_READY";
        case GSM_STATE_AT_OK:          return "AT_OK";
        case GSM_STATE_SIGNAL_OK:      return "SIGNAL_OK";
        case GSM_STATE_APN_SET:        return "APN_SET";
        case GSM_STATE_DIALING:        return "DIALING";
        case GSM_STATE_PPP_MODE:       return "PPP_MODE";
        case GSM_STATE_ERROR:          return "ERROR";
        default:                       return "UNKNOWN";
    }
}

// Hàm log trạng thái (có thể thay bằng uart_log nếu cần)
static void gsm_log_state(gsm_state_t state) {
    char buf[64];
    snprintf(buf, sizeof(buf), "GSM State: %s", gsm_state_str(state));
    uart_log(buf);
    if (state == GSM_STATE_PPP_MODE) {
        uart_log("PPP mode established! GSM network is ready.");
    }
    if (state == GSM_STATE_ERROR) {
        uart_log("GSM error occurred!");
    }
}

// Hàm xử lý trạng thái chính sử dụng các hàm phân tích cú pháp mới
gsm_state_t gsm_state_handle_response(gsm_state_t current, const char* resp) {
    if (!resp || gsm_parser_is_empty_line(resp))
        return current;

    // Bỏ qua các thông báo nhà mạng hoặc các dòng không liên quan
    if (gsm_parser_has_prefix(resp, "*COPN:")) {
        // Có thể log hoặc xử lý riêng nếu cần
        return current;
    }

    if (gsm_parser_is_sim_error(resp)) {
        gsm_log_state(GSM_STATE_ERROR);
        return GSM_STATE_ERROR;
    }

    if (gsm_parser_is_error(resp)) {
        gsm_log_state(GSM_STATE_ERROR);
        return GSM_STATE_ERROR;
    }

    switch (current) {
        case GSM_STATE_POWER_ON:
            if (gsm_parser_is_ok(resp) || gsm_parser_has_prefix(resp, "*ATREADY: 1")) {
                gsm_log_state(GSM_STATE_WAIT_READY);
                return GSM_STATE_WAIT_READY;
            }
            break;

        case GSM_STATE_WAIT_READY:
            if (gsm_parser_is_ok(resp)) {
                gsm_log_state(GSM_STATE_AT_OK);
                return GSM_STATE_AT_OK;
            }
            break;

        case GSM_STATE_AT_OK:
            if (gsm_parser_is_ok(resp)) {
                gsm_log_state(GSM_STATE_SIGNAL_OK);
                return GSM_STATE_SIGNAL_OK;
            }
            break;

        case GSM_STATE_SIGNAL_OK:
            if (gsm_parser_is_ok(resp)) {
                gsm_log_state(GSM_STATE_APN_SET);
                return GSM_STATE_APN_SET;
            }
            break;

        case GSM_STATE_APN_SET:
            if (gsm_parser_is_ok(resp)) {
                gsm_log_state(GSM_STATE_DIALING);
                return GSM_STATE_DIALING;
            }
            break;

        case GSM_STATE_DIALING:
            if (gsm_parser_is_connect(resp)) {
                gsm_log_state(GSM_STATE_PPP_MODE);
                return GSM_STATE_PPP_MODE;
            }
            break;

        default:
            break;
    }

    return current;
}