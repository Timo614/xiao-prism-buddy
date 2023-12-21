#ifndef _PRISM_UI_BROWSER_H
#define _PRISM_UI_BROWSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

LV_IMG_DECLARE( ui_gifs_confetti );
LV_IMG_DECLARE( ui_gifs_fire );
LV_IMG_DECLARE( ui_gifs_hologram );
LV_IMG_DECLARE( ui_gifs_watermelon );

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


