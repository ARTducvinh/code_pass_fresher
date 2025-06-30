#include "ppp_connection.h"
#include "hardware/uart.h"
#include "lwip/tcpip.h"
#include "netif/ppp/pppos.h"
#include <string.h>
#include <stdio.h> // Thêm vào để dùng sprintf
#include "lwip/netif.h" // Thêm vào để truy cập thông tin netif
#include "lwip/ip_addr.h" // Thêm vào để dùng ipaddr_ntoa
#include "mqtt/mqtt.h" // Thêm dòng này ở đầu file
#include "net_test.h"
#include "timer.h" // Thêm vào để dùng sys_check_timeouts
// Thêm một biến cờ toàn cục
volatile bool ppp_connection_established = false;

// Định nghĩa các biến toàn cục
ppp_pcb *ppp = NULL;
volatile bool gsm_ppp_mode = false;

static struct netif ppp_netif;
static bool ppp_connected = false;

/**
 * @brief  Hàm wrapper cho việc gửi dữ liệu PPP qua UART.
 * @brief  Hàm này có prototype khớp với yêu cầu của pppos_create.
 * @param  pcb: PPP control block
 * @param  data: Dữ liệu cần gửi
 * @param  len: Độ dài dữ liệu
 * @param  ctx: Con trỏ ngữ cảnh (không dùng)
 * @retval Luôn trả về độ dài đã gửi.
 */
static u32_t ppp_output_callback(ppp_pcb *pcb, const void *data, u32_t len, void *ctx) {
    (void)pcb;
    (void)ctx;
    uart1_send_raw((const uint8_t*)data, len);
    return len;
}

static void ppp_status_cb(ppp_pcb *pcb, int err_code, void *ctx) {
    (void)pcb;
    (void)ctx;
    switch (err_code) {
        case PPPERR_NONE: {
            char log_buf[100];
            uart_log("PPP connected");
            ppp_connected = true;
            gsm_ppp_mode = true;

            // Đặt cờ để báo hiệu kết nối PPP đã được thiết lập
            ppp_connection_established = true;

            sprintf(log_buf, "   IP address:  %s", ipaddr_ntoa(netif_ip4_addr(&ppp_netif)));
            uart_log(log_buf);
            sprintf(log_buf, "   Gateway:     %s", ipaddr_ntoa(netif_ip4_gw(&ppp_netif)));
            uart_log(log_buf);
            sprintf(log_buf, "   Netmask:     %s", ipaddr_ntoa(netif_ip4_netmask(&ppp_netif)));
            uart_log(log_buf);

            // // Gọi các hàm kiểm tra sau khi kết nối thành công
            // uart_log("Running network tests...");
            // test_tcp_connect_google();
            // test_dns_resolution();
            // test_http_request();
            // uart_log("Network tests completed.");

            // Khởi tạo MQTT sau khi kết nối PPP thành công
            uart_log("Initializing MQTT...");
            mqtt_example_init();
            uart_log("MQTT initialization completed.");

            break;
        }
        case PPPERR_USER:
            uart_log("PPP connection terminated by user");
            ppp_connected = false;
            gsm_ppp_mode = false;
            ppp_connection_established = false;
            break;
        default:
            uart_log("PPP connection error");
            ppp_connected = false;
            gsm_ppp_mode = false;
            ppp_connection_established = false;
            break;
    }
}

void ppp_connection_init(void) {
    uart_log("Initializing PPP connection...");

    tcpip_init(NULL, NULL);

    // Sử dụng hàm wrapper ppp_output_callback
    ppp = pppos_create(&ppp_netif, ppp_output_callback, ppp_status_cb, NULL);
    if (ppp == NULL) {
        uart_log("Failed to create PPP control block");
        return;
    }

    ppp_set_default(ppp);
    // Đặt mật khẩu nếu cần
    // ppp_set_auth(ppp, PPPAUTHTYPE_PAP, "user", "password");
    ppp_connect(ppp, 0);
}

void ppp_connection_poll(void) {
    // Hàm này nên được gọi thường xuyên từ vòng lặp chính
    sys_check_timeouts();

    if (ppp_connected) {
        // Có thể thêm log ở đây nếu cần, nhưng hạn chế để tránh spam
        // uart_log("PPP connection is active");
    } else {
        // uart_log("PPP connection is not active");
    }
}

void ppp_connection_close(void) {
    if (ppp != NULL) {
        ppp_close(ppp, 0);
        ppp = NULL;
    }
}