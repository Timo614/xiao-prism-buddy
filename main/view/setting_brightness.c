#include "setting_brightness.h"
#include "ui_helpers.h"
#include "setting.h"

lv_obj_t *screen_setting_brightness;
lv_obj_t *screen_setting_brightness_wifi;
lv_obj_t *screen_setting_brightness_slider;
lv_obj_t *screen_setting_brightness_title;
lv_obj_t *screen_setting_brightness_icon_1;
lv_obj_t *screen_setting_brightness_icon_2;

void screen_setting_brightness_init(void) {
    screen_setting_brightness = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_setting_brightness, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_brightness_wifi = lv_img_create(screen_setting_brightness);
    lv_img_set_src(screen_setting_brightness_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_setting_brightness_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_brightness_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_setting_brightness_wifi, -20 );
    lv_obj_set_y( screen_setting_brightness_wifi, 20 );
    lv_obj_set_align( screen_setting_brightness_wifi, LV_ALIGN_TOP_RIGHT );
    lv_obj_add_flag( screen_setting_brightness_wifi, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_setting_brightness_wifi, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_brightness_title = lv_label_create(screen_setting_brightness);
    lv_obj_set_width( screen_setting_brightness_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_brightness_title, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_y( screen_setting_brightness_title, 50 );
    lv_obj_set_align( screen_setting_brightness_title, LV_ALIGN_TOP_MID );
    lv_label_set_text(screen_setting_brightness_title,"Brightness");
    lv_obj_set_style_text_font(screen_setting_brightness_title, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_setting_brightness_slider = lv_slider_create(screen_setting_brightness);
    lv_slider_set_range(screen_setting_brightness_slider, 0, 100);
    lv_slider_set_value(screen_setting_brightness_slider, 100, LV_ANIM_OFF);
    lv_obj_set_width( screen_setting_brightness_slider, 200);
    lv_obj_set_height( screen_setting_brightness_slider, 15);
    lv_obj_set_x( screen_setting_brightness_slider, 0 );
    lv_obj_set_y( screen_setting_brightness_slider, 10 );
    lv_obj_set_align( screen_setting_brightness_slider, LV_ALIGN_CENTER );
    lv_obj_set_style_bg_color(screen_setting_brightness_slider, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_brightness_slider, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_setting_brightness_slider, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_brightness_slider, 255, LV_PART_INDICATOR| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_setting_brightness_slider, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_brightness_slider, 255, LV_PART_KNOB| LV_STATE_DEFAULT);

    screen_setting_brightness_icon_1 = lv_img_create(screen_setting_brightness);
    lv_img_set_src(screen_setting_brightness_icon_1, &ui_img_high_light_png);
    lv_obj_set_width( screen_setting_brightness_icon_1, LV_SIZE_CONTENT);  /// 22
    lv_obj_set_height( screen_setting_brightness_icon_1, LV_SIZE_CONTENT);   /// 22
    lv_obj_set_x( screen_setting_brightness_icon_1, 0 );
    lv_obj_set_y( screen_setting_brightness_icon_1, 10 );
    lv_obj_set_align( screen_setting_brightness_icon_1, LV_ALIGN_RIGHT_MID );
    lv_obj_add_flag( screen_setting_brightness_icon_1, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_setting_brightness_icon_1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_brightness_icon_2 = lv_img_create(screen_setting_brightness);
    lv_img_set_src(screen_setting_brightness_icon_2, &ui_img_low_light_png);
    lv_obj_set_width( screen_setting_brightness_icon_2, LV_SIZE_CONTENT);  /// 18
    lv_obj_set_height( screen_setting_brightness_icon_2, LV_SIZE_CONTENT);   /// 18
    lv_obj_set_x( screen_setting_brightness_icon_2, 0 );
    lv_obj_set_y( screen_setting_brightness_icon_2, 10 );
    lv_obj_set_align( screen_setting_brightness_icon_2, LV_ALIGN_LEFT_MID );
    lv_obj_add_flag( screen_setting_brightness_icon_2, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_setting_brightness_icon_2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
}