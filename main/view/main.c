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
lv_obj_t *screen_main_adorn;
lv_obj_t *screen_main_wifi;
lv_obj_t *screen_main_date_panel;
lv_obj_t *screen_main_date;
lv_obj_t *screen_main_location;
lv_obj_t *screen_main_location_icon;
lv_obj_t *screen_main_city;

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
    lv_obj_add_flag( screen_main_wifi, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_main_wifi, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_add_flag( screen_main_wifi, LV_OBJ_FLAG_CLICKABLE );    /// Flags

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
    lv_obj_set_style_text_font(screen_main_hour_dis, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);

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
    lv_obj_set_style_text_font(screen_main_min_dis, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_location = lv_obj_create(screen_main);
    lv_obj_set_width( screen_main_location, 399);
    lv_obj_set_height( screen_main_location, 30);
    lv_obj_set_x( screen_main_location, 6 );
    lv_obj_set_y( screen_main_location, 103 );
    lv_obj_set_align( screen_main_location, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_location, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_location, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_location, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_main_location, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_color(screen_main_location, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(screen_main_location, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_city = lv_label_create(screen_main_location);
    lv_obj_set_width( screen_main_city, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_city, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_city, LV_ALIGN_RIGHT_MID );
    lv_label_set_text(screen_main_city," -- ");
    lv_obj_set_style_text_font(screen_main_city, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_location_icon = lv_img_create(screen_main);
    lv_img_set_src(screen_main_location_icon, &ui_img_location_png);
    lv_obj_set_width( screen_main_location_icon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_location_icon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_main_location_icon, 199 );
    lv_obj_set_y( screen_main_location_icon, 103 );
    lv_obj_set_align( screen_main_location_icon, LV_ALIGN_CENTER );
    lv_obj_add_flag( screen_main_location_icon, LV_OBJ_FLAG_ADV_HITTEST );   /// Flags
    lv_obj_clear_flag( screen_main_location_icon, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

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
    lv_obj_set_style_text_font(screen_main_date, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_colon = lv_label_create(screen_main);
    lv_obj_set_width( screen_main_colon, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_main_colon, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_main_colon, LV_ALIGN_CENTER );
    lv_label_set_text(screen_main_colon,":");
    lv_obj_set_style_text_font(screen_main_colon, &ui_font_font3, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_main_adorn = lv_obj_create(screen_main);
    lv_obj_set_width( screen_main_adorn, 480);
    lv_obj_set_height( screen_main_adorn, 6);
    lv_obj_set_align( screen_main_adorn, LV_ALIGN_CENTER );
    lv_obj_clear_flag( screen_main_adorn, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(screen_main_adorn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(screen_main_adorn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(screen_main_adorn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_color(screen_main_adorn, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_border_opa(screen_main_adorn, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

}