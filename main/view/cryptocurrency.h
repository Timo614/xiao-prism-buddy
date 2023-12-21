#ifndef _PRISM_UI_CRYPTOCURRENCY_H
#define _PRISM_UI_CRYPTOCURRENCY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_cryptocurrency;
extern lv_obj_t *screen_cryptocurrency_wifi;
extern lv_obj_t *screen_cryptocurrency_image;
extern lv_obj_t *screen_cryptocurrency_value;
extern lv_obj_t *screen_cryptocurrency_text;
extern lv_obj_t *screen_cryptocurrency_change_image;
extern lv_obj_t *screen_cryptocurrency_page;

void screen_cryptocurrency_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif