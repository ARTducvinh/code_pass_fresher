#include "lwip/apps/mqtt.h"
#include "mqtt_example.h"
#include "hardware/timer.h"
#include "hardware/uart.h" // Để sử dụng uart_log

#if LWIP_TCP

/** Địa chỉ broker mới: 103.56.158.48, port 1883 */
static ip_addr_t mqtt_ip = IPADDR4_INIT_BYTES(103, 56, 158, 48);
#define MQTT_PORT 1883

static mqtt_client_t* mqtt_client;

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

static void
mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Incoming publish: topic %s, len %d", topic, (int)tot_len);
    uart_log(log_buf);
}

static void
mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Incoming data: len %d, flags %d", (int)len, (int)flags);
    uart_log(log_buf);

    // In nội dung dữ liệu nhận được qua log
    uart_log("Received data:");
    uart_log((const char *)data); // In dữ liệu nhận được qua log

    // Gửi dữ liệu nhận được qua MQTT publish
    const char *response_topic = "mydevice2/room1/switch01/response";
    mqtt_publish(mqtt_client, response_topic, data, len, 0, 0, NULL, NULL);
}

static void
mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    const struct mqtt_connect_client_info_t* client_info = (const struct mqtt_connect_client_info_t*)arg;

    if (status == MQTT_CONNECT_ACCEPTED) {
        uart_log("MQTT connection accepted!");

        // Subscribe tới topic
        mqtt_sub_unsub(client,
                "mydevice1/room1/switch01/cmd", 0, /* QoS 0 */
                NULL, LWIP_CONST_CAST(void*, client_info),
                1);

        // Publish dữ liệu tới broker
        const char *topic = "mydevice2/room1/switch01/cmd";
        const char *message = "{\"msg\": \"ON\"}";
        mqtt_publish(client, topic, message, strlen(message), 0, 0, NULL, NULL);
    } else {
        uart_log("MQTT connection failed!");
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
#endif /* LWIP_TCP */
}

#endif /* LWIP_TCP */