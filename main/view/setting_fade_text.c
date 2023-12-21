#include "setting_fade_text.h"
#include "ui_helpers.h"

lv_obj_t *screen_setting_fade_text;
lv_obj_t *screen_setting_fade_text_wifi;
lv_obj_t *screen_setting_fade_text_title;
lv_obj_t *screen_setting_fade_text_cfg;

void screen_setting_fade_text_init(void) {

    screen_setting_fade_text = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_setting_fade_text, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_fade_text_wifi = lv_img_create(screen_setting_fade_text);
    lv_img_set_src(screen_setting_fade_text_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_setting_fade_text_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_fade_text_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_setting_fade_text_wifi, -20 );
    lv_obj_set_y( screen_setting_fade_text_wifi, 20 );
    lv_obj_set_align( screen_setting_fade_text_wifi, LV_ALIGN_TOP_RIGHT );
    lv_obj_add_flag( screen_setting_fade_text_wifi, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_setting_fade_text_wifi, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_fade_text_title = lv_label_create(screen_setting_fade_text);
    lv_obj_set_width( screen_setting_fade_text_title, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_fade_text_title, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_setting_fade_text_title, LV_ALIGN_LEFT_MID );
    lv_label_set_text(screen_setting_fade_text_title,"Always-on");

    screen_setting_fade_text_cfg = lv_switch_create(screen_setting_fade_text);
    lv_obj_set_width( screen_setting_fade_text_cfg, 50);
    lv_obj_set_height( screen_setting_fade_text_cfg, 25);
    lv_obj_set_align( screen_setting_fade_text_cfg, LV_ALIGN_RIGHT_MID );
    lv_obj_set_style_bg_color(screen_setting_fade_text_cfg, lv_color_hex(0x363636), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_setting_fade_text_cfg, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(screen_setting_fade_text_cfg, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_CHECKED|LV_STATE_PRESSED );
    lv_obj_set_style_bg_opa(screen_setting_fade_text_cfg, 255, LV_PART_MAIN| LV_STATE_CHECKED|LV_STATE_PRESSED);

    lv_obj_set_style_bg_color(screen_setting_fade_text_cfg, lv_color_hex(0x529D53), LV_PART_INDICATOR | LV_STATE_CHECKED );
    lv_obj_set_style_bg_opa(screen_setting_fade_text_cfg, 255, LV_PART_INDICATOR| LV_STATE_CHECKED);
}