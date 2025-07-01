// Hệ thống
#define NO_SYS                    0
#define SYS_LIGHTWEIGHT_PROT      1

// IPv4 và IPv6
#define LWIP_IPV4                 1
#define LWIP_IPV6                 0 // Tắt IPv6 nếu không sử dụng

// ARP (không cần với PPP)
#define LWIP_ARP                  0

// IP Reassembly và Fragmentation
#define IP_REASSEMBLY             0  // Tắt IP reassembly
#define IP_FRAG                   0  // Tắt IP fragmentation

// TCP
#define LWIP_TCP                  1
#define TCP_MSS                   1460 // Maximum Segment Size (Ethernet MTU - IP/TCP headers)
#define TCP_SND_BUF               (2 * TCP_MSS) // Giảm kích thước bộ nhớ gửi TCP
#define TCP_WND                   TCP_MSS // Đặt cửa sổ TCP bằng kích thước MSS
#define TCP_SND_QUEUELEN          (4 * TCP_SND_BUF / TCP_MSS)
#define MEMP_NUM_TCP_SEG          8   // Giảm số lượng TCP segment
#define TCP_SNDQUEUELOWAT         4
#define LWIP_DISABLE_TCP_SANITY_CHECKS  1

// UDP
#define LWIP_UDP                  0 // Tắt UDP nếu không sử dụng

// RAW sockets
#define LWIP_RAW                  0 // Tắt RAW sockets nếu không sử dụng

// Netconn và Socket API
#define LWIP_NETCONN              0 // Tắt Netconn API nếu không sử dụng
#define LWIP_SOCKET               0 // Tắt Socket API nếu không sử dụng

// Bộ nhớ đệm
#define PBUF_POOL_SIZE            6   // Tăng số lượng bộ nhớ đệm
#define PBUF_POOL_BUFSIZE         512 // Giữ nguyên kích thước mỗi gói

// DNS
#define LWIP_DNS                  0 // Tắt DNS nếu không sử dụng

// PPP
#define PPP_SUPPORT               1
#define PPPOS_SUPPORT             1
#define PAP_SUPPORT               1
#define CHAP_SUPPORT              0 // Tắt CHAP nếu không sử dụng
#define MSCHAP_SUPPORT            0 // Tắt MSCHAP nếu không sử dụng
#define VJ_SUPPORT                0 // Tắt Van Jacobson TCP/IP header compression nếu không sử dụng

// IGMP (Multicast)
#define LWIP_IGMP                 0 // Tắt IGMP nếu không sử dụng

// ICMP
#define LWIP_ICMP                 0 // Tắt ICMP nếu không sử dụng

// IP Forwarding
#define IP_FORWARD                0 // Tắt IP Forwarding nếu không sử dụng

// Debug
#define LWIP_DEBUG                0 // Tắt debug để giảm dung lượng flash

// HTTPD, FTP, Telnet
#define LWIP_HTTPD                0 // Tắt nếu không cần
#define LWIP_FTP                  0 // Tắt nếu không cần
#define LWIP_TELNET               0 // Tắt nếu không cần

// SNMP và mDNS
#define LWIP_SNMP                 0 // Tắt nếu không cần
#define LWIP_MDNS_RESPONDER       0 // Tắt nếu không cần