#ifndef GSM_STATE_H
#define GSM_STATE_H

#include "gsm.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Hàm xử lý trạng thái dựa trên phản hồi
gsm_state_t gsm_state_handle_response(gsm_state_t current, const char* resp);

#ifdef __cplusplus
}
#endif

#endif // GSM_STATE_H