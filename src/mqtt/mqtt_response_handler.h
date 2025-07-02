#ifndef MQTT_RESPONSE_HANDLER_H
#define MQTT_RESPONSE_HANDLER_H
#include <stdint.h>

// Xử lý phản hồi đã phân tích và gửi lại trạng thái thiết bị
void mqtt_handle_command_and_respond(const char* topic, int cmd_code);
void handle_led_stt_switch(int cmd_code);
#endif // MQTT_RESPONSE_HANDLER_H
