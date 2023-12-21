#include "prism_controller.h"
#include "lvgl.h"
#include "lv_port.h"

#include "main.h"
#include "browser.h"
#include "cryptocurrency.h"
#include "setting.h"
#include "setting_24_hour_clock.h"
#include "setting_brightness.h"
#include "setting_fade_text.h"

#include "view_data.h"
#include "ui_helpers.h"
#include <time.h>
#include <sys/time.h>

/**********************
 *  STATIC VARIABLES
 **********************/

static const char *TAG = "controller";

/**********************  display cfg **********************/
static void __brightness_cfg_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_BRIGHTNESS_UPDATE, &value, sizeof(value), portMAX_DELAY);
}

void __save_settings(void) {
    struct view_data_display cfg;
    cfg.brightness = lv_slider_get_value(screen_setting_brightness_slider);
    if( ! lv_obj_has_state( screen_setting_fade_text_cfg, LV_STATE_CHECKED) ) {
        cfg.fade_text_enabled = true;
    } else {
        cfg.fade_text_enabled = false;
    }

    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG_APPLY, &cfg, sizeof(cfg), portMAX_DELAY);
}


static void __view_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    lv_port_sem_take();
    switch (id)
    {
        case VIEW_EVENT_SCREEN_START: {
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
            struct tm timeinfo = {
                .tm_sec = 0,
                .tm_min = 0,
                .tm_hour = 0,
                .tm_mday = 1,
                .tm_mon = 0, 
                .tm_year = 0,
                .tm_wday = 0, 
                .tm_yday = 0, 
                .tm_isdst = 0 
            };

            time(&now);
            localtime_r(&now, &timeinfo);
            char buf[6];
            int hour = timeinfo.tm_hour;

            if( ! time_format_24 ) {
                if( hour>=13 && hour<=23) {
                    hour = hour-12;
                }
            } 

            lv_snprintf(buf, sizeof(buf), "%02d:%02d", hour, timeinfo.tm_min);
            lv_label_set_text(screen_main_time, buf);
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
            if( !p_cfg->fade_text_enabled ) {
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
            if (current_screen == screen_main) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;
                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    _ui_screen_change( screen_cryptocurrency, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    _ui_screen_change( screen_setting, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                }
            } else if (current_screen == screen_browser) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;
                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    _ui_screen_change( screen_setting, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    _ui_screen_change( screen_cryptocurrency, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                }
            } else if (current_screen == screen_cryptocurrency) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;
                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    _ui_screen_change( screen_browser, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    _ui_screen_change( screen_main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                }
            } else if (current_screen == screen_setting_24_hour_clock ||
                current_screen == screen_setting_brightness ||
                current_screen == screen_setting_fade_text ||
                current_screen == screen_setting
                ) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;
                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    _ui_screen_change( screen_main, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    _ui_screen_change( screen_browser, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                }
            }
            break;
        }
        default:
            break;
    }
    lv_port_sem_give();
}

int prism_controller_init(void)
{   
    current_screen = screen_main;
    lv_obj_add_event_cb(screen_setting_brightness_slider, __brightness_cfg_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    
    int i  = 0;
    for( i = 0; i < VIEW_EVENT_ALL; i++ ) {
        ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                                VIEW_EVENT_BASE, i, 
                                                                __view_event_handler, NULL, NULL));
    }

    return 0;
}