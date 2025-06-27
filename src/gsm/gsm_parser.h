#ifndef GSM_PARSER_H
#define GSM_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Hàm phân tích phản hồi cho lệnh "AT"
bool parse_response_at(const char* response);

// Hàm phân tích phản hồi cho lệnh "ATI"
bool parse_response_ati(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CPIN?"
bool parse_response_at_cpin(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CSQ"
bool parse_response_at_csq(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CEREG?"
bool parse_response_at_cereg(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CGDCONT"
bool parse_response_at_cgdcont(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CGAUTH"
bool parse_response_at_cgauth(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CGACT"
bool parse_response_at_cgact(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CGPADDR"
bool parse_response_at_cgpaddr(const char* response);

// Hàm phân tích phản hồi cho lệnh "ATD*99#"
bool parse_response_atd99(const char* response);

// Hàm phân tích phản hồi cho lệnh "AT+CGDATA"
bool parse_response_at_cgdata(const char* response);

// // Hàm phân tích phản hồi cho lệnh "+++"
// bool parse_response_plus(const char* response);

// // Hàm phân tích phản hồi cho lệnh "ATH"
// bool parse_response_ath(const char* response);

#ifdef __cplusplus
}
#endif

#endif // GSM_PARSER_H