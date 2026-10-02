#include "sysmon.h"
#include "queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "SYSMON";

void sysmon_task(void *arg)
{
    char buf[640];
    while (1) {
        vTaskGetRunTimeStats(buf);
        sys_snapshot_t s;
        s.heap_free = esp_get_free_heap_size();
        s.heap_min  = esp_get_minimum_free_heap_size();
        strncpy(s.task_stats, buf, sizeof(s.task_stats) - 1);
        s.task_stats[sizeof(s.task_stats) - 1] = 0;
        xQueueOverwrite(g_sys_q, &s);
        ESP_LOGI(TAG, "free=%u min=%u", s.heap_free, s.heap_min);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
