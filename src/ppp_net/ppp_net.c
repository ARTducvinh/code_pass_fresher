#include "ppp_net.h"
#include "ppp_io.h"
#include "netif/ppp/pppos.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "lwip/tcpip.h"
#include <string.h>
#include <stdio.h>
#include "hardware/uart.h"

static struct netif ppp_netif;
ppp_pcb *ppp = NULL;
ppp_net_state_t ppp_state = PPP_NET_DISCONNECTED; // Định nghĩa biến trạng thái PPP để dùng extern

static void ppp_status_cb(ppp_pcb *pcb, int err_code, void *ctx)
{
    char logbuf[128];
    switch (err_code) {
    case PPPERR_NONE:
        ppp_state = PPP_NET_CONNECTED;
        sprintf(logbuf, "PPP: Connected successfully, IP: %s", ip4addr_ntoa(netif_ip4_addr(&ppp_netif)));
        uart_log(logbuf);
        break;
    case PPPERR_USER:
        uart_log("PPP: Disconnected by user");
        break;
    case PPPERR_CONNECT:
        uart_log("PPP: Connection failed, retrying...");
        break;
    case PPPERR_AUTHFAIL:
        uart_log("PPP: Authentication failed, check credentials");
        break;
    case PPPERR_PROTOCOL:
        uart_log("PPP: Protocol error during IPCP negotiation, restarting...");
        break;
    case PPPERR_PEERDEAD:
        uart_log("PPP: Peer is dead, attempting reconnection...");
        break;
    case PPPERR_IDLETIMEOUT:
        uart_log("PPP: Idle timeout, reconnecting...");
        break;
    case PPPERR_CONNECTTIME:
        uart_log("PPP: Connection timeout, retrying...");
        break;
    case PPPERR_LOOPBACK:
        uart_log("PPP: Loopback detected, restarting connection...");
        break;
    default:
        sprintf(logbuf, "PPP: Unknown error code %d, restarting...", err_code);
        uart_log(logbuf);
        break;
    }
}

void ppp_net_init(void)
{
    if (ppp == NULL) {
        ppp = pppos_create(&ppp_netif, ppp_io_output_cb, ppp_status_cb, NULL);
        if (ppp != NULL) {
            ppp_set_default(ppp);
            ppp_set_auth(ppp, PPPAUTHTYPE_NONE, NULL, NULL); // Không cần tài khoản và mật khẩu
            ppp_state = PPP_NET_CONNECTING;
            ppp_connect(ppp, 0);
        }
    }
}

void ppp_net_poll(void)
{
    struct netif* pppif = ppp_net_get_netif();
    if (pppif && !ip4_addr_isany_val(*netif_ip4_addr(pppif))) {
        char ipbuf[64];
        sprintf(ipbuf, "PPP: IP Address obtained: %s", ip4addr_ntoa(netif_ip4_addr(pppif)));
        uart_log(ipbuf);
    } else {
        uart_log("PPP: No IP address yet, retrying connection...");
        // Thêm log trạng thái PPP và con trỏ ppp
        char debugbuf[64];
        sprintf(debugbuf, "PPP state: %d, ppp ptr: %p", ppp_state, (void*)ppp);
        uart_log(debugbuf);
        if (ppp != NULL) {
            uart_log("PPP: Closing current PPP session...");
            ppp_close(ppp, 0);
            ppp = NULL; // Đảm bảo sẽ tạo lại ppp_pcb mới
        }
        uart_log("PPP: Reinitializing PPP connection...");
        ppp_net_init();
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
