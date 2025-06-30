#define NO_SYS                    0
#define SYS_LIGHTWEIGHT_PROT      1

#define LWIP_IPV4                 1
#define LWIP_IPV6                 0

#define LWIP_ARP                  0 // Không dùng với PPP
#define IP_REASSEMBLY             0

#define LWIP_TCP                  1
#define LWIP_UDP                  1  // Bật hỗ trợ UDP
#define LWIP_RAW                  0

#define LWIP_NETCONN              1  // Bật hỗ trợ Netconn API
#define LWIP_SOCKET               1

#define LWIP_STATS                0
#define LWIP_DEBUG                0

// PPP
#define PPP_SUPPORT               1
#define PPPOS_SUPPORT             1
#define PAP_SUPPORT               1
#define CHAP_SUPPORT              1
#define MSCHAP_SUPPORT            0
#define VJ_SUPPORT                0

#define MEMP_NUM_TCP_PCB          5
#define PBUF_POOL_SIZE            16
#define PBUF_POOL_BUFSIZE         512

#define TCP_MSS                   536
#define TCP_SND_BUF               (4 * TCP_MSS)
#define TCP_WND                   (4 * TCP_MSS)
#define TCP_SND_QUEUELEN          (2 * TCP_SND_BUF / TCP_MSS)
#define TCP_SNDQUEUELOWAT         2

#define LWIP_MD5_SUPPORT          1

// Bật debug cho PPP và TCP/IP (giúp xem log chi tiết nếu cần)
#define PPP_DEBUG                 LWIP_DBG_ON
#define TCP_DEBUG                 LWIP_DBG_ON
#define SOCKETS_DEBUG             LWIP_DBG_ON

// Bật hỗ trợ fcntl (nếu lwIP version hỗ trợ)
#define LWIP_FCNTL_API            1

// Bật hỗ trợ SO_ERROR cho getsockopt (giúp kiểm tra lỗi khi non-blocking connect)
#define LWIP_SO_SNDTIMEO          1
#define LWIP_SO_RCVTIMEO          1
#define LWIP_SO_RCVBUF            1
#define LWIP_SO_SNDBUF            1

// Bật debug toàn cục nếu cần
//#define LWIP_DEBUG                1

// Đảm bảo các macro này không bị định nghĩa lại ở nơi khác

#define LWIP_DNS                  1  // Bật hỗ trợ DNS
