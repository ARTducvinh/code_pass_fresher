#include "gsm_parser.h"
#include "gsm_commands.h"
#include "uart.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "gsm_state.h"


// Hàm phân tích phản hồi cho lệnh "AT"
bool parse_response_at(const char* response) {
    uart_log(response);
    return strstr(response, "OK") != NULL;
}

// Hàm phân tích phản hồi cho lệnh "ATI"
bool parse_response_ati(const char* response) {
    uart_log(response);
    return strstr(response, "Manufacturer: INCORPORATED") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+CPIN?"
bool parse_response_at_cpin(const char* response) {
    uart_log(response);
    return strstr(response, "+CPIN: READY") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+CSQ"
bool parse_response_at_csq(const char* response) {
    uart_log(response);
    return strstr(response, "+CSQ:") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+CEREG?"
bool parse_response_at_cereg(const char* response) {
    uart_log(response);
    return strstr(response, "+CEREG: 0,1") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+COPS"
bool parse_response_at_cops(const char* response) {
    uart_log(response);
    return strstr(response, "+COPS=0") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+CGDCONT"
bool parse_response_at_cgdcont(const char* response) {
    uart_log(response);
    return strstr(response, "OK") != NULL; // Dòng xác nhận
}

// Hàm phân tích phản hồi cho lệnh "AT+CGAUTH"
bool parse_response_at_cgauth(const char* response) {
    uart_log(response);
    return strstr(response, "OK") != NULL; // Dòng xác nhận
}

// Hàm phân tích phản hồi cho lệnh "AT+CGACT"
bool parse_response_at_cgact(const char* response) {
    uart_log(response);
    return strstr(response, "OK") != NULL; // Dòng xác nhận
}

// Hàm phân tích phản hồi cho lệnh "AT+CGPADDR"
bool parse_response_at_cgpaddr(const char* response) {
    uart_log(response);
    return strstr(response, "+CGPADDR: 1,") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "AT+CGDATA"
bool parse_response_at_cgdata(const char* response) {
    uart_log(response);
    return strstr(response, "CONNECT") != NULL; // Dòng dữ liệu chính
}

// Hàm phân tích phản hồi cho lệnh "ATD*99#"
bool parse_response_atd99(const char* response) {
    uart_log(response);
    if (strstr(response, "CONNECT") != NULL) {
        // ppp_mode = true;
        //uart_disable_uart1_irq();
        restart_dma2_stream2(); // Khởi động lại DMA để nhận dữ liệu PPP
        return true;
    }
    return false;
}

// // Hàm phân tích phản hồi cho lệnh "+++"
// bool parse_response_plus(const char* response) {
//     uart_log(response);
//     return strstr(response, "OK") != NULL; // Dòng xác nhận
// }

// // Hàm phân tích phản hồi cho lệnh "ATH"
// bool parse_response_ath(const char* response) {
//     uart_log(response);
//     return strstr(response, "OK") != NULL; // Dòng xác nhận
// }