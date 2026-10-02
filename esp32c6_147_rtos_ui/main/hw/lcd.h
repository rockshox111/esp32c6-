#pragma once
#include "esp_lcd_panel_handle.h"
#include "lvgl.h"
typedef struct { esp_lcd_panel_handle_t panel; lv_display_t *disp; void *buf1; } lcd_ctx_t;
esp_err_t lcd_init(lcd_ctx_t *out);
void lcd_backlight_set(int percent);
