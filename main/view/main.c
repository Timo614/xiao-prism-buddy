#include "main.h"
#include "setting.h"
#include "ui_helpers.h"

lv_obj_t *screen_main;
lv_obj_t *screen_main_time;
lv_obj_t *screen_main_weather;

void screen_main_init(void)
{
    screen_main = lv_obj_create(NULL);
    
    lv_obj_t * screen_main_time = lv_label_create(screen_main);
    lv_obj_set_y( screen_main_time, -30 );
    lv_obj_set_x( screen_main_time, 10 );
    lv_obj_set_align( screen_main_time, LV_ALIGN_BOTTOM_MID );
    lv_obj_clear_flag( screen_main_time, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_text_font(screen_main_time, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);
}