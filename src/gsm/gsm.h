#ifndef GSM_H
#define GSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void send_gsm_command_by_index(int idx);
bool check_gsm_response_by_index(int idx, const char* response);
void send_all_gsm_commands_with_check(void);

#ifdef __cplusplus
}
#endif

#endif // GSM_H