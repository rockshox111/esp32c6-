#include "board_pin.h"
#include "driver/ledc.h"
void lcd_backlight_set(int percent)
{
    if (percent < 0) percent = 0;
    if (percent > 50) percent = 50;
    ledc_timer_config_t t = {
        .speed_mode = LEDC_LOW_SPEED_MODE, .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = LEDC_TIMER_0, .freq_hz = 5000, .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&t);
    ledc_channel_config_t ch = {
        .gpio_num = PIN_LCD_BL, .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0, .timer_sel = LEDC_TIMER_0,
        .duty = (uint32_t)((percent * 255) / 100),
    };
    ledc_channel_config(&ch);
}
