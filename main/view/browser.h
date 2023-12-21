#ifndef _PRISM_UI_BROWSER_H
#define _PRISM_UI_BROWSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_browser;
extern lv_obj_t *screen_browser_wifi;
extern lv_obj_t *screen_browser_image;
extern lv_obj_t *screen_browser_text;
extern lv_obj_t *screen_browser_page;

void screen_browser_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif


