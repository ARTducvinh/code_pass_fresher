#ifndef GSM_COMMANDS_H
#define GSM_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* command;       
    const char* syntax;        
    const char* response;      
    int max_response_time_ms;  
} gsm_command_t;

extern const gsm_command_t gsm_commands[];
extern const int gsm_commands_count;

#ifdef __cplusplus
}
#endif

#endif // GSM_COMMANDS_H
