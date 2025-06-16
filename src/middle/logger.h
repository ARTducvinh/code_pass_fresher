#ifndef __LOGGER_H__
#define __LOGGER_H__

#ifdef __cplusplus
extern "C" {
#endif

void logger_init(void);
void logger_log(const char* msg);

#ifdef __cplusplus
}
#endif

#endif