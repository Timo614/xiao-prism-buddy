#ifndef _PRISM_UI_SETTING_24_HOUR_CLOCK_H
#define _PRISM_UI_SETTING_24_HOUR_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_setting_24_hour_clock;
extern lv_obj_t *screen_setting_24_hour_clock_wifi;
extern lv_obj_t *screen_setting_24_hour_clock_title;
extern lv_obj_t *screen_setting_24_hour_clock_toggle;

void screen_setting_24_hour_clock_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif