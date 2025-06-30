#include "gsm_commands.h"

const gsm_command_t gsm_commands[] = {
    {"AT", "AT", "OK", 1000},                        // Kiểm tra giao tiếp
    {"ATI", "ATI", "Manufacturer: SIMCOM INCORPORATED", 1000}, // Lấy thông tin module
    {"AT+CPIN?", "AT+CPIN?", "+CPIN: READY", 1000},  // Kiểm tra trạng thái SIM
    {"AT+CSQ", "AT+CSQ", "+CSQ: <rssi>,<ber>", 1000}, // Kiểm tra chất lượng tín hiệu
    {"AT+CEREG?", "AT+CEREG?", "+CEREG: <n>,<stat>", 20000}, // Kiểm tra đăng ký mạng LTE
    {"AT+COPS", "AT+COPS=0", "+COPS: 0", 60000},     // Chọn mạng tự động
    {"AT+CGDCONT", "AT+CGDCONT=1,\"IP\",\"v-internet\"", "OK", 9000}, // Cấu hình PDP với APN mobifone
    {"AT+CGAUTH", "AT+CGAUTH=1,0", "OK", 9000},      // Không sử dụng tài khoản/mật khẩu
    {"AT+CGACT", "AT+CGACT=1,1", "OK", 150000},      // Kích hoạt PDP context
    {"AT+CGPADDR", "AT+CGPADDR=1", "+CGPADDR: 1,\"<PDP_addr>\"", 5000}, // Kiểm tra địa chỉ IP
    {"AT+CGDATA", "ATD*99#", "CONNECT", 150000}, // Khởi tạo PPP mode bằng ATD*99#
};

const int gsm_commands_count = sizeof(gsm_commands) / sizeof(gsm_commands[0]);