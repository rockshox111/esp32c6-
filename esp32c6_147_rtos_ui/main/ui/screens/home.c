#include "home.h"
#include "files.h"
#include "sysmon.h"
#include "icons.h"
#include <string.h>

static void event_cb(lv_event_t *e)
{
    lv_obj_t *t = lv_event_get_target(e);
    lv_obj_t *lbl = lv_obj_get_child(t, 0);
    if (!lbl) return;
    const char *txt = lv_label_get_text(lbl);
    if (!strcmp(txt, "文件")) files_screen_create(lv_group_get_default());
    else if (!strcmp(txt, "系统")) sysmon_screen_create(lv_group_get_default());
}

void home_screen_create(lv_group_t *group)
{
    lv_obj_t *scr = lv_scr_create();
    lv_scr_load(scr);

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "ESP32-C6 微系统");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 6);

    const char *names[4] = {"文件", "播放", "系统", "设置"};
    lv_color_t cols[4] = {
        lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_ORANGE), lv_palette_main(LV_PALETTE_TEAL)};
    for (int i = 0; i < 4; i++) {
        lv_obj_t *ic = ui_icon(scr, names[i], 70, 56, cols[i]);
        lv_obj_set_pos(ic, 16 + (i % 2) * 86, 70 + (i / 2) * 66);
        lv_group_add_obj(group, ic);
        lv_obj_add_event_cb(ic, event_cb, LV_EVENT_CLICKED, NULL);
    }

    lv_obj_t *cpu_bar = lv_bar_create(scr);
    lv_obj_set_size(cpu_bar, 150, 10);
    lv_obj_align(cpu_bar, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_bar_set_value(cpu_bar, 10, LV_ANIM_OFF);
}
