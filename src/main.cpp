#include "hardware/hardware.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "gsm/gsm.h"
#include "middle/led_logic.h"
#include "ppp_net/ppp_connection.h"
#include "ppp_net/net_test.h"
#include "lwip/tcpip.h"
#include "main.h"
#include "middle/button_logic.h"
#include "hardware/button.h"

device_state_t current_device_state = DEVICE_STATE_BOOTING_GSM;
swich_state_t switch_state = swich_off;
int main(void)
{
    hardware_init();
    uart_log("UART2 log test");
    uint32_t start_tick = timer2_get_tick();
    while (timer2_get_tick() - start_tick < 15000) {
        led_update_by_device_state();
        delay_ms(10);
    }
    ppp_connection_init();
    //led_stt_switch_set_state(1); 
    while (1) {      
        led_update_by_device_state();
        led_stt_switch_control();
        button_logic_update();
        uart1_poll();
        send_all_gsm_commands_with_check();
        ppp_connection_poll();
    }
}

