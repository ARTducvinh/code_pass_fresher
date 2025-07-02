#include "gsm_parser.h"
#include "gsm_commands.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "gsm_state.h"
#include "ppp_net/ppp_connection.h"
#include "main.h"

bool parse_response_at(const char* response) {
    //uart_log(response);
    return strstr(response, "OK") != NULL;
}

bool parse_response_ati(const char* response) {
    //uart_log(response);
    return strstr(response, "Manufacturer: INCORPORATED") != NULL;
}

bool parse_response_at_cpin(const char* response) {
    //uart_log(response);
    return strstr(response, "+CPIN: READY") != NULL;
}

bool parse_response_at_csq(const char* response) {
    //uart_log(response);
    return strstr(response, "+CSQ:") != NULL;
}

bool parse_response_at_cereg(const char* response) {
    //uart_log(response);
    return strstr(response, "+CEREG: 0,1") != NULL;
}

bool parse_response_at_cops(const char* response) {
    //uart_log(response);
    return strstr(response, "+COPS=0") != NULL;
}

bool parse_response_at_cgdcont(const char* response) {
   // uart_log(response);
    return strstr(response, "OK") != NULL;
}

bool parse_response_at_cgauth(const char* response) {
    //uart_log(response);
    return strstr(response, "OK") != NULL;
}

bool parse_response_at_cgact(const char* response) {
    //uart_log(response);
    return strstr(response, "OK") != NULL;
}

bool parse_response_at_cgpaddr(const char* response) {
    //uart_log(response);
    return strstr(response, "+CGPADDR: 1,") != NULL;
}

bool parse_response_at_cgdata(const char* response) {
    //uart_log(response);
    return strstr(response, "CONNECT") != NULL;
}

bool parse_response_atd99(const char* response) {
    //uart_log(response);
    if (strstr(response, "CONNECT") != NULL) {
        gsm_ppp_mode = true;
        //uart_log("Switching to PPP mode.");
        current_device_state = DEVICE_STATE_BOOTING_DEVICE;
        restart_dma2_stream2();
        return true;
    }
    return false;
}