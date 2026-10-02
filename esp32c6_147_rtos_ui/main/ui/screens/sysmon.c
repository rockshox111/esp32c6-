#include "sysmon.h"
#include "queue.h"
#include <string.h>

static lv_obj_t *ta;
static lv_obj_t *heap_lbl;

void sysmon_screen_create(lv_group_t *group)
{
    lv_obj_t *scr = lv_scr_create();
    lv_scr_load(scr);
    ta = lv_textarea_create(scr);
    lv_obj_set_size(ta, 168, 230);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 6);
    lv_textarea_set_text(ta, "采集中...");
    lv_group_add_obj(group, ta);

    heap_lbl = lv_label_create(scr);
    lv_obj_align(heap_lbl, LV_ALIGN_BOTTOM_MID, 0, -6);
    sysmon_screen_refresh();
}

void sysmon_screen_refresh(void)
{
    if (!ta) return;
    sys_snapshot_t s;
    if (xQueuePeek(g_sys_q, &s, 0) == pdTRUE) {
        char out[900];
        snprintf(out, sizeof(out), "HeapFree:%u Min:%u\n%s", s.heap_free, s.heap_min, s.task_stats);
        lv_textarea_set_text(ta, out);
        if (heap_lbl) lv_label_set_text_fmt(heap_lbl, "free %uB", s.heap_free);
    }
}
