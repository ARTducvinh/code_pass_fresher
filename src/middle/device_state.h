#ifndef DEVICE_STATE_H
#define DEVICE_STATE_H

// Trạng thái của thiết bị
typedef enum {
    DEVICE_STATE_BOOTING_GSM,      
    DEVICE_STATE_GSM_READY,         
    DEVICE_STATE_INTERNET_ERROR,    
    DEVICE_STATE_CONNECTING_BROKER, 
    DEVICE_STATE_BROKER_CONNECTED,  
    DEVICE_STATE_BROKER_ERROR       
} device_state_t;
extern device_state_t current_device_state;

#endif /* DEVICE_STATE_H */