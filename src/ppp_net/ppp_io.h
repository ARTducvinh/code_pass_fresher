#ifndef PPP_IO_H
#define PPP_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "netif/ppp/pppos.h"
#include <stdint.h>

u32_t ppp_io_output_cb(ppp_pcb *pcb, const u8_t *data, u32_t len, void *ctx);
void ppp_io_input(ppp_pcb *ppp, const u8_t *data, u32_t len);

#ifdef __cplusplus
}
#endif

#endif // PPP_IO_H
