#include "sd.h"
#include "board_pin.h"
#include "driver/spi_master.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_log.h"

static const char *TAG = "SD";
static sdmmc_card_t *card;

bool sd_mount(void)
{
    spi_bus_config_t bus = {
        .mosi_io_num = PIN_SD_MOSI, .miso_io_num = PIN_SD_MISO, .sclk_io_num = PIN_SD_SCLK,
        .quadwp_io_num = -1, .quadhd_io_num = -1, .max_transfer_sz = 8192,
    };
    esp_err_t r = spi_bus_initialize(SD_SPI_HOST, &bus, SPI_DMA_CH_AUTO);
    if (r != ESP_OK && r != ESP_ERR_INVALID_STATE)
        ESP_LOGE(TAG, "spi_bus_init:%s", esp_err_to_name(r));

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SD_SPI_HOST;
    sdspi_device_config_t slot = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot.gpio_cs = PIN_SD_CS;
    slot.host_id = SD_SPI_HOST;

    esp_vfs_fat_mount_config_t cfg = { .format_if_mount_failed = false, .max_files = 8, .allocation_unit_size = 16384 };
    r = esp_vfs_fat_sdspi_mount(SD_MOUNT, &host, &slot, &cfg, &card);
    if (r == ESP_OK) {
        ESP_LOGI(TAG, "SD ok %lluMB", ((uint64_t)card->csd.capacity) >> 20);
        return true;
    }
    ESP_LOGE(TAG, "SD mount fail %s", esp_err_to_name(r));
    return false;
}

void sd_unmount(void) { esp_vfs_fat_sdcard_unmount(SD_MOUNT, card); }

bool sd_dir_list(const char *path, char names[][64], uint32_t sizes[], uint8_t isdir[], int *count, int max)
{
    *count = 0;
    DIR *d = opendir(path);
    if (!d) return false;
    struct dirent *e;
    while ((e = readdir(d)) && *count < max) {
        char full[128];
        snprintf(full, sizeof(full), "%s/%s", path, e->d_name);
        struct stat st;
        if (stat(full, &st) == 0) { sizes[*count] = (uint32_t)st.st_size; isdir[*count] = S_ISDIR(st.st_mode) ? 1 : 0; }
        else { sizes[*count] = 0; isdir[*count] = 0; }
        strncpy(names[*count], e->d_name, 63);
        (*count)++;
    }
    closedir(d);
    return true;
}
