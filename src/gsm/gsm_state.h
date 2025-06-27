#ifndef GSM_STATE_H
#define GSM_STATE_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GSM_STATE_INITIALIZING,  
    GSM_STATE_NETWORK_READY, 
    GSM_STATE_CONNECTED      
} gsm_state_t;

// Biến kiểm tra mode PPP
extern bool ppp_mode;
#ifdef __cplusplus
}
#endif

#endif // GSM_STATE_H