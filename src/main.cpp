#include "hardware/hardware.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "gsm/gsm.h"
#include "gsm/gsm_data_layer.h"
#include "middle/led_logic.h" 
#include "ppp_net/ppp_net.h"
#include "lwip/tcpip.h"
#include <stdio.h>

int main(void)
{
    hardware_init();
    uart_log("UART2 log test");
    delay_ms(15000);
    send_all_gsm_commands_with_check();
    tcpip_init(NULL, NULL);
    ppp_net_init();
    while (1) {
        uart1_poll();
        ppp_net_poll();
    }
}
