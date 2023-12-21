#include "setting.h"
#include "ui_helpers.h"
#include "main.h"
#include "setting_brightness.h"
#include "setting_fade_text.h"

lv_obj_t *screen_setting;
lv_obj_t *screen_setting_wifi;
lv_obj_t *screen_setting_heading;
lv_obj_t *screen_setting_text;
lv_obj_t *screen_setting_indicator;

void event_screen_setting( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
if ( event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT  ) {
      _ui_screen_change( screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
}
if ( event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP  ) {
      _ui_screen_change( screen_setting_brightness, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 0);
}
}

void screen_setting_init(void) {

}