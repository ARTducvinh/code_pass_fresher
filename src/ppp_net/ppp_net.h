#ifndef PPP_NET_H
#define PPP_NET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "lwip/netif.h"

typedef enum {
    PPP_NET_DISCONNECTED = 0,
    PPP_NET_CONNECTING,
    PPP_NET_CONNECTED,
    PPP_NET_ERROR
} ppp_net_state_t;

void ppp_net_init(void);
void ppp_net_poll(void);
bool ppp_net_is_connected(void);
struct netif* ppp_net_get_netif(void);

#ifdef __cplusplus
}
#endif

#endif // PPP_NET_H