#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint8_t g_button_pressed_flag;
extern volatile uint8_t g_button_released_flag;
void button_init(void);
#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */