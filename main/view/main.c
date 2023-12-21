#include "main.h"
#include "setting.h"
#include "ui_helpers.h"

lv_obj_t *screen_main;
lv_obj_t *screen_main_weather;
lv_obj_t *screen_main_time;
lv_obj_t *screen_main_colon;
lv_obj_t *screen_main_wifi;

void screen_main_init(void)
{
    screen_main = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_main, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_main_wifi = lv_img_create(screen_main);
    lv_img_set_src(screen_main_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_main_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_main_wifi, -20 );
    lv_obj_set_y( screen_main_wifi, 20 );
    lv_obj_set_align( screen_main_wifi, LV_ALIGN_TOP_RIGHT );

    screen_main_weather = lv_img_create(screen_main);
    lv_img_set_src(screen_main_weather, &ui_weather_clear_day);
    lv_img_set_src(screen_main_weather, &ui_weather_clear_night);
    lv_img_set_src(screen_main_weather, &ui_weather_cloudy);
    lv_img_set_src(screen_main_weather, &ui_weather_partly_cloudy_day);
    lv_img_set_src(screen_main_weather, &ui_weather_partly_cloudy_night);
    lv_img_set_src(screen_main_weather, &ui_weather_rain);
    lv_img_set_src(screen_main_weather, &ui_weather_sleet);
    lv_img_set_src(screen_main_weather, &ui_weather_snow);
    lv_obj_set_width( screen_main_weather, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_weather, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_main_weather, 0 );
    lv_obj_set_y( screen_main_weather, 0 );
    lv_obj_set_align( screen_main_weather, LV_ALIGN_CENTER );

    screen_main_time = lv_label_create(screen_main);
    lv_obj_set_width( screen_main_time, 200);
    lv_obj_set_height( screen_main_time, 170);
    lv_obj_set_x( screen_main_time, -110 );
    lv_obj_set_y( screen_main_time, 0 );
    lv_obj_set_align( screen_main_time, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_time, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_time, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_time, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_label_set_text(screen_main_time,"00:00");
}