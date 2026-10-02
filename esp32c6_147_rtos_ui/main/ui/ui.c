#include "ui.h"
#include "screens/home.h"
#include "screens/files.h"
#include "screens/sysmon.h"

void ui_init(lv_group_t *group) { home_screen_create(group); }
void ui_sys_update(void) { sysmon_screen_refresh(); }
