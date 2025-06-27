#ifndef GSM_COMMANDS_H
#define GSM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

// Tổng hợp các lệnh và thời gian phản hồi

typedef struct {
    const char* command;       // Lệnh AT
    const char* syntax;        // Cú pháp
    const char* response;      // Phản hồi chính
    int max_response_time_ms;  // Thời gian phản hồi tối đa (ms)
} gsm_command_t;

extern const gsm_command_t gsm_commands[];
extern const int gsm_commands_count;

#ifdef __cplusplus
}
#endif

#endif // GSM_COMMANDS_H
