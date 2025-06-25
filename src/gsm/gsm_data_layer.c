#include "gsm_data_layer.h"

// Lấy trạng thái PPP thực tế từ gsm.c
extern bool ppp_connected;

bool gsm_data_layer_is_ppp_connected(void) {
    return ppp_connected;
}
