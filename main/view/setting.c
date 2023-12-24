#include "setting.h"
#include "ui_helpers.h"
#include "main.h"
#include "setting_brightness.h"
#include "setting_fade_text.h"

lv_obj_t *screen_setting;
lv_obj_t *screen_setting_wifi;
lv_obj_t *screen_setting_heading;
lv_obj_t *screen_setting_text;

void screen_setting_init(void) {
    screen_setting = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_setting, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_setting_wifi = lv_img_create(screen_setting);
    lv_img_set_src(screen_setting_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_setting_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_setting_wifi, -20 );
    lv_obj_set_y( screen_setting_wifi, 20 );
    lv_obj_set_align( screen_setting_wifi, LV_ALIGN_TOP_RIGHT );

    screen_setting_heading = lv_label_create(screen_setting);
    lv_obj_set_width( screen_setting_heading, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_heading, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_setting_heading, LV_ALIGN_CENTER );
    lv_label_set_text(screen_setting_heading,"Settings");

    screen_setting_text = lv_label_create(screen_setting);
    lv_obj_set_width( screen_setting_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_setting_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_setting_text, LV_ALIGN_CENTER );
    lv_label_set_text(screen_setting_text,"Grab to change setting screen\nRotate to toggle as needed");
}