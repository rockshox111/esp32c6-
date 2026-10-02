#include "viewer.h"
#include "file_mgr.h"
#include <string.h>

void viewer_open(const char *path, lv_group_t *group)
{
    lv_obj_t *scr = lv_scr_create();
    lv_scr_load(scr);
    lv_obj_t *tb = lv_textarea_create(scr);
    lv_obj_set_size(tb, 168, 290);
    lv_obj_align(tb, LV_ALIGN_TOP_MID, 0, 6);
    lv_textarea_set_text(tb, "加载中...");
    lv_group_add_obj(group, tb);

    char buf[2048];
    if (fm_read_text(path, buf, sizeof(buf)))
        lv_textarea_set_text(tb, buf);
    else
        lv_textarea_set_text(tb, "无法打开/非文本");
}
