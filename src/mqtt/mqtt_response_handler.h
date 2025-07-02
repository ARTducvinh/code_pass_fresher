#ifndef MQTT_RESPONSE_HANDLER_H
#define MQTT_RESPONSE_HANDLER_H
#include <stdint.h>


void mqtt_handle_command_and_respond(const char* topic, int cmd_code);
void handle_led_stt_switch(int cmd_code);
void mqtt_publish_switch_state(void);
#endif // MQTT_RESPONSE_HANDLER_H
