#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>
#include "hardware.h"

#define BUTTON_PIN        BTN_1_PIN
#define BUTTON_PORT       BTN_1_PORT

#ifdef __cplusplus
extern "C" {
#endif

void button_init(void);
uint8_t button_read_pin(void); // Đọc trạng thái nút (0: nhấn, 1: thả)

#ifdef __cplusplus
}
#endif

#endif