#include "hardware/hardware.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "gsm/gsm.h"
#include "gsm/gsm_data_layer.h"
#include "middle/led_logic.h" 
#include "ppp_net/ppp_connection.h"
#include "lwip/tcpip.h"
#include <stdio.h>

int main(void)
{
    hardware_init();
    uart_log("UART2 log test");
    delay_ms(15000);
    ppp_connection_init();

    while (1) {
        uart1_poll();
        send_all_gsm_commands_with_check();
        ppp_connection_poll();
    }
}
