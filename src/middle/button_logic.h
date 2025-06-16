#ifndef __BUTTON_LOGIC_H__
#define __BUTTON_LOGIC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void button_logic_init(void);
void button_logic_update(void);
void button_logic_process(void);

extern volatile uint8_t led_4g_mode;
extern volatile uint8_t led_4g_state;

#ifdef __cplusplus
}
#endif

#endif