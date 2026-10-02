#include "queue.h"
#include "freertos/queue.h"
QueueHandle_t g_sys_q;
void os_queues_create(void) { g_sys_q = xQueueCreate(SYS_Q_LEN, sizeof(sys_snapshot_t)); }
