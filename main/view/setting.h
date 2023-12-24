#ifndef _PRISM_UI_SETTING_H
#define _PRISM_UI_SETTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_setting;
extern lv_obj_t *screen_setting_wifi;
extern lv_obj_t *screen_setting_heading;
extern lv_obj_t *screen_setting_text;

void screen_setting_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif