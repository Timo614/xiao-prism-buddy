#include "browser.h"
#include "ui_helpers.h"

lv_obj_t *screen_browser;
lv_obj_t *screen_browser_image;

void screen_browser_init(void)
{
    screen_browser = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_browser, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
}