#include "ppp_connection.h"
#include "hardware/uart.h"
#include "lwip/tcpip.h"
#include "netif/ppp/pppos.h"
#include <string.h>

static ppp_pcb *ppp = NULL;
static struct netif ppp_netif;
static bool ppp_connected = false;

static void ppp_status_cb(ppp_pcb *pcb, int err_code, void *ctx) {
    (void)ctx;
    switch (err_code) {
        case PPPERR_NONE:
            uart_log("PPP connected");
            ppp_connected = true;
            break;
        case PPPERR_USER:
            uart_log("PPP connection closed");
            ppp_connected = false;
            break;
        default:
            uart_log("PPP error occurred");
            ppp_connected = false;
            break;
    }
}

void ppp_connection_init(void) {
    uart_log("Initializing PPP connection...");

    tcpip_init(NULL, NULL);

    ppp = pppos_create(&ppp_netif, uart1_send_raw, gsm_hw_layer_uart_fill_rx, ppp_status_cb, NULL);
    if (ppp == NULL) {
        uart_log("Failed to create PPP control block");
        return;
    }

    ppp_set_default(ppp);
    ppp_connect(ppp, 0);
}

void ppp_connection_poll(void) {
    if (ppp_connected) {
        uart_log("PPP connection is active");
    } else {
        uart_log("PPP connection is not active");
    }
}

void ppp_connection_close(void) {
    if (ppp != NULL) {
        ppp_close(ppp, 0);
        ppp = NULL;
    }
}
