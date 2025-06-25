#include "hardware/hardware.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "gsm/gsm.h"
#include "gsm/gsm_data_layer.h"
#include "middle/led_logic.h" 
#include "ppp_net/ppp_net.h"
#include "lwip/init.h"
#include <stdio.h> 


// Thêm khai báo trạng thái PPP
extern ppp_net_state_t ppp_state;

int main(void)
{
    hardware_init();
    uart_init_all();
    timer2_init_1ms();
    gsm_init();
    uart_log("UART2 log test");
    delay_ms(10000);
    lwip_init();

    gsm_state_t last_state = GSM_STATE_POWER_OFF;
    bool ppp_started = false;

    while (1) {
        // Xử lý trạng thái GSM
        gsm_process();
        led_4g_update_logic();

        // Thêm log để kiểm tra trạng thái GSM
        char gsm_state_log[64];
        sprintf(gsm_state_log, "GSM State: %d", gsm_get_state());
        uart_log(gsm_state_log);

        // Khi GSM đã quay số PPP thành công, khởi tạo PPP nếu chưa khởi tạo
        if (gsm_data_layer_is_ppp_connected() && !ppp_started) {
            uart_log("PPP: GSM bao da ket noi, bat dau khoi tao PPP...");
            ppp_net_init();
            ppp_started = true;
            uart_log("PPP started");
        }

        // Xử lý PPP định kỳ
        if (ppp_started) {
            ppp_net_poll();
            if (ppp_net_is_connected()) {
                struct netif* pppif = ppp_net_get_netif();
                if (pppif && !ip4_addr_isany_val(*netif_ip4_addr(pppif))) {
                    char ipbuf[16];
                    sprintf(ipbuf, "%s", ip4addr_ntoa(netif_ip4_addr(pppif)));
                    uart_log("PPP: Da co IP, PPP hoat dong OK!");
                    uart_log(ipbuf);
                    // Có thể tick MQTT ở đây
                } else {
                    uart_log("PPP: Chua co IP, dang cho ket noi...");
                }
            } else {
                uart_log("PPP: Chua co IP, dang cho ket noi...");
            }
        }

        // Thêm log để kiểm tra trạng thái PPP
        if (ppp_started) {
            char ppp_state_log[64];
            sprintf(ppp_state_log, "PPP State: %d", ppp_state);
            uart_log(ppp_state_log);
        }
    }
}