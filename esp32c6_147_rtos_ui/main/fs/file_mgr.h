#pragma once
#include <stdbool.h>
#define FM_MAX 16
typedef struct { char name[64]; uint32_t size; uint8_t is_dir; } fm_item_t;
bool fm_list(const char *dir, fm_item_t *items, int *count, int max);
bool fm_read_text(const char *path, char *out, int outlen);
bool fm_write_line(const char *path, const char *line);
bool fm_mkdir(const char *path);
bool fm_delete(const char *path);
