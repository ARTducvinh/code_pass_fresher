#ifndef MQTT_RESPONSE_PARSER_H
#define MQTT_RESPONSE_PARSER_H
#include <stdbool.h>
#include <stdint.h>

int mqtt_parse_command(const char* topic, const uint8_t* data, uint16_t len);

#endif // MQTT_RESPONSE_PARSER_H
