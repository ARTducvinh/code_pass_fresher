#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void sensor_init(void);
uint8_t sensor_read(void);
void sensor_exti_callback(void);
#ifdef __cplusplus
}
#endif

#endif