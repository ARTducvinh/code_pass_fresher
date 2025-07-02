#include "mqtt_response_parser.h"
#include <string.h>
#include <stdio.h>
#include "hardware/uart.h"
#include "main.h"

// Trả về 1 nếu là ON, 2 nếu là OFF, 0 nếu không hợp lệ
int mqtt_parse_command(const char* topic, const uint8_t* data, uint16_t len) {
    if (topic == NULL || data == NULL || len == 0) return 0;

    // Tìm chuỗi "msg"
    const char* msg_ptr = strstr((const char*)data, "\"msg\"");
    if (msg_ptr) {
        const char* colon = strchr(msg_ptr, ':');
        if (colon) {
            const char* value = colon + 1;
            while (*value == ' ' || *value == '\"') value++;
            if (strncmp(value, "ON", 2) == 0) {
                uart_log("MQTT CMD: ON");
                return 1;
            }
            if (strncmp(value, "OFF", 3) == 0) {
                uart_log("MQTT CMD: OFF");
                return 2;
            }
        }
    }
    return 0;
}

const char* get_device_state_string(device_state_t state) {
    switch (state) {
        case DEVICE_STATE_BOOTING_GSM:     return "BOOTING_GSM";
        case DEVICE_STATE_GSM_READY:       return "GSM_READY";
        case DEVICE_STATE_GSM_ERROR:       return "GSM_ERROR";
        case DEVICE_STATE_BOOTING_DEVICE:  return "BOOTING_DEVICE";
        case DEVICE_STATE_DEVICE_READY:    return "DEVICE_READY";
        case DEVICE_STATE_DEVICE_ERROR:    return "DEVICE_ERROR";
        default:                           return "UNKNOWN";
    }
}

const char* get_switch_state_string(swich_state_t state) {
    switch (state) {
        case swich_on:  return "ON";
        case swich_off: return "OFF";
        default:        return "UNKNOWN";
    }
}