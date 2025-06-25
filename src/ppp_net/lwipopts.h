#define LWIP_IPV4                 1
#define LWIP_IPV6                 0
#define LWIP_UDP                  0 // Nếu chỉ dùng TCP/MQTT
#define LWIP_TCP                  1
#define LWIP_RAW                  0
#define LWIP_NETCONN              0
#define LWIP_SOCKET               1 // Nếu dùng MQTT client dạng socket
#define PPP_SUPPORT               1
#define PPPOS_SUPPORT             1
#define PAP_SUPPORT               1 // Nếu dùng PAP
#define CHAP_SUPPORT              0
#define MSCHAP_SUPPORT            0
#define VJ_SUPPORT                0
#define LWIP_STATS                0
#define LWIP_DEBUG                0
#define MEMP_NUM_TCP_PCB          2
#define MEMP_NUM_TCP_SEG          8
#define TCP_MSS                   536     // hoặc giá trị bạn đang dùng
#define TCP_SND_BUF               (2 * TCP_MSS)   // hoặc lớn hơn
#define TCP_WND                   800   // Giảm xuống để phù hợp với PBUF_POOL_SIZE * (PBUF_POOL_BUFSIZE - header)
#define TCP_SND_QUEUELEN          4       // hoặc lớn hơn
#define TCP_SNDQUEUELOWAT         2       // phải nhỏ hơn TCP_SND_QUEUELEN
#define PBUF_POOL_SIZE            4
#define PBUF_POOL_BUFSIZE         256
#define IP_REASSEMBLY 0
#define IP_FRAG 0
#define LWIP_ARP 0
// ... và các option khác giảm tối đa