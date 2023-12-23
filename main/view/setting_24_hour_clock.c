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
    lv_label_set_text(screen_setting_24_hour_clock_title,"Use 24 Hour Time Format");

    screen_setting_24_hour_clock_toggle = lv_switch_create(screen_setting_24_hour_clock);
    lv_obj_set_width( screen_setting_24_hour_clock_toggle, 50);
    lv_obj_set_height( screen_setting_24_hour_clock_toggle, 25);
    lv_obj_set_align( screen_setting_24_hour_clock_toggle, LV_ALIGN_RIGHT_MID );
    lv_obj_set_style_bg_color(screen_setting_24_hour_clock_toggle, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_24_hour_clock_toggle, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_setting_24_hour_clock_toggle, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED|LV_STATE_PRESSED );
    lv_obj_set_style_bg_opa(screen_setting_24_hour_clock_toggle, 255, LV_PART_MAIN| LV_STATE_CHECKED|LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(screen_setting_24_hour_clock_toggle, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_CHECKED );
    lv_obj_set_style_bg_opa(screen_setting_24_hour_clock_toggle, 255, LV_PART_INDICATOR| LV_STATE_CHECKED);
}