#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/err.h"
#include "arch/sys_arch.h"

void sys_init(void) {}

err_t sys_mutex_new(sys_mutex_t *mutex) { *mutex = 0; return ERR_OK; }
void sys_mutex_lock(sys_mutex_t *mutex) { (void)mutex; }
void sys_mutex_unlock(sys_mutex_t *mutex) { (void)mutex; }
void sys_mutex_free(sys_mutex_t *mutex) { (void)mutex; }

err_t sys_sem_new(sys_sem_t *sem, u8_t count) { *sem = 0; (void)count; return ERR_OK; }
void sys_sem_signal(sys_sem_t *sem) { (void)sem; }
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout) { (void)sem; (void)timeout; return 0; }
void sys_sem_free(sys_sem_t *sem) { (void)sem; }

err_t sys_mbox_new(sys_mbox_t *mbox, int size) { *mbox = 0; (void)size; return ERR_OK; }
void sys_mbox_post(sys_mbox_t *mbox, void *msg) { (void)mbox; (void)msg; }
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg) { (void)mbox; (void)msg; return ERR_OK; }
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout) { (void)mbox; (void)msg; (void)timeout; return 0; }
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg) { (void)mbox; (void)msg; return 0; }
void sys_mbox_free(sys_mbox_t *mbox) { (void)mbox; }

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio) { (void)name; (void)thread; (void)arg; (void)stacksize; (void)prio; return 0; }
