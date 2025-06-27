#include "ppp_net.h"
#include "ppp_io.h"
#include "netif/ppp/pppos.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"

static struct netif ppp_netif;
ppp_pcb *ppp = NULL;
ppp_net_state_t ppp_state = PPP_NET_DISCONNECTED;

static void ppp_status_cb(ppp_pcb *pcb, int err_code, void *ctx)
{
    char debug_msg[100];
    snprintf(debug_msg, sizeof(debug_msg), "PPP status callback: err_code=%d", err_code);
    uart_log(debug_msg);

    switch (err_code) {
    case PPPERR_NONE:
        uart_log("PPP: Connected successfully");
        ppp_state = PPP_NET_CONNECTED;
        break;
    case PPPERR_USER:
        uart_log("PPP: Disconnected by user");
        ppp_state = PPP_NET_DISCONNECTED;
        ppp = NULL;
        break;
    default:
        uart_log("PPP: Error occurred, closing connection");
        ppp_state = PPP_NET_ERROR;
        if (ppp) {
            ppp_close(ppp, 0);
            ppp = NULL;
        }
        break;
    }
}

void ppp_net_init(void)
{
    if (ppp == NULL) {
        ppp = pppos_create(&ppp_netif, ppp_io_output_cb, ppp_status_cb, NULL);
        if (ppp != NULL) {
            ppp_set_default(ppp);
            ppp_set_auth(ppp, PPPAUTHTYPE_NONE, NULL, NULL);
            ppp_state = PPP_NET_CONNECTING;
            ppp_connect(ppp, 0);
        }
    }
}

void ppp_net_poll(void)
{
    char debug_msg[50];
    snprintf(debug_msg, sizeof(debug_msg), "PPP state: %d", ppp_state);
    uart_log(debug_msg);

    if (ppp_state == PPP_NET_ERROR) {
        uart_log("PPP: Error detected, resetting state to DISCONNECTED");
        ppp_state = PPP_NET_DISCONNECTED;
    } else if (ppp_state == PPP_NET_CONNECTING) {
        uart_log("PPP: Currently connecting...");
    } else if (ppp_state == PPP_NET_CONNECTED) {
        uart_log("PPP: Connected successfully");
    } else if (ppp_state == PPP_NET_DISCONNECTED) {
        uart_log("PPP: Currently disconnected");
    }
}

bool ppp_net_is_connected(void)
{
    return (ppp_state == PPP_NET_CONNECTED);
}

struct netif* ppp_net_get_netif(void)
{
    return &ppp_netif;
}