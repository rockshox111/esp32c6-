#pragma once
#include <stdbool.h>
bool sd_mount(void);
void sd_unmount(void);
bool sd_dir_list(const char *path, char names[][64], uint32_t sizes[], uint8_t isdir[], int *count, int max);
