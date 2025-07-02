#include "mqtt_response_handler.h"
#include "hardware/uart.h"
#include "lwip/apps/mqtt.h"
#include "main.h"
#include "middle/led_logic.h"
#include <stdio.h>
#include <string.h>
#include "mqtt/mqtt_example.h"
#include "mqtt_response_parser.h"

extern mqtt_client_t* mqtt_client;
extern device_state_t current_device_state;
extern swich_state_t switch_state;

void handle_led_stt_switch(int cmd_code) {
    if (cmd_code == 1) { 
        switch_state = swich_on;
        //uart_log("led_stt_switch: ON");
    } else if (cmd_code == 2) { 
        switch_state = swich_off;
        //uart_log("led_stt_switch: OFF");
    } else if (cmd_code == 3) { 
        mqtt_publish_switch_state();
        //uart_log("led_stt_switch: RELAY");
    }
    
    //uart_log("publish");
}

void mqtt_handle_command_and_respond(const char* topic, int cmd_code) {
    handle_led_stt_switch(cmd_code);
}

void mqtt_publish_switch_state(void) {
    uart_log("mqtt_publish_switch_state called");
    char response_topic[64] = "device/switch1/status";
    char response_msg[128];

    snprintf(response_msg, sizeof(response_msg),
             "{\"status\":\"%s\",\"state\":\"%s\"}",
             get_switch_state_string(switch_state),
             get_device_state_string(current_device_state));

    mqtt_publish(mqtt_client, response_topic, response_msg, strlen(response_msg), 0, 0, NULL, NULL);
}