#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DEVICE_STATE_BOOTING_GSM = 0,      
    DEVICE_STATE_GSM_READY = 1,       
    DEVICE_STATE_GSM_ERROR = 2,        
    DEVICE_STATE_BOOTING_DEVICE = 3,   
    DEVICE_STATE_DEVICE_READY = 4,     
    DEVICE_STATE_DEVICE_ERROR = 5,
} device_state_t;

typedef enum {
    swich_off = 0,
    swich_on = 1,
} swich_state_t;

extern device_state_t current_device_state;
extern swich_state_t switch_state;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */