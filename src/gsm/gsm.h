// gsm.h
#ifndef GSM_H
#define GSM_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

//các trạng thái cần thiết cho PPP
typedef enum {
    GSM_STATE_POWER_OFF,
    GSM_STATE_POWER_ON,
    GSM_STATE_WAIT_READY,
    GSM_STATE_AT_OK,
    GSM_STATE_SIGNAL_OK,
    GSM_STATE_APN_SET,
    GSM_STATE_DIALING,
    GSM_STATE_PPP_MODE,
    GSM_STATE_ERROR
} gsm_state_t;

void gsm_init(void);
void gsm_process(void);
gsm_state_t gsm_get_state(void);
void gsm_send_command(const char* cmd);

#ifdef __cplusplus
}
#endif

#endif // GSM_H
