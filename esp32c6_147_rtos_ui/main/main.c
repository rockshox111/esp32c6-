#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "lvgl.h"
#include "hw/lcd.h"
#include "hw/sd.h"
#include "hw/input.h"
#include "hw/bl.h"
#include "os/queue.h"
#include "os/sysmon.h"
#include "ui/ui.h"

static lcd_ctx_t lcd;

void gui_task(void *arg)
{
    lv_group_t *g;
    input_init(&g);
    ui_init(g);
    while (1) {
        ui_sys_update();
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    nvs_flash_init();
    os_queues_create();
    lcd_init(&lcd);
    lcd_backlight_set(40);
    sd_mount();
    xTaskCreate(sysmon_task, "sysmon", 4096, NULL, 5, NULL);
    xTaskCreate(gui_task, "gui", 8192, NULL, 6, NULL);
}
