#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "hardware/uart.h"


void test_tcp_connect_google(void) {
    char log_buf[128];
    uart_log("Start test_tcp_connect_google");

    struct tcp_pcb *pcb = tcp_new();
    if (pcb == NULL) {
        uart_log("Failed to create TCP PCB");
        return;
    }

    ip_addr_t google_ip;
    IP4_ADDR(&google_ip, 142, 250, 190, 46); // IP của google.com

    err_t err = tcp_connect(pcb, &google_ip, 80, NULL); // Không cần callback cho test đơn giản
    if (err != ERR_OK) {
        snprintf(log_buf, sizeof(log_buf), "TCP Connect failed: %d", err);
        uart_log(log_buf);
    } else {
        uart_log("TCP Connected to google.com:80!");
    }

    tcp_abort(pcb); // Đóng kết nối sau khi test
}

void test_dns_resolution(void) {
    char log_buf[128];
    uart_log("Start DNS resolution test");

    ip_addr_t resolved_ip;
    err_t dns_err = dns_gethostbyname("google.com", &resolved_ip, NULL, NULL);
    if (dns_err == ERR_OK) {
        snprintf(log_buf, sizeof(log_buf), "DNS resolved: %s", ipaddr_ntoa(&resolved_ip));
        uart_log(log_buf);
    } else {
        snprintf(log_buf, sizeof(log_buf), "DNS resolution failed: %d", dns_err);
        uart_log(log_buf);
    }
}

void test_http_request(void) {
    char log_buf[128];
    uart_log("Start HTTP request test");

    struct tcp_pcb *pcb = tcp_new();
    if (pcb == NULL) {
        uart_log("Failed to create TCP PCB");
        return;
    }

    ip_addr_t google_ip;
    IP4_ADDR(&google_ip, 142, 250, 190, 46); // IP của google.com

    err_t err = tcp_connect(pcb, &google_ip, 80, NULL);
    if (err != ERR_OK) {
        snprintf(log_buf, sizeof(log_buf), "TCP Connect failed: %d", err);
        uart_log(log_buf);
        tcp_abort(pcb);
        return;
    }

    char http_request[] = "GET / HTTP/1.1\r\nHost: google.com\r\n\r\n";
    err = tcp_write(pcb, http_request, strlen(http_request), TCP_WRITE_FLAG_COPY);
    if (err == ERR_OK) {
        uart_log("HTTP request sent successfully!");
    } else {
        snprintf(log_buf, sizeof(log_buf), "HTTP request send failed: %d", err);
        uart_log(log_buf);
    }

    tcp_abort(pcb); // Đóng kết nối sau khi test
}