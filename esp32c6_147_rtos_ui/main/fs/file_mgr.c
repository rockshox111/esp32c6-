#include "file_mgr.h"
#include "board_pin.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

bool fm_list(const char *dir, fm_item_t *items, int *count, int max)
{
    *count = 0;
    DIR *d = opendir(dir);
    if (!d) return false;
    struct dirent *e;
    while ((e = readdir(d)) && *count < max) {
        char full[160];
        snprintf(full, sizeof(full), "%s/%s", dir, e->d_name);
        struct stat st;
        if (stat(full, &st) == 0) { items[*count].size = (uint32_t)st.st_size; items[*count].is_dir = S_ISDIR(st.st_mode) ? 1 : 0; }
        else { items[*count].size = 0; items[*count].is_dir = 0; }
        strncpy(items[*count].name, e->d_name, 63);
        (*count)++;
    }
    closedir(d);
    return true;
}

bool fm_read_text(const char *path, char *out, int outlen)
{
    FILE *f = fopen(path, "r");
    if (!f) return false;
    int n = fread(out, 1, outlen - 1, f);
    out[n < 0 ? 0 : n] = 0;
    fclose(f);
    return true;
}

bool fm_write_line(const char *path, const char *line)
{
    FILE *f = fopen(path, "a");
    if (!f) return false;
    fprintf(f, "%s\n", line);
    fclose(f);
    return true;
}

bool fm_mkdir(const char *path) { return mkdir(path, 0777) == 0; }
bool fm_delete(const char *path) { return unlink(path) == 0; }
