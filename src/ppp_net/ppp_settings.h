#ifndef __PPP_SETTINGS_H__
#define __PPP_SETTINGS_H__

// ===================================================================
// PPP Feature Support
// ===================================================================

#define EAP_SUPPORT           0   // Tắt Extensible Authentication Protocol
#define LCP_ECHO_SUPPORT      1   // Bật LCP Echo-Request/Echo-Reply
#define PPP_IPV4_SUPPORT      1   // Bật hỗ trợ IPv4
#ifndef PPP_IPV6_SUPPORT
#define PPP_IPV6_SUPPORT      0   // Tắt hỗ trợ IPv6
#endif

// ===================================================================
// PPP Configuration Options
// ===================================================================

/**
 * PPP_INPROC_OWN_THREAD
 *
 * Create a dedicated thread for PPP input processing.
 * This is the recommended setting for most systems.
 * If set to 0, you must call ppp_input() from your main loop
 * or a driver thread.
 */
#define PPP_INPROC_OWN_THREAD   0   // Chúng ta sẽ gọi pppos_input từ uart_poll

/**
 * MEMP_NUM_PPP_PCB
 *
 * Number of PPP control blocks.
 */
#define MEMP_NUM_PPP_PCB        1

/**
 * PPP_MAX_OPTIONS
 *
 * Maximum number of LCP/IPCP options per negotiation.
 */
#define PPP_MAX_OPTIONS         6

/**
 * PPP_MAX_FAILURE
 *
 * Maximum number of configure-nak loops.
 */
#define PPP_MAX_FAILURE         5

#endif /* __PPP_SETTINGS_H__ */