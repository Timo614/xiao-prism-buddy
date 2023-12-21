#ifndef _PRISM_UI_SETTING_FADE_H
#define _PRISM_UI_SETTING_FADE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_setting_fade_text;
extern lv_obj_t *screen_setting_fade_text_wifi;
extern lv_obj_t *screen_setting_fade_text_title;
extern lv_obj_t *screen_setting_fade_text_cfg;

void screen_setting_fade_text_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif