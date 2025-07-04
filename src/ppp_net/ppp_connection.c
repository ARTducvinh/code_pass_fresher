#include "ppp_connection.h"
#include "hardware/uart.h"
#include "lwip/tcpip.h"
#include "netif/ppp/pppos.h"
#include <string.h>
#include <stdio.h> 
#include "lwip/netif.h" 
#include "lwip/ip_addr.h" 
#include "mqtt/mqtt_example.h" 
#include "net_test.h"
#include "timer.h" 

volatile bool ppp_connection_established = false;
ppp_pcb *ppp = NULL;
volatile bool gsm_ppp_mode = false;

static struct netif ppp_netif;
static bool ppp_connected = false;

static u32_t ppp_output_callback(ppp_pcb *pcb, const void *data, u32_t len, void *ctx) {
    (void)pcb;
    (void)ctx;
    uart1_send_raw((const uint8_t*)data, len);
    return len;
}

static void ppp_status_cb(ppp_pcb *pcb, int err_code, void *ctx) {
    (void)pcb;
    (void)ctx;
    switch (err_code) {
        case PPPERR_NONE: {
            char log_buf[100];
            uart_log("PPP connected");
            ppp_connected = true;
            gsm_ppp_mode = true;
            ppp_connection_established = true;

            sprintf(log_buf, "   IP address:  %s", ipaddr_ntoa(netif_ip4_addr(&ppp_netif)));
            uart_log(log_buf);
            sprintf(log_buf, "   Gateway:     %s", ipaddr_ntoa(netif_ip4_gw(&ppp_netif)));
            uart_log(log_buf);
            sprintf(log_buf, "   Netmask:     %s", ipaddr_ntoa(netif_ip4_netmask(&ppp_netif)));
            uart_log(log_buf);
            
            uart_log("Waiting for PPP to stabilize...");
            delay_ms(2000);

            uart_log("Checking routing...");
            if (netif_is_up(&ppp_netif)) {
                uart_log("PPP netif is up and routing is active.");
            } else {
                uart_log("PPP netif is down. Routing may not be active.");
            }

            uart_log("Testing Internet connection...");
            test_tcp_connect_google();
            uart_log("Internet connection test completed.");

            uart_log("Initializing MQTT...");
            mqtt_example_init();
            uart_log("MQTT initialization completed.");

            break;
        }
        case PPPERR_USER:
            uart_log("PPP connection terminated by user");
            ppp_connected = false;
            gsm_ppp_mode = false;
            ppp_connection_established = false;
            break;
        default:
            uart_log("PPP connection error");
            ppp_connected = false;
            gsm_ppp_mode = false;
            ppp_connection_established = false;
            break;
    }
}

void ppp_connection_init(void) {
    //uart_log("Initializing PPP connection...");

    tcpip_init(NULL, NULL);

    ppp = pppos_create(&ppp_netif, ppp_output_callback, ppp_status_cb, NULL);
    if (ppp == NULL) {
        uart_log("Failed to create PPP control block");
        return;
    }

    ppp_set_default(ppp);
    ppp_connect(ppp, 0);
}

void ppp_connection_poll(void) {
    sys_check_timeouts();
}

void ppp_connection_close(void) {
    if (ppp != NULL) {
        ppp_close(ppp, 0);
        ppp = NULL;
    }
}