#include "files.h"
#include "file_mgr.h"
#include "viewer.h"
#include <string.h>

static char cur_dir[128] = SD_MOUNT;
static fm_item_t items[FM_MAX];
static int count = 0;
static lv_obj_t *list;

static void refresh_list(lv_group_t *group)
{
    fm_list(cur_dir, items, &count, FM_MAX);
    lv_obj_clean(list);
    for (int i = 0; i < count; i++) {
        lv_obj_t *btn = lv_list_add_btn(list, NULL, items[i].name);
        lv_obj_add_event_cb(btn, item_cb, LV_EVENT_CLICKED, (void *)(intptr_t)i);
        lv_group_add_obj(group, btn);
    }
}

static void item_cb(lv_event_t *e)
{
    int idx = (int)(intptr_t)lv_event_get_user_data(e);
    if (idx < 0 || idx >= count) return;
    char full[192];
    snprintf(full, sizeof(full), "%s/%s", cur_dir, items[idx].name);
    if (items[idx].is_dir) {
        strncpy(cur_dir, full, sizeof(cur_dir) - 1);
        refresh_list(lv_group_get_default());
    } else {
        viewer_open(full, lv_group_get_default());
    }
}

static void back_cb(lv_event_t *e)
{
    lv_obj_t *scr = lv_scr_create();
    lv_scr_load(scr);
}

void files_screen_create(lv_group_t *group)
{
    lv_obj_t *scr = lv_scr_create();
    lv_scr_load(scr);

    lv_obj_t *back = lv_btn_create(scr);
    lv_obj_align(back, LV_ALIGN_TOP_LEFT, 4, 4);
    lv_obj_t *bl = lv_label_create(back);
    lv_label_set_text(bl, "返回");
    lv_group_add_obj(group, back);
    lv_obj_add_event_cb(back, back_cb, LV_EVENT_CLICKED, NULL);

    list = lv_list_create(scr);
    lv_obj_set_size(list, 168, 270);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 34);
    refresh_list(group);
}
