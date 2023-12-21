#include "setting_24_hour_clock.h"
#include "ui_helpers.h"
#include "setting.h"
#include "main.h"

lv_obj_t *screen_setting_24_hour_clock;
lv_obj_t *screen_setting_24_hour_clock_wifi;
lv_obj_t *screen_setting_24_hour_clock_title;
lv_obj_t *screen_setting_24_hour_clock_toggle;

void screen_setting_24_hour_clock_init(void) {
    screen_setting_24_hour_clock = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_setting_24_hour_clock, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_24_hour_clock_wifi = lv_img_create(screen_setting_24_hour_clock);
    lv_img_set_src(screen_setting_24_hour_clock_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_setting_24_hour_clock_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_24_hour_clock_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_setting_24_hour_clock_wifi, -20 );
    lv_obj_set_y( screen_setting_24_hour_clock_wifi, 20 );
    lv_obj_set_align( screen_setting_24_hour_clock_wifi, LV_ALIGN_TOP_RIGHT );
    lv_obj_add_flag( screen_setting_24_hour_clock_wifi, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_setting_24_hour_clock_wifi, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_24_hour_clock_title = lv_label_create(screen_setting_24_hour_clock);
    lv_obj_set_width( screen_setting_24_hour_clock_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_24_hour_clock_title, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_setting_24_hour_clock_title, LV_ALIGN_LEFT_MID );
    lv_label_set_text(screen_setting_24_hour_clock_title,"Time Format");

    screen_setting_24_hour_clock_toggle = lv_dropdown_create(screen_setting_24_hour_clock);
    lv_dropdown_set_options( screen_setting_24_hour_clock_toggle, "24H\n12H" );
    lv_obj_set_width( screen_setting_24_hour_clock_toggle, 100);
    lv_obj_set_height( screen_setting_24_hour_clock_toggle, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_setting_24_hour_clock_toggle, LV_ALIGN_RIGHT_MID );
    lv_obj_add_flag( screen_setting_24_hour_clock_toggle, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
    lv_obj_set_style_bg_color(screen_setting_24_hour_clock_toggle, lv_color_hex(0x6F6F6F), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_24_hour_clock_toggle, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
}