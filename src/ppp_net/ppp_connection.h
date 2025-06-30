#ifndef __PPP_CONNECTION_H__
#define __PPP_CONNECTION_H__

#include <stdbool.h>
#include "lwip/netif.h"
#include "netif/ppp/ppp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief PPP control block.
 * @note  Defined in ppp_connection.c
 */
extern ppp_pcb *ppp;

/**
 * @brief Global flag to indicate if the system is in PPP mode.
 * @note  Defined in ppp_connection.c
 */
extern volatile bool gsm_ppp_mode;

/**
 * @brief Initializes the PPP connection.
 */
void ppp_connection_init(void);

/**
 * @brief Polls the PPP connection status.
 *        Should be called periodically in the main loop.
 */
void ppp_connection_poll(void);

/**
 * @brief Closes the PPP connection.
 */
void ppp_connection_close(void);

// Khai báo biến cờ để các file khác có thể truy cập
extern volatile bool ppp_connection_established;

#ifdef __cplusplus
}
#endif

#endif /* __PPP_CONNECTION_H__ */