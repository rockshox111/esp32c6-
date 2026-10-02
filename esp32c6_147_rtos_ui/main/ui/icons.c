#include "icons.h"
lv_obj_t *ui_icon(lv_obj_t *parent, const char *text, int w, int h, lv_color_t bg)
{
    lv_obj_t *c = lv_obj_create(parent);
    lv_obj_set_size(c, w, h);
    lv_obj_set_style_bg_color(c, bg, 0);
    lv_obj_clear_flag(c, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *l = lv_label_create(c);
    lv_label_set_text(l, text);
    lv_obj_center(l);
    return c;
}
