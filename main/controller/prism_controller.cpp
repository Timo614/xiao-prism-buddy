#include "prism_controller.h"
#include "lvgl.h"
#include "lv_port.h"
#include <math.h>

#include "main.h"
#include "browser.h"
#include "cryptocurrency.h"
#include "prism_cryptocurrency.h"
#include "cryptocurrency.h"
#include "setting.h"
#include "setting_24_hour_clock.h"
#include "setting_brightness.h"

#include "view_data.h"
#include "ui_helpers.h"
#include <time.h>
#include <sys/time.h>

#define ROTATION_COOLDOWN_MS 1000  // Cooldown period in milliseconds
static int64_t __g_last_rotation_time = 0;  // Timestamp of the last rotation

/**********************
 *  STATIC VARIABLES
 **********************/

static const char *TAG = "controller";

static const void *image_sources[] = {
    &ui_gifs_watermelon,
    &ui_gifs_confetti,
    &ui_gifs_hologram,
    &ui_gifs_fire,
    &ui_weather_clear_day,
    &ui_weather_clear_night,
    &ui_weather_cloudy,
    &ui_weather_partly_cloudy_day,
    &ui_weather_partly_cloudy_night,
    &ui_weather_rain,
    &ui_weather_sleet,
    &ui_weather_snow
};

static const void *crypto_image_sources[] = {
    &ui_coins_btc,
    &ui_coins_doge,
    &ui_coins_eth,
    &ui_coins_xrp
};

static const char *crypto_descriptions[] = {
    "Bitcoin",
    "Dogecoin",
    "Ethereum",
    "Ripple",
};

static const int image_length = 12;
static const int cryptocurrency_length = 4;

lv_obj_t *last_setting_screen;
int __g_selected_cryptocurrency;
int __g_selected_image;
weather_type_t __g_weather;

/**********************  display cfg **********************/
void __save_settings(void) {
    struct view_data_display data_config;
    data_config.brightness = lv_slider_get_value(screen_setting_brightness_slider);
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG_APPLY, &data_config, sizeof(data_config), portMAX_DELAY);

    struct view_data_time_cfg time_config;
    if( lv_obj_has_state( screen_setting_24_hour_clock, LV_STATE_CHECKED) ) {
        time_config.time_format_24 = true;
    } else {
        time_config.time_format_24 = false;
    }
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_TIME_CFG_APPLY, &time_config, sizeof(time_config), portMAX_DELAY);
}

void render_cryptocurrency(int index) {
    ESP_LOGI(TAG, "rendering cryptocurrency %d", index);
    char page_text[40];

    int current_page = index + 1; 
    sprintf(page_text, "%d / %d", current_page, cryptocurrency_length);
    
    screen_cryptocurrency_image = lv_img_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_image, LV_SIZE_CONTENT);
    lv_obj_set_height( screen_cryptocurrency_image, LV_SIZE_CONTENT); 
    lv_obj_set_align( screen_cryptocurrency_image, LV_ALIGN_TOP_MID );
    lv_obj_set_y( screen_cryptocurrency_image, 10 );
    lv_img_set_src(screen_cryptocurrency_image, crypto_image_sources[index]);
    lv_label_set_text(screen_cryptocurrency_text, crypto_descriptions[index]);

    view_cryptocurrency_data* cryptocurrency_data = get_cryptocurrency_data();
    double value, value_24;
    switch (index) {
        default:
        case 0: {
            value = cryptocurrency_data->bitcoin.value;
            value_24 = cryptocurrency_data->bitcoin.value_24;
            break;
        }
        case 1: {
            value = cryptocurrency_data->ethereum.value;
            value_24 = cryptocurrency_data->ethereum.value_24;
            break;
        }
        case 2: {
            value = cryptocurrency_data->dogecoin.value;
            value_24 = cryptocurrency_data->dogecoin.value_24;
            break;
        }
        case 3: {
            value = cryptocurrency_data->ripple.value;
            value_24 = cryptocurrency_data->ripple.value_24;
            break;
        }
    }

    char cryptocurrency_value[20];
    sprintf(cryptocurrency_value, "%s%0.2f", CURRENCY_SYMBOL, value);
    lv_label_set_text(screen_cryptocurrency_value, cryptocurrency_value);
    
    double change_percent;
    if (value_24 == 0.0) {
        change_percent = 0.0;
    } else {
        change_percent = (value - value_24) / value_24 * 100.0;
    }
    change_percent = round(change_percent * 100) / 100;
    char cryptocurrency_percent[20];
    sprintf(cryptocurrency_percent, "%0.2f%%", change_percent);
    lv_label_set_text(screen_cryptocurrency_change_percent, cryptocurrency_percent);
    lv_label_set_text(screen_cryptocurrency_page, page_text);    
}

void render_browser_image(int index) {
    ESP_LOGI(TAG, "rendering browser image %d", index);
    screen_browser_image = lv_gif_create(screen_browser);
    lv_obj_set_width( screen_browser_image, LV_SIZE_CONTENT);
    lv_obj_set_height( screen_browser_image, LV_SIZE_CONTENT); 
    lv_obj_set_align( screen_browser_image, LV_ALIGN_CENTER );
    lv_gif_set_src(screen_browser_image, image_sources[index]);
}

void render_weather(void) {
    ESP_LOGI(TAG, "rendering weather %d", __g_weather);
    screen_main_weather = lv_gif_create(screen_main);
    lv_obj_set_width( screen_main_weather, LV_SIZE_CONTENT);
    lv_obj_set_height( screen_main_weather, LV_SIZE_CONTENT); 
    lv_obj_set_x( screen_main_weather, 0 );
    lv_obj_set_y( screen_main_weather, 0 );
    lv_obj_set_align( screen_main_weather, LV_ALIGN_TOP_MID );
    switch(__g_weather) {
        case CLOUDY: {
            lv_gif_set_src(screen_main_weather, &ui_weather_cloudy);
            break;
        }
        case CLEAR_DAY: {
            lv_gif_set_src(screen_main_weather, &ui_weather_clear_day);
            break;
        }
        case CLEAR_NIGHT: {
            lv_gif_set_src(screen_main_weather, &ui_weather_clear_night);
            break;
        }
        case PARTLY_CLOUDY_DAY: {
            lv_gif_set_src(screen_main_weather, &ui_weather_partly_cloudy_day);
            break;
        }
        case PARTLY_CLOUDY_NIGHT: {
            lv_gif_set_src(screen_main_weather, &ui_weather_partly_cloudy_night);
            break;
        }
        case SLEET: {
            lv_gif_set_src(screen_main_weather, &ui_weather_sleet);
            break;
        }
        case SNOWY: {
            lv_gif_set_src(screen_main_weather, &ui_weather_snow);
            break;
        }
        case RAINY: {
            lv_gif_set_src(screen_main_weather, &ui_weather_rain);
            break;
        }
    }     
}

void render_main(lv_scr_load_anim_t animation_flag) {
    render_weather();
    _ui_screen_change( screen_main, animation_flag, 200, 0);
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

        case VIEW_EVENT_DISPLAY_CFG: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_DISPLAY_CFG");
            struct view_data_display *p_cfg = ( struct view_data_display *)event_data;
            
            lv_slider_set_value(screen_setting_brightness_slider, p_cfg->brightness, LV_ANIM_OFF);
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

            lv_img_set_src(screen_setting_24_hour_clock_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_brightness_wifi , (void *)p_src);
            lv_img_set_src(screen_setting_wifi , (void *)p_src);
            break;
        }

        case VIEW_EVENT_CRYPTOCURRENCY: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_CRYPTOCURRENCY");
            if (current_screen == screen_cryptocurrency) {
                render_cryptocurrency(__g_selected_cryptocurrency);
            }
            break;
        }

        case VIEW_EVENT_WEATHER: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WEATHER");
            __g_weather = *( weather_type_t* )event_data;
            
            if (current_screen == screen_main) {
                lv_obj_clean(screen_main_weather);
                lv_obj_del_async(screen_main_weather);
                render_weather();
            }
            break;
        }
        
        case VIEW_EVENT_TIME_CFG_UPDATE: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME_CFG_UPDATE");
            struct view_data_time_cfg *p_cfg = ( struct view_data_time_cfg *)event_data;
            
            if(  p_cfg->time_format_24 ) {
                lv_obj_clear_state( screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
            } else {
                lv_obj_add_state( screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
            }          
            break;
        }
        case VIEW_EVENT_SENSOR_DATA: {
            int64_t current_time = esp_timer_get_time() / 1000;  
            if (current_screen == screen_main) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;

                if (p_data->type == GESTURE_SWIPE_LEFT || p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    lv_obj_clean(screen_main_weather);
                    lv_obj_del_async(screen_main_weather);
                }

                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    render_cryptocurrency(__g_selected_cryptocurrency);
                    _ui_screen_change( screen_cryptocurrency, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    _ui_screen_change( last_setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                }
            } else if (current_screen == screen_browser) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;

                if (p_data->type == GESTURE_SWIPE_LEFT || p_data->type == GESTURE_SWIPE_RIGHT) {
                    lv_obj_clean(screen_browser_image);
                    lv_obj_del_async(screen_browser_image);
                }

                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    _ui_screen_change( last_setting_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    render_cryptocurrency(__g_selected_cryptocurrency);
                    _ui_screen_change( screen_cryptocurrency, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                } 
            } else if (current_screen == screen_cryptocurrency) {
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;

                if (p_data->type == GESTURE_SWIPE_LEFT || p_data->type == GESTURE_SWIPE_RIGHT) {
                    lv_obj_clean(screen_cryptocurrency_image);
                    lv_obj_del_async(screen_cryptocurrency_image);
                }

                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    render_browser_image(__g_selected_image);
                    _ui_screen_change( screen_browser, LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 0);
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    render_main(LV_SCR_LOAD_ANIM_MOVE_RIGHT);
                } else if ((p_data->type == GESTURE_ROTATE_LEFT || p_data->type == GESTURE_ROTATE_RIGHT) && 
                        (current_time - __g_last_rotation_time > ROTATION_COOLDOWN_MS)) {
                    __g_last_rotation_time = current_time;
                    if (p_data->type == GESTURE_ROTATE_LEFT) {
                        __g_selected_cryptocurrency -= 1;
                        __g_selected_cryptocurrency += cryptocurrency_length;
                        __g_selected_cryptocurrency %= cryptocurrency_length;
                    } else if (p_data->type == GESTURE_ROTATE_RIGHT) {
                        __g_selected_cryptocurrency += 1;
                        __g_selected_cryptocurrency %= cryptocurrency_length;
                    }
                    render_cryptocurrency(__g_selected_cryptocurrency);
                }
            } else if (current_screen == screen_setting_24_hour_clock ||
                current_screen == screen_setting_brightness ||
                current_screen == screen_setting
                ) {
                last_setting_screen = current_screen;
                struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;
                if (p_data->type == GESTURE_SWIPE_LEFT  ) {
                    __save_settings();
                    render_main(LV_SCR_LOAD_ANIM_MOVE_LEFT);
                } else if (p_data->type == GESTURE_ROTATE_RIGHT || p_data->type == GESTURE_ROTATE_LEFT  ) {
                    if (current_screen == screen_setting_24_hour_clock) {
                        if (p_data->type == GESTURE_ROTATE_LEFT) {
                            lv_obj_clear_state(screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
                        } else if (p_data->type == GESTURE_ROTATE_RIGHT  ) {
                            lv_obj_add_state(screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
                        }
                    } else if (current_screen == screen_setting_brightness) {
                        if (p_data->type == GESTURE_ROTATE_LEFT) {
                            int slider_value = lv_slider_get_value(screen_setting_brightness_slider);
                            slider_value = slider_value - 5;
                            if (slider_value < 0) {
                                slider_value = 0;
                            }
                            lv_slider_set_value(screen_setting_brightness_slider, slider_value, LV_ANIM_ON);    
                            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_BRIGHTNESS_UPDATE, &slider_value, sizeof(slider_value), portMAX_DELAY);
                        } else if (p_data->type == GESTURE_ROTATE_RIGHT) {
                            int slider_value = lv_slider_get_value(screen_setting_brightness_slider);
                            slider_value = slider_value + 5;
                            if (slider_value > 100) {
                                slider_value = 100;
                            }
                            lv_slider_set_value(screen_setting_brightness_slider, slider_value, LV_ANIM_ON);
                            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_BRIGHTNESS_UPDATE, &slider_value, sizeof(slider_value), portMAX_DELAY);
                        }

                    }
                } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
                    __save_settings();                    
                    render_browser_image(__g_selected_image);
                    _ui_screen_change( screen_browser, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 0);
                } else if (p_data->type == GESTURE_GRAB) {
                    __save_settings();
                    lv_obj_t *next_screen;
                    if (current_screen == screen_setting) {
                        next_screen = screen_setting_24_hour_clock;
                    } else if (current_screen == screen_setting_24_hour_clock) {
                        next_screen = screen_setting_brightness;
                    } else {
                        next_screen = screen_setting;
                    }
                    _ui_screen_change( next_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 200, 0);
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
    __g_selected_cryptocurrency = 0;
    __g_selected_image = 0;
    __g_weather = CLEAR_DAY;
    render_weather();

    current_screen = screen_main;
    last_setting_screen = screen_setting;
    
    int i  = 0;
    for( i = 0; i < VIEW_EVENT_ALL; i++ ) {
        ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                                VIEW_EVENT_BASE, i, 
                                                                __view_event_handler, NULL, NULL));
    }

    return 0;
}