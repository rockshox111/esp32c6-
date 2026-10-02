#include "lcd.h"
#include "board_pin.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

static const char *TAG = "LCD";

static void lcd_flush_cb(lv_display_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    lcd_ctx_t *c = lv_display_get_user_data(disp);
    esp_lcd_panel_draw_bitmap(c->panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, color_p);
    lv_display_flush_ready(disp);
}

esp_err_t lcd_init(lcd_ctx_t *out)
{
    spi_bus_config_t bus = {
        .mosi_io_num = PIN_LCD_MOSI, .miso_io_num = -1, .sclk_io_num = PIN_LCD_SCLK,
        .quadwp_io_num = -1, .quadhd_io_num = -1,
        .max_transfer_sz = LCD_W * LV_BUF_ROWS * 2,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &bus, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_spi_config_t io = {
        .cs_gpio_num = PIN_LCD_CS, .dc_gpio_num = PIN_LCD_DC, .spi_mode = 0,
        .pclk_hz = LCD_PCLK_HZ, .trans_queue_depth = 10,
        .buffer_size = LCD_W * LV_BUF_ROWS * 2,
    };
    esp_lcd_panel_io_handle_t io_h;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io, &io_h));

    esp_lcd_panel_dev_config_t dev = {
        .reset_gpio_num = PIN_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_h, &dev, &out->panel));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(out->panel));
    ESP_ERROR_CHECK(esp_lcd_panel_init(out->panel));
    esp_lcd_panel_invert_color(out->panel, true);
    esp_lcd_panel_set_gap(out->panel, 0, 0);

    size_t buf_sz = LCD_W * LV_BUF_ROWS * sizeof(lv_color_t);
    out->buf1 = heap_caps_malloc(buf_sz, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    assert(out->buf1);

    lv_init();
    out->disp = lv_display_create(LCD_W, LCD_H);
    lv_display_set_user_data(out->disp, out);
    lv_display_set_buffers(out->disp, out->buf1, NULL, buf_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(out->disp, lcd_flush_cb);
    ESP_LOGI(TAG, "LCD init ok");
    return ESP_OK;
}
