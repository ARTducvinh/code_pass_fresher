#ifndef GSM_STATE_H
#define GSM_STATE_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

bool check_gsm_response_by_index(int idx, const char* response);

#ifdef __cplusplus
}
#endif

#endif // GSM_STATE_H