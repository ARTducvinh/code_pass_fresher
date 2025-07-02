#include "lwip/apps/mqtt.h"
#include "mqtt_example.h"
#include "hardware/timer.h"
#include "hardware/uart.h" 
#include "main.h" 
#include "mqtt_response_parser.h"
#include "mqtt_response_handler.h"
#include <string.h>
#if LWIP_TCP

/** Địa chỉ broker mới: 103.56.158.48, port 1883 */
static ip_addr_t mqtt_ip = IPADDR4_INIT_BYTES(103, 56, 158, 48);

static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  "vinh2kk2",    /* client id */
  "test",              /* user */
  "test",              /* pass */
  100,                 /* keep alive */
  NULL,                /* will_topic */
  NULL,                /* will_msg */
  0,                   /* will_msg_len */
  0,                   /* will_qos */
  0                    /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

mqtt_client_t* mqtt_client = NULL;

static char last_topic[128] = {0};
static int mqtt_phase = 0; 
static int mqtt_reconnect_attempts = 0;
#define MQTT_MAX_RECONNECT_ATTEMPTS 10

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

static void mqtt_try_reconnect(void) {
    if (mqtt_reconnect_attempts < MQTT_MAX_RECONNECT_ATTEMPTS) {
        mqtt_reconnect_attempts++;
        uart_log("MQTT reconnecting...");
        delay_ms(2000); 
        mqtt_client_connect(mqtt_client,
            &mqtt_ip, MQTT_PORT,
            mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
            &mqtt_client_info);
    } else {
        uart_log("MQTT reconnect failed too many times!");
        current_device_state = DEVICE_STATE_DEVICE_ERROR;
    }
}

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    snprintf(last_topic, sizeof(last_topic), "%s", topic);
    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Incoming publish: topic %s, len %d", topic, (int)tot_len);
    uart_log(log_buf);
}

static void
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    uart_log("Received data:");
    uart_log((const char *)data);

    if (mqtt_phase == 0 && strcmp(last_topic, "start/device/cmd") == 0) {
        uart_log("Received connect confirm, switching to main topics.");
        mqtt_sub_unsub(mqtt_client, "start/device/cmd", 0, NULL, NULL, 0); 
        mqtt_sub_unsub(mqtt_client, "device/switch1/cmd", 0, NULL, NULL, 1); 
        const char *topic = "device/switch1/status";
        const char *message = "{\"msg\": \"OFF\"}";
        mqtt_publish(mqtt_client, topic, message, strlen(message), 0, 0, NULL, NULL);
        mqtt_phase = 1;
    } else if (mqtt_phase == 1 && strcmp(last_topic, "device/switch1/cmd") == 0) {
        int cmd_code = mqtt_parse_command(last_topic, data, len);
        if (cmd_code == 1 || cmd_code == 2) {
            mqtt_handle_command_and_respond("device/switch1/status", cmd_code);
        }
    }
}

static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

    if (status == MQTT_CONNECT_ACCEPTED) {
        uart_log("MQTT connection accepted!");
        current_device_state = DEVICE_STATE_DEVICE_READY;
        mqtt_reconnect_attempts = 0; 

        if (mqtt_phase == 1) {
            mqtt_sub_unsub(client, "device/switch1/cmd", 0, NULL, NULL, 1);
            const char *topic = "device/switch1/status";
            const char *message = "{\"msg\": \"Reconnect broker\"}";
            mqtt_publish(client, topic, message, strlen(message), 0, 0, NULL, NULL);
        } else {
            mqtt_phase = 0;
            mqtt_sub_unsub(client,
                    "start/device/cmd", 0,
                    NULL, LWIP_CONST_CAST(void*, client_info),
                    1);
            const char *topic = "start/device/status";
            const char *message = "{\"msg\": \"Connect broker\"}";
            mqtt_publish(client, topic, message, strlen(message), 0, 0, NULL, NULL);
        }
    } else {
        uart_log("MQTT connection failed!");
        current_device_state = DEVICE_STATE_DEVICE_ERROR;
        mqtt_try_reconnect();
    }
}

void
mqtt_example_init(void)
{
#if LWIP_TCP
  mqtt_client = mqtt_client_new();

  mqtt_set_inpub_callback(mqtt_client,
          mqtt_incoming_publish_cb,
          mqtt_incoming_data_cb,
          LWIP_CONST_CAST(void*, &mqtt_client_info));

  mqtt_client_connect(mqtt_client,
          &mqtt_ip, MQTT_PORT,
          mqtt_connection_cb, LWIP_CONST_CAST(void*, &mqtt_client_info),
          &mqtt_client_info);
#endif
}

#endif /* LWIP_TCP */