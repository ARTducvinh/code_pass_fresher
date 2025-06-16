#include "hardware/hardware.h"
#include "middle/button_logic.h"
#include "middle/led_logic.h"
#include "middle/logger.h"

int main(void)
{
    hardware_init();
    logger_log("System started");
    while (1)
    {
        button_logic_update();    
        button_logic_process();   
        switch (led_4g_mode)
        {
            case 0:
                break;
            case 1:
                led_logic_blink_200_200();
                break;
            case 2:
                led_logic_blink_200_500_200_2000();
                break;
        }
    }
}
