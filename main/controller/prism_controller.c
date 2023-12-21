#include "prism_controller.h"
#include "lvgl.h"

#include "main.h"
#include "setting.h"
#include "setting_brightness.h"
#include "setting_fade_text.h"

#include "ui_helpers.h"
#include <time.h>
#include <sys/time.h>

/**********************
 *  VARIABLES alias
 **********************/



/**********************
 *  STATIC VARIABLES
 **********************/

static const char *TAG = "controller";

/**********************  display cfg **********************/
static void __brightness_cfg_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_BRIGHTNESS_UPDATE, &value, sizeof(value), portMAX_DELAY);
}

void __screen_event_screen_main( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if ( event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT  ) {
        _ui_screen_change( screen_setting, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
    }
    if ( event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT  ) {
        _ui_screen_change( screen_setting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
    }
}

void __screen_event_screen_setting( lv_event_t * e) {
    struct view_data_display cfg;
    cfg.brightness = lv_slider_get_value(screen_setting_brightness_slider);
    if( ! lv_obj_has_state( screen_setting_fade_text_cfg, LV_STATE_CHECKED) ) {
        cfg.fade_text_enabled = true;
    } else {
        cfg.fade_text_enabled = false;
    }

    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG_APPLY, &cfg, sizeof(cfg), portMAX_DELAY);
}

int prism_controller_init(void)
{   
    lv_obj_add_event_cb(screen_setting_brightness_slider, __brightness_cfg_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_add_event_cb(screen_main, __screen_event_screen_main, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(screen_setting_brightness, __screen_event_screen_setting, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(screen_setting_fade_text, __screen_event_screen_setting, LV_EVENT_ALL, NULL);

    return 0;
}