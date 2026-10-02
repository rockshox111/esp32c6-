idf_component_register(
  SRCS
    main.c
    hw/lcd.c hw/bl.c hw/sd.c hw/input.c
    os/queue.c os/sysmon.c
    fs/file_mgr.c
    ui/ui.c ui/font.c ui/icons.c
    ui/screens/home.c ui/screens/files.c ui/screens/viewer.c ui/screens/sysmon.c
  INCLUDE_DIRS
    . hw os fs ui ui/screens
  REQUIRES
    driver gpio spi_flash nvs_flash fatfs sdmmc esp-timer lvgl esp_lcd esp_lcd_st7789
)
