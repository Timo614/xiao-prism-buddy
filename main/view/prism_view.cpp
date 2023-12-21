#include "prism_view.h"

#include "browser.h"
#include "cryptocurrency.h"
#include "main.h"
#include "setting_24_hour_clock.h"
#include "setting_brightness.h"
#include "setting_fade_text.h"
#include "setting.h"

#include "config.h"
#include "ui_helpers.h"
#include "esp_wifi.h"
#include <time.h>

static const char *TAG = "view";

lv_obj_t *screen_last;

/*****************************************************************/
// wifi config 
/*****************************************************************/
LV_IMG_DECLARE( ui_img_wifi_1_png);
LV_IMG_DECLARE( ui_img_wifi_2_png);
LV_IMG_DECLARE( ui_img_wifi_3_png);
LV_IMG_DECLARE( ui_img_lock_png);

/***********************************************************************************************************/

static void __view_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    lv_port_sem_take();
    switch (id)
    {
        case VIEW_EVENT_SCREEN_START: {
            uint8_t screen = *( uint8_t *)event_data;
            lv_disp_load_scr( screen_main ); 
            break;
        }
        case VIEW_EVENT_TIME: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME");
            bool time_format_24 = true;
            if( event_data) {
                time_format_24 = *( bool *)event_data;
            } 
            
            time_t now = 0;
            struct tm timeinfo = { 0 };
            char *p_wday_str;

            time(&now);
            localtime_r(&now, &timeinfo);
            char buf_h[3];
            char buf_m[3];
            char buf[6];
            int hour = timeinfo.tm_hour;

            if( ! time_format_24 ) {
                if( hour>=13 && hour<=23) {
                    hour = hour-12;
                }
            } 
            lv_snprintf(buf_h, sizeof(buf_h), "%02d", hour);
            lv_label_set_text(screen_main_hour_dis, buf_h);
            lv_snprintf(buf_m, sizeof(buf_m), "%02d", timeinfo.tm_min);
            lv_label_set_text(screen_main_min_dis, buf_m);

            lv_snprintf(buf, sizeof(buf), "%02d:%02d", hour, timeinfo.tm_min);
            // lv_label_set_text(screen_main_time2, buf);
            // lv_label_set_text(screen_main_time3, buf);

            switch (timeinfo.tm_wday)
            {
                case 0: p_wday_str="Sunday";break;
                case 1: p_wday_str="Monday";break;
                case 2: p_wday_str="Tuesday";break;
                case 3: p_wday_str="Wednesday";break;
                case 4: p_wday_str="Thursday";break;
                case 5: p_wday_str="Friday";break;
                case 6: p_wday_str="Sunday";break;
                default: p_wday_str="";break;
            }
            char buf1[32];
            lv_snprintf(buf1, sizeof(buf1), "%s, %02d / %02d / %04d", p_wday_str,  timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year+1900);
            lv_label_set_text(screen_main_date, buf1);
            break;
        }

        case VIEW_EVENT_TIME_CFG_UPDATE: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME_CFG_UPDATE");
            struct view_data_time_cfg *p_cfg = ( struct view_data_time_cfg *)event_data;
            
            if(  p_cfg->time_format_24 ) {
                lv_dropdown_set_selected(screen_setting_24_hour_clock_toggle, 0);
            } else {
                lv_dropdown_set_selected(screen_setting_24_hour_clock_toggle, 1);
            }            
            break;
        }

        case VIEW_EVENT_DISPLAY_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_DISPLAY_CFG");
            struct view_data_display *p_cfg = ( struct view_data_display *)event_data;
            
            lv_slider_set_value(screen_setting_brightness_slider, p_cfg->brightness, LV_ANIM_OFF);
            if( p_cfg->fade_text_enabled ) {
                lv_obj_clear_state( screen_setting_fade_text_cfg, LV_STATE_CHECKED);
            } else {
                lv_obj_add_state( screen_setting_fade_text_cfg, LV_STATE_CHECKED);
            }
            break;
        }

        case VIEW_EVENT_WIFI_ST: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            struct view_data_wifi_st *p_st = ( struct view_data_wifi_st *)event_data;
            
            const lv_img_dsc_t *p_src;
            if ( p_st->is_connected ) {
                //    0    rssi<=-100
                //    1    (-100, -88]
                //    2    (-88, -77]
                //    3    (-66, -55]
                //    4    rssi>=-55
                if( p_st->rssi > -66 ) {
                    p_src = &ui_img_wifi_3_png;
                } else if( p_st->rssi > -88) {
                    p_src = &ui_img_wifi_2_png;
                } else {
                    p_src = &ui_img_wifi_1_png;
                }
            } else {
                p_src = &ui_img_wifi_disconnect_png;
            }

            lv_img_set_src(screen_main_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_24_hour_clock_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_brightness_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_fade_text_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_wifi , (void *)p_src);
            lv_img_set_src(screen_browser_wifi , (void *)p_src);
            lv_img_set_src(screen_cryptocurrency_wifi , (void *)p_src);
            break;
        }
        case VIEW_EVENT_SENSOR_DATA: {
            // TODO
        }
        case VIEW_EVENT_SCREEN_CTRL: {
            bool  *p_st = (bool *) event_data;

            if ( *p_st == 1) {
                lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
    	        lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_TRANSP, 0);
            } else {
                lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_bg_opa(lv_layer_top(), LV_OPA_COVER, 0);
                lv_obj_set_style_bg_color(lv_layer_top(), lv_color_black(), 0);
            }
            break;
        }

        default:
            break;
    }
    lv_port_sem_give();
}

void prism_view_init(void)
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_color_hex(0x529d53), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    screen_browser_init();
    screen_cryptocurrency_init();
    screen_main_init();
    screen_setting_24_hour_clock_init();
    screen_setting_brightness_init();
    screen_setting_fade_text_init();
    screen_setting_init();

    lv_disp_load_scr( screen_main);

    int i  = 0;
    for( i = 0; i < VIEW_EVENT_ALL; i++ ) {
        ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                                VIEW_EVENT_BASE, i, 
                                                                __view_event_handler, NULL, NULL));
    }
}