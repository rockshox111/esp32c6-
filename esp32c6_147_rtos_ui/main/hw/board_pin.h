#pragma once
#include "driver/gpio.h"

#define LCD_W        172
#define LCD_H        320
#define PIN_LCD_MOSI 6
#define PIN_LCD_SCLK 7
#define PIN_LCD_CS   14
#define PIN_LCD_DC   15
#define PIN_LCD_RST  21
#define PIN_LCD_BL   22
#define LCD_SPI_HOST SPI2_HOST
#define LCD_PCLK_HZ  40000000

#define PIN_SD_MISO  5
#define PIN_SD_MOSI  6
#define PIN_SD_SCLK  7
#define PIN_SD_CS    4
#define SD_SPI_HOST  SPI2_HOST
#define SD_MOUNT     "/sdcard"

#define PIN_KEY_OK   9
#define PIN_KEY_UP   10
#define PIN_KEY_DOWN 11
#define PIN_KEY_BACK 8

#define LV_BUF_ROWS  40
