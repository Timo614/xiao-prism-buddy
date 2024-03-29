#include "prism_display.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "nvs.h"
#include "lv_port.h"

#define DISPLAY_CFG_STORAGE  "display"

struct prism_display
{
    struct view_data_display cfg;
    bool timer_st; 
    bool display_st;
};

static const char *TAG = "display";

static struct prism_display __g_display_model;
static SemaphoreHandle_t       __g_data_mutex;

static esp_timer_handle_t      sleep_timer_handle;
static SemaphoreHandle_t       __g_timer_mutex;

static bool init_done_flag = false;
static void __display_cfg_set(struct view_data_display *p_data )
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    memcpy( &__g_display_model.cfg, p_data, sizeof(struct view_data_display));
    xSemaphoreGive(__g_data_mutex);
}

static void __display_cfg_get(struct view_data_display *p_data )
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    memcpy(p_data, &__g_display_model.cfg, sizeof(struct view_data_display));
    xSemaphoreGive(__g_data_mutex);
}

static void __timer_st_set( bool st )
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    __g_display_model.timer_st = st;
    xSemaphoreGive(__g_data_mutex);
}

static bool __timer_st_get(void)
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    bool flag =  __g_display_model.timer_st;
    xSemaphoreGive(__g_data_mutex);
    return flag;
}

static void __display_st_set( bool st )
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    __g_display_model.display_st = st;
    xSemaphoreGive(__g_data_mutex);
}

static bool __display_st_get(void)
{
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    bool st =  __g_display_model.display_st;
    xSemaphoreGive(__g_data_mutex);
    return st;
}


static void __display_cfg_print(struct view_data_display *p_data )
{
    ESP_LOGI(TAG, "brightness:%d",p_data->brightness );
}

static void __lcd_bl_set(int brightness )
{   
    if(brightness > 99) {
        brightness=99;
    } else if( brightness < 1) {
        brightness=1;
    }

    lcd.setBrightness(brightness);
}

static void __lcd_bl_on(void)
{
    struct view_data_display cfg;
    __display_cfg_get(&cfg);
    __lcd_bl_set(cfg.brightness);
    __display_st_set(true);
}

static void __lcd_bl_off(void)
{
    lcd.setBrightness(0);
    __display_st_set(false);
}

static void __timer_stop(void)
{
    if(  __timer_st_get()) {
        xSemaphoreTake(__g_timer_mutex, portMAX_DELAY);
        ESP_ERROR_CHECK(esp_timer_stop(sleep_timer_handle));
        xSemaphoreGive(__g_timer_mutex);
        __timer_st_set(false);
    }
}

static void __display_cfg_save(struct view_data_display *p_data ) 
{
    esp_err_t ret = 0;
    ret = prism_storage_write(DISPLAY_CFG_STORAGE, (void *)p_data, sizeof(struct view_data_display));
    if( ret != ESP_OK ) {
        ESP_LOGI(TAG, "cfg write err:%d", ret);
    } else {
        ESP_LOGI(TAG, "cfg write successful");
    }
}


static void __display_cfg_restore(void)
{
    esp_err_t ret = 0;
    struct view_data_display cfg;
    
    size_t len = sizeof(cfg);
    
    ret = prism_storage_read(DISPLAY_CFG_STORAGE, (void *)&cfg, &len);
    if( ret == ESP_OK  && len== (sizeof(cfg)) ) {
        ESP_LOGI(TAG, "cfg read successful");
        __display_cfg_set(&cfg);
    } else {
        // err or not find
        if( ret == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGI(TAG, "cfg not found");
        }else {
            ESP_LOGI(TAG, "cfg read err:%d", ret);
        } 
        
        cfg.brightness = 80;
        __display_cfg_set(&cfg);
    }
}

static void __view_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    switch (id)
    {
        case VIEW_EVENT_BRIGHTNESS_UPDATE: {
            int *p_brightness= (int *)event_data;
            struct view_data_display cfg;

            ESP_LOGI(TAG, "event: VIEW_EVENT_BRIGHTNESS_UPDATE, brightness:%d", *p_brightness);
         
            __lcd_bl_set(*p_brightness);
           
            __display_cfg_get(&cfg);
            cfg.brightness=*p_brightness;
            __display_cfg_set(&cfg);
            break;
        }
        case VIEW_EVENT_DISPLAY_CFG_APPLY: {

            struct view_data_display * p_cfg = (struct view_data_display *)event_data;
            ESP_LOGI(TAG, "event: VIEW_EVENT_DISPLAY_CFG_APPLY");
            __display_cfg_print(p_cfg);

            __display_cfg_set(p_cfg);
            __display_cfg_save(p_cfg);
            break;
        }
    
    default:
        break;
    }


}
int prism_display_init(void)
{
    struct view_data_display cfg;
    __g_data_mutex  =  xSemaphoreCreateMutex();
    __g_timer_mutex = xSemaphoreCreateMutex();

    __display_cfg_restore();

    __display_cfg_get(&cfg);

    __lcd_bl_set(cfg.brightness);
    __display_st_set(true);

    esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG, &cfg, sizeof(cfg), portMAX_DELAY);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                            VIEW_EVENT_BASE, VIEW_EVENT_DISPLAY_CFG_APPLY, 
                                                            __view_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                            VIEW_EVENT_BASE, VIEW_EVENT_BRIGHTNESS_UPDATE, 
                                                            __view_event_handler, NULL, NULL));

    init_done_flag = true;
    return 0;
}

bool prism_display_st_get(void)
{
    return __display_st_get();
}

void prism_display_on(void)
{
   __lcd_bl_on();
}

void prism_display_off(void)
{
    __lcd_bl_off();
    __timer_stop();
}
