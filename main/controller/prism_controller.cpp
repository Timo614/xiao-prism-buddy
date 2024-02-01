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

#define ROTATION_COOLDOWN_MS 1500  // Cooldown period in milliseconds


/**********************
 *  STATIC VARIABLES
 **********************/

static int64_t __g_last_rotation_time = 0;  // Timestamp of the last rotation

static const int __g_cryptocurrency_length = 4;
static const int __g_image_length = 2;

static lv_obj_t *last_setting_screen;
static int __g_selected_cryptocurrency;
static int __g_selected_image;
static char *__g_city;

static struct view_weather_data __g_weather_data = {
    .weather = CLEAR_DAY,
    .humidity = 0,
    .temperature = 0.0,
};

static struct view_cryptocurrency_data __g_cryptocurrency_data = {
    .bitcoin = {
        .value = 0.0,
        .value_24 = 0.0
    },
    .ethereum = {
        .value = 0.0,
        .value_24 = 0.0
    },
    .ripple = {
        .value = 0.0,
        .value_24 = 0.0
    },
    .dogecoin = {
        .value = 0.0,
        .value_24 = 0.0
    }
};

static bool __g_time_format_24;

static const char *TAG = "controller";

static const void *__g_crypto_image_sources[] = {
    &ui_coins_btc,
    &ui_coins_eth,
    &ui_coins_doge,
    &ui_coins_xrp
};

static const char *__g_crypto_descriptions[] = {
    "Bitcoin",
    "Ethereum",
    "Dogecoin",
    "Ripple",
};

static const void *__g_image_sources[] = {
    &ui_gifs_watermelon,
    &ui_gifs_fire,
};

void update_time(void);

/**********************  display cfg **********************/
void __save_settings(void) {
    
    ESP_LOGI(TAG, "saving settings");
    struct view_data_display data_config;
    data_config.brightness = lv_slider_get_value(screen_setting_brightness_slider);
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG_APPLY, &data_config, sizeof(data_config), portMAX_DELAY);

    struct view_data_time_cfg time_config;
    if( lv_obj_has_state( screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED) ) {
        __g_time_format_24 = true;
    } else {
        __g_time_format_24 = false;
    }
    time_config.time_format_24 = __g_time_format_24;
    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_TIME_CFG_APPLY, &time_config, sizeof(time_config), portMAX_DELAY);
}

void render_cryptocurrency(int index) {
    ESP_LOGI(TAG, "rendering cryptocurrency %d", index);
    int current_page = index + 1; 
    
    double value, value_24;
    switch (index) {
        default:
        case 0: {
            value = __g_cryptocurrency_data.bitcoin.value;
            value_24 = __g_cryptocurrency_data.bitcoin.value_24;
            break;
        }
        case 1: {
            value = __g_cryptocurrency_data.ethereum.value;
            value_24 = __g_cryptocurrency_data.ethereum.value_24;
            break;
        }
        case 2: {
            value = __g_cryptocurrency_data.dogecoin.value;
            value_24 = __g_cryptocurrency_data.dogecoin.value_24;
            break;
        }
        case 3: {
            value = __g_cryptocurrency_data.ripple.value;
            value_24 = __g_cryptocurrency_data.ripple.value_24;
            break;
        }
    }

    lv_obj_t * cryptocurrency = lv_obj_create(NULL);
    lv_obj_clear_flag( cryptocurrency, LV_OBJ_FLAG_SCROLLABLE );
    
    lv_obj_t * cryptocurrency_text = lv_label_create(cryptocurrency);
    lv_obj_set_width( cryptocurrency_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( cryptocurrency_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( cryptocurrency_text, LV_ALIGN_TOP_LEFT );
    lv_obj_set_y( cryptocurrency_text, 40 );
    lv_obj_set_x( cryptocurrency_text, 40 ); 
    lv_obj_set_style_text_font(cryptocurrency_text, &lv_font_montserrat_30, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_label_set_text(cryptocurrency_text, __g_crypto_descriptions[index]);

    lv_obj_t * cryptocurrency_value = lv_label_create(cryptocurrency);
    lv_obj_set_width( cryptocurrency_value, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( cryptocurrency_value, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( cryptocurrency_value, LV_ALIGN_LEFT_MID );    
    lv_obj_set_y( cryptocurrency_value, -20 );   
    lv_obj_set_x( cryptocurrency_value, 40 ); 
    lv_label_set_recolor(cryptocurrency_value, true); 
    if (value_24 < 0) {
        lv_label_set_text_fmt(cryptocurrency_value, "%s#ff0000 %0.2f#", CURRENCY_SYMBOL, value);
        
    } else {
        lv_label_set_text_fmt(cryptocurrency_value, "%s#00ff00 %0.2f#", CURRENCY_SYMBOL, value);
    }
    lv_obj_set_style_text_font(cryptocurrency_value, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_t * cryptocurrency_change_24_hours = lv_label_create(cryptocurrency);
    lv_obj_set_width( cryptocurrency_change_24_hours, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( cryptocurrency_change_24_hours, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( cryptocurrency_change_24_hours, LV_ALIGN_RIGHT_MID );      
    lv_label_set_recolor(cryptocurrency_change_24_hours, true); 
    lv_obj_set_x( cryptocurrency_change_24_hours, -40 );   
    lv_obj_set_y( cryptocurrency_change_24_hours, 30 );
    lv_obj_set_style_text_font(cryptocurrency_change_24_hours, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
    if (value_24 < 0) {
        lv_label_set_text_fmt(cryptocurrency_change_24_hours, "24H #ff0000 (%0.2f)#", value_24 * -1.0);
        
    } else {
        lv_label_set_text_fmt(cryptocurrency_change_24_hours, "24H #00ff00 %0.2f#", value_24 * 1.0);
    }

    lv_obj_t * cryptocurrency_page = lv_label_create(cryptocurrency);
    lv_obj_set_width( cryptocurrency_page, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( cryptocurrency_page, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_y( cryptocurrency_page, -20 );
    lv_obj_set_align( cryptocurrency_page, LV_ALIGN_BOTTOM_MID );
    lv_obj_set_style_text_font(cryptocurrency_page, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_label_set_text_fmt(cryptocurrency_page, "%d / %d", current_page, __g_cryptocurrency_length);

    lv_obj_t * cryptocurrency_image = lv_img_create(cryptocurrency);
    lv_obj_set_width( cryptocurrency_image, LV_SIZE_CONTENT);
    lv_obj_set_height( cryptocurrency_image, LV_SIZE_CONTENT); 
    lv_obj_set_align( cryptocurrency_image, LV_ALIGN_TOP_RIGHT );
    lv_obj_set_y( cryptocurrency_image, 40 );
    lv_obj_set_x( cryptocurrency_image, -40 );
    lv_img_set_src(cryptocurrency_image, __g_crypto_image_sources[index]);

    screen_cryptocurrency = cryptocurrency;
    current_screen = screen_cryptocurrency;
}

void render_browser_image(int index) {
    ESP_LOGI(TAG, "rendering browser image %d", index);
    
    lv_obj_t * browser = lv_obj_create(NULL);
    lv_obj_clear_flag( browser, LV_OBJ_FLAG_SCROLLABLE );
    lv_obj_t * browser_image = lv_gif_create(browser);
    lv_obj_set_width( browser_image, LV_SIZE_CONTENT);
    lv_obj_set_height( browser_image, LV_SIZE_CONTENT); 
    lv_obj_set_align( browser_image, LV_ALIGN_CENTER );
    lv_gif_set_src(browser_image, __g_image_sources[index]);
    
    screen_browser_image = browser_image;
    screen_browser = browser;
}

void render_main() {
    lv_obj_t * main = lv_obj_create(NULL);
    lv_obj_clear_flag( main, LV_OBJ_FLAG_SCROLLABLE );

    lv_obj_t * main_time = lv_label_create(main);
    lv_obj_set_y( main_time, -30 );
    lv_obj_set_x( main_time, 0 );
    lv_obj_set_align( main_time, LV_ALIGN_BOTTOM_MID );
    lv_obj_clear_flag( main_time, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_text_font(main_time, &lv_font_montserrat_48, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_t * main_city = lv_label_create(main);
    lv_obj_set_y( main_city, 30 );
    lv_obj_set_x( main_city, 20 );
    lv_obj_set_align( main_city, LV_ALIGN_TOP_LEFT );
    lv_obj_clear_flag( main_city, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_text_font(main_city, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    if (__g_city == NULL) {
        lv_label_set_text(main_city, "The World");
    } else {
        lv_label_set_text(main_city, __g_city);
    }

    lv_obj_t * main_humidity = lv_label_create(main);
    lv_obj_set_y( main_humidity, 60 );
    lv_obj_set_x( main_humidity, 120 );
    lv_obj_set_align( main_humidity, LV_ALIGN_TOP_LEFT );
    lv_obj_clear_flag( main_humidity, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_label_set_text_fmt(main_humidity, "%d %%", __g_weather_data.humidity);
    lv_obj_set_style_text_font(main_humidity, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);

    lv_obj_t * main_temperature = lv_label_create(main);
    lv_obj_set_y( main_temperature, 60 );
    lv_obj_set_x( main_temperature, 20 );
    lv_obj_set_align( main_temperature, LV_ALIGN_TOP_LEFT );
    lv_obj_clear_flag( main_temperature, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_label_set_text_fmt(main_temperature, "%2.1f C", __g_weather_data.temperature);
    lv_obj_set_style_text_font(main_temperature, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);

    ESP_LOGI(TAG, "rendering weather %d", __g_weather_data.weather);    
    lv_obj_t * main_weather = lv_gif_create(main);
    lv_obj_set_width( main_weather, LV_SIZE_CONTENT);
    lv_obj_set_height( main_weather, LV_SIZE_CONTENT); 
    lv_obj_set_x( main_weather, -10 );
    lv_obj_set_y( main_weather, 10 );
    lv_obj_set_align( main_weather, LV_ALIGN_TOP_RIGHT );
    switch(__g_weather_data.weather) {
        case CLOUDY: {
            lv_gif_set_src(main_weather, &ui_weather_cloudy);
            break;
        }
        case CLEAR_DAY: {
            lv_gif_set_src(main_weather, &ui_weather_clear_day);
            break;
        }
        case CLEAR_NIGHT: {
            lv_gif_set_src(main_weather, &ui_weather_clear_night);
            break;
        }
        case PARTLY_CLOUDY_DAY: {
            lv_gif_set_src(main_weather, &ui_weather_partly_cloudy_day);
            break;
        }
        case PARTLY_CLOUDY_NIGHT: {
            lv_gif_set_src(main_weather, &ui_weather_partly_cloudy_night);
            break;
        }
        case SLEET: {
            lv_gif_set_src(main_weather, &ui_weather_sleet);
            break;
        }
        case SNOWY: {
            lv_gif_set_src(main_weather, &ui_weather_snow);
            break;
        }
        case RAINY: {
            lv_gif_set_src(main_weather, &ui_weather_rain);
            break;
        }
    }     

    screen_main = main;
    screen_main_time = main_time;
    screen_main_weather = main_weather;
    update_time();
}

void update_time(void)
{
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
    int hour = timeinfo.tm_hour;

    if( ! __g_time_format_24 ) {
        if( hour>=13 && hour<=23) {
            hour = hour-12;
        }
        else if (hour == 0) {
            hour = 12;
        }
    } 

    lv_label_set_text_fmt(screen_main_time, "%02d:%02d", hour, timeinfo.tm_min);
}

void event_wifi_status(void * event_data) {
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
}

void sensor_data_update(void * event_data) {
    struct gesture_event_t  *p_data = (struct gesture_event_t *) event_data;

    int64_t current_time = esp_timer_get_time() / 1000;  
    bool clean_screen = p_data->type == GESTURE_SWIPE_LEFT || p_data->type == GESTURE_SWIPE_RIGHT;

    if (current_screen == screen_main) {
        if (p_data->type == GESTURE_SWIPE_LEFT  ) {
            render_cryptocurrency(__g_selected_cryptocurrency);
            _ui_screen_change( screen_cryptocurrency);
        } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
            _ui_screen_change( last_setting_screen);
        }

        if (clean_screen) {
            lv_obj_del(screen_main);
            screen_main = NULL;
        }
    } else if (current_screen == screen_browser) {
        if (p_data->type == GESTURE_SWIPE_LEFT  ) {
            _ui_screen_change( last_setting_screen);
        } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
            render_cryptocurrency(__g_selected_cryptocurrency);
            _ui_screen_change( screen_cryptocurrency);
        } else if ((p_data->type == GESTURE_ROTATE_LEFT || p_data->type == GESTURE_ROTATE_RIGHT) && 
                (current_time - __g_last_rotation_time > ROTATION_COOLDOWN_MS)) {
            __g_last_rotation_time = current_time;
            if (p_data->type == GESTURE_ROTATE_LEFT) {
                __g_selected_image -= 1;
                __g_selected_image += __g_image_length;
                __g_selected_image %= __g_image_length;
            } else if (p_data->type == GESTURE_ROTATE_RIGHT) {
                __g_selected_image += 1;
                __g_selected_image %= __g_image_length;
            }
            lv_obj_t * old_screen = screen_browser;
            lv_obj_clean(lv_scr_act());
            vTaskDelay(pdMS_TO_TICKS(500));

            render_browser_image(__g_selected_image);
            _ui_screen_change( screen_browser);
            lv_obj_del(old_screen);
        } 
        if (clean_screen) {
            lv_obj_del(screen_browser);
            screen_browser = NULL;
        } 
    } else if (current_screen == screen_cryptocurrency) {     
        if (p_data->type == GESTURE_SWIPE_LEFT  ) {
            render_browser_image(__g_selected_image);
            _ui_screen_change( screen_browser);
        } else if (p_data->type == GESTURE_SWIPE_RIGHT  ) {
            render_main();
            _ui_screen_change( screen_main);
        } else if ((p_data->type == GESTURE_ROTATE_LEFT || p_data->type == GESTURE_ROTATE_RIGHT) && 
                (current_time - __g_last_rotation_time > ROTATION_COOLDOWN_MS)) {
            __g_last_rotation_time = current_time;
            if (p_data->type == GESTURE_ROTATE_LEFT) {
                __g_selected_cryptocurrency -= 1;
                __g_selected_cryptocurrency += __g_cryptocurrency_length;
                __g_selected_cryptocurrency %= __g_cryptocurrency_length;
            } else if (p_data->type == GESTURE_ROTATE_RIGHT) {
                __g_selected_cryptocurrency += 1;
                __g_selected_cryptocurrency %= __g_cryptocurrency_length;
            }
            lv_obj_t * old_screen = screen_cryptocurrency;
            render_cryptocurrency(__g_selected_cryptocurrency);
            _ui_screen_change( screen_cryptocurrency);
            lv_obj_del(old_screen);
        }      
        if (clean_screen) {
            lv_obj_del(screen_cryptocurrency);
            screen_cryptocurrency = NULL;
        } 

    } else if (current_screen == screen_setting_24_hour_clock ||
        current_screen == screen_setting_brightness ||
        current_screen == screen_setting
        ) {
        last_setting_screen = current_screen;
        if (p_data->type == GESTURE_SWIPE_LEFT  ) {
            __save_settings();
            render_main();
            _ui_screen_change( screen_main);
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
            _ui_screen_change( screen_browser);
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
            _ui_screen_change( next_screen);
        }
    }
}

static void __view_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    switch (id)
    {
        case VIEW_EVENT_TIME: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME");
            if( event_data) {
                __g_time_format_24 = *( bool *)event_data;
            } 
    
            if (current_screen == screen_main) {
                lv_port_sem_take();
                update_time();
                lv_port_sem_give();
            }
            break;
        }
        case VIEW_EVENT_CITY: {
            char *p_data = (char *)event_data;

            if (p_data == NULL) {
                break;
            }

            if (__g_city != NULL) {
                free(__g_city);
            }

            __g_city = (char *)malloc(strlen(p_data) + 1); 
            if (__g_city == NULL) {
                break;
            }

            strcpy(__g_city, p_data);

            if (current_screen == screen_main) {
                lv_obj_t *old_screen = screen_main;
                render_main();
                _ui_screen_change(screen_main);
                lv_obj_del(old_screen);
            }
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
            lv_async_call(event_wifi_status, event_data);
            break;
        }
        case VIEW_EVENT_CRYPTOCURRENCY: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_CRYPTOCURRENCY");
            view_cryptocurrency_data * cryptocurrency_data = ( view_cryptocurrency_data* )event_data;
            __g_cryptocurrency_data.bitcoin.value = cryptocurrency_data->bitcoin.value;
            __g_cryptocurrency_data.bitcoin.value_24 = cryptocurrency_data->bitcoin.value_24;
            __g_cryptocurrency_data.ethereum.value = cryptocurrency_data->ethereum.value;
            __g_cryptocurrency_data.ethereum.value_24 = cryptocurrency_data->ethereum.value_24;
            __g_cryptocurrency_data.ripple.value = cryptocurrency_data->ripple.value;
            __g_cryptocurrency_data.ripple.value_24 = cryptocurrency_data->ripple.value_24;
            __g_cryptocurrency_data.dogecoin.value = cryptocurrency_data->dogecoin.value;
            __g_cryptocurrency_data.dogecoin.value_24 = cryptocurrency_data->dogecoin.value_24;
                        
            if (current_screen == screen_cryptocurrency) {
                lv_obj_t * old_screen = screen_cryptocurrency;        
                lv_port_sem_take();
                lv_obj_clean(lv_scr_act());
                lv_port_sem_give();
                vTaskDelay(pdMS_TO_TICKS(500));
                
                render_cryptocurrency(__g_selected_cryptocurrency);
                    
                lv_port_sem_take();
                render_main();
                _ui_screen_change( screen_cryptocurrency);
                lv_obj_del(old_screen);
                lv_port_sem_give();
            }
            break;
        }
        case VIEW_EVENT_WEATHER: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WEATHER");
            view_weather_data * weather_data = ( view_weather_data* )event_data;
            weather_type_t old_weather = __g_weather_data.weather;
            __g_weather_data.weather = weather_data->weather;
            __g_weather_data.temperature = weather_data->temperature;
            __g_weather_data.humidity = weather_data->humidity;
            
            if (current_screen == screen_main) {
                lv_obj_t * old_screen = screen_main;
                lv_port_sem_take();
                lv_obj_clean(lv_scr_act());
                lv_port_sem_give();
                vTaskDelay(pdMS_TO_TICKS(500));
                
                lv_port_sem_take();
                render_main();
                _ui_screen_change( screen_main);
                lv_obj_del(old_screen);
                lv_port_sem_give();
            }
            break;
        }
        case VIEW_EVENT_TIME_CFG_UPDATE: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_TIME_CFG_UPDATE");
            struct view_data_time_cfg *p_cfg = ( struct view_data_time_cfg *)event_data;
    
            if(  p_cfg->time_format_24 ) {
                lv_obj_add_state( screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
            } else {
                lv_obj_clear_state( screen_setting_24_hour_clock_toggle, LV_STATE_CHECKED);
            }     
            break;
        }
        case VIEW_EVENT_SENSOR_DATA: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_SENSOR_DATA");
            lv_port_sem_take();
            sensor_data_update(event_data);
            lv_port_sem_give();
            break;
        }
        default:
            break;
    }
}

int prism_controller_init(void)
{   
    __g_selected_cryptocurrency = 0;
    __g_selected_image = 0;
    __g_time_format_24 = false;
    render_main();
    lv_scr_load( screen_main);

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