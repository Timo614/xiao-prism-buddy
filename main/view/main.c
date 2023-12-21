#include "main.h"
#include "setting.h"
#include "ui_helpers.h"

lv_obj_t *screen_main;
lv_obj_t *screen_main_weather;
lv_obj_t *screen_main_hour;
lv_obj_t *screen_main_hour_dis;
lv_obj_t *screen_main_min;
lv_obj_t *screen_main_min_dis;
lv_obj_t *screen_main_colon;
lv_obj_t *screen_main_wifi;
lv_obj_t *screen_main_date_panel;
lv_obj_t *screen_main_date;

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

    screen_main_hour = lv_obj_create(screen_main);
    lv_obj_set_width( screen_main_hour, 200);
    lv_obj_set_height( screen_main_hour, 170);
    lv_obj_set_x( screen_main_hour, -110 );
    lv_obj_set_y( screen_main_hour, 0 );
    lv_obj_set_align( screen_main_hour, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_hour, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_hour, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_hour, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_hour_dis = lv_label_create(screen_main_hour);
    lv_obj_set_width( screen_main_hour_dis, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_hour_dis, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_hour_dis, LV_ALIGN_CENTER );
    lv_label_set_text(screen_main_hour_dis,"00");

    screen_main_min = lv_obj_create(screen_main);
    lv_obj_set_width( screen_main_min, 200);
    lv_obj_set_height( screen_main_min, 170);
    lv_obj_set_x( screen_main_min, 110 );
    lv_obj_set_y( screen_main_min, 0 );
    lv_obj_set_align( screen_main_min, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_min, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_min, lv_color_hex(0x2E2E2E), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_min, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_min_dis = lv_label_create(screen_main_min);
    lv_obj_set_width( screen_main_min_dis, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_min_dis, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_min_dis, LV_ALIGN_CENTER );
    lv_label_set_text(screen_main_min_dis,"00");

    screen_main_date_panel = lv_obj_create(screen_main);
    lv_obj_set_width( screen_main_date_panel, 235);
    lv_obj_set_height( screen_main_date_panel, 30);
    lv_obj_set_x( screen_main_date_panel, -103 );
    lv_obj_set_y( screen_main_date_panel, -108 );
    lv_obj_set_align( screen_main_date_panel, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_date_panel, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_date_panel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_date_panel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_main_date_panel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_color(screen_main_date_panel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(screen_main_date_panel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_date = lv_label_create(screen_main_date_panel);
    lv_obj_set_width( screen_main_date, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_date, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_date, LV_ALIGN_LEFT_MID );
    lv_label_set_text(screen_main_date,"Monday, 01 / 01 / 1970");
    lv_label_set_recolor(screen_main_date,"true");

    screen_main_colon = lv_label_create(screen_main);
    lv_obj_set_width( screen_main_colon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_colon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_colon, LV_ALIGN_CENTER );
    lv_label_set_text(screen_main_colon,":");
}