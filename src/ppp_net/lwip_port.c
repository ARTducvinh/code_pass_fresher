#include <stdint.h>
#include <stdio.h>

// Assertion handler for LwIP
void lwip_example_app_platform_assert(const char *msg) {
    // You can replace this with your own error handler or LED blink, etc.
    (void)msg;
    while (1) { }
}

// Diagnostic output for LwIP (optional)
void lwip_win32_platform_diag(const char *msg) {
    // Optionally send to UART or ignore
    (void)msg;
}

// Core locking check (no-op for baremetal/single-thread)
void sys_check_core_locking(void) {
    // No-op for baremetal
}

// Returns system time in ms (replace with your own timer if needed)
uint32_t sys_now(void) {
    // TODO: Thay thế bằng hàm trả về thời gian thực tế nếu bạn có timer
    static uint32_t fake_tick = 0;
    return fake_tick++;
}

// Returns system time in ticks (can be same as sys_now)
uint32_t sys_jiffies(void) {
    return sys_now();
}

// Returns a random 32-bit value (replace with your own RNG if needed)
uint32_t lwip_port_rand(void) {
    static uint32_t seed = 123456789;
    seed = seed * 1103515245 + 12345;
    return seed;
}

// --- LwIP baremetal porting functions ---

// sys_arch_protect/sys_arch_unprotect: No real protection needed for single-thread
unsigned long sys_arch_protect(void) {
    return 0;
}
void sys_arch_unprotect(unsigned long pval) {
    (void)pval;
}

// Đã xoá các hàm sys_mutex_lock, sys_mutex_unlock, sys_mbox_trypost để tránh trùng với sys_arch.c
