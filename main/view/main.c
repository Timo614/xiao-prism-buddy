#include "main.h"
#include "setting.h"
#include "ui_helpers.h"

lv_obj_t *screen_main;
lv_obj_t *screen_main_weather;
lv_obj_t *screen_main_time;

void screen_main_init(void)
{
    screen_main = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_main, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_main_time = lv_label_create(screen_main);
    lv_obj_set_width( screen_main_time, 200);
    lv_obj_set_height( screen_main_time, 170);
    lv_obj_set_x( screen_main_time, -110 );
    lv_obj_set_y( screen_main_time, 0 );
    lv_obj_set_align( screen_main_time, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_time, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_label_set_text(screen_main_time,"00:00");
}