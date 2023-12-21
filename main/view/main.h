#ifndef _PRISM_UI_MAIN_H
#define _PRISM_UI_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

LV_IMG_DECLARE( ui_weather_clear_day );
LV_IMG_DECLARE( ui_weather_clear_night );
LV_IMG_DECLARE( ui_weather_cloudy );
LV_IMG_DECLARE( ui_weather_partly_cloudy_day );
LV_IMG_DECLARE( ui_weather_partly_cloudy_night );
LV_IMG_DECLARE( ui_weather_rain );
LV_IMG_DECLARE( ui_weather_sleet );
LV_IMG_DECLARE( ui_weather_snow );

extern lv_obj_t *screen_main;
extern lv_obj_t *screen_main_weather;
extern lv_obj_t *screen_main_hour;
extern lv_obj_t *screen_main_hour_dis;
extern lv_obj_t *screen_main_min;
extern lv_obj_t *screen_main_min_dis;
extern lv_obj_t *screen_main_colon;
extern lv_obj_t *screen_main_wifi;
extern lv_obj_t *screen_main_date_panel;
extern lv_obj_t *screen_main_date;

void screen_main_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif