#ifndef _PRISM_UI_SETTING_BRIGHTNESS_H
#define _PRISM_UI_SETTING_BRIGHTNESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_setting_brightness;
extern lv_obj_t *screen_setting_brightness_wifi;
extern lv_obj_t *screen_setting_brightness_slider;
extern lv_obj_t *screen_setting_brightness_title;
extern lv_obj_t *screen_setting_brightness_icon_1;
extern lv_obj_t *screen_setting_brightness_icon_2;

void screen_setting_brightness_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif