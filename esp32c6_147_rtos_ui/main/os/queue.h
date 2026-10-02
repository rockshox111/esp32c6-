#pragma once
#include <stdint.h>
#define SYS_Q_LEN 4
typedef struct {
    uint32_t heap_free;
    uint32_t heap_min;
    char     task_stats[512];
} sys_snapshot_t;
extern QueueHandle_t g_sys_q;
void os_queues_create(void);
