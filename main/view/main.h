#ifndef _PRISM_UI_MAIN_H
#define _PRISM_UI_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

extern lv_obj_t *screen_main;
extern lv_obj_t *screen_main_weather;
extern lv_obj_t *screen_main_hour;
extern lv_obj_t *screen_main_hour_dis;
extern lv_obj_t *screen_main_min;
extern lv_obj_t *screen_main_min_dis;
extern lv_obj_t *screen_main_colon;
extern lv_obj_t *screen_main_adorn;
extern lv_obj_t *screen_main_wifi;
extern lv_obj_t *screen_main_date_panel;
extern lv_obj_t *screen_main_date;
extern lv_obj_t *screen_main_location;
extern lv_obj_t *screen_main_location_icon;
extern lv_obj_t *screen_main_city;

void screen_main_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif