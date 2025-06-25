#ifndef GSM_PARSER_H
#define GSM_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool gsm_parser_is_empty_line(const char* resp);
bool gsm_parser_is_ok(const char* resp);
bool gsm_parser_is_sim_error(const char* resp);
bool gsm_parser_is_error(const char* resp);
bool gsm_parser_is_connect(const char* resp);
bool gsm_parser_has_prefix(const char* resp, const char* prefix);

#ifdef __cplusplus
}
#endif

#endif // GSM_PARSER_H