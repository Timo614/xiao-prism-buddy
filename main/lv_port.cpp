#include "esp_heap_caps.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "rom/cache.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "lv_port.h"

#define LGFX_USE_V1
#include <lvgl.h>

#define LV_PORT_TASK_DELAY_MS           (10)
#define LV_PORT_TICK_PERIOD_MS          (2)
#define BUFF_SIZE 40

LGFX lcd;

static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;

static const char *TAG = "lvgl_port";

static lv_disp_drv_t disp_drv;
static SemaphoreHandle_t lvgl_mutex = NULL;
static TaskHandle_t lvgl_task_handle;

#ifndef CONFIG_LCD_TASK_PRIORITY
#define CONFIG_LCD_TASK_PRIORITY    5
#endif

static void lv_port_disp_init(void);
static esp_err_t lv_port_tick_init(void);
static void lvgl_task(void *args);
static void ui_task(void *args);
static void flush_cb( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p );

/**
 * @brief Initialize lv port
 */
void lv_port_init(void)
{
    lv_port_disp_init();
    lv_port_tick_init();

    lvgl_mutex = xSemaphoreCreateMutex();
    xTaskCreate(lvgl_task, "lvgl_task", 4096 * 4, NULL, CONFIG_LCD_TASK_PRIORITY, &lvgl_task_handle);
}

/**
 * @brief Take semaphore
 */
void lv_port_sem_take(void)
{
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    if (lvgl_task_handle != task) {
        xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
    }
}

/**
 * @brief Give semaphore
 */
void lv_port_sem_give(void)
{
    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    if (lvgl_task_handle != task) {
        xSemaphoreGive(lvgl_mutex);
    }
}

/**
 * @brief Initialize display driver for LVGL.
 *
 */
static void lv_port_disp_init(void)
{
    lcd.init();  
    lv_init(); 
    
    lcd.setRotation(3);
    lcd.setColorDepth(16);
    lcd.fillScreen(TFT_BLACK);

    lv_color_t * buf = (lv_color_t *)heap_caps_malloc(screenWidth * BUFF_SIZE * sizeof(lv_color_t),MALLOC_CAP_DMA);
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * BUFF_SIZE );
    lv_disp_drv_init( &disp_drv );
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
}

/**
 * @brief Task to generate ticks for LVGL.
 *
 * @param pvParam Not used.
 */
static void lv_tick_inc_cb(void *arg)
{
    (void)arg;
    lv_tick_inc(LV_PORT_TICK_PERIOD_MS);
}

/**
 * @brief Create tick task for LVGL.
 *
 * @return esp_err_t
 */
static esp_err_t lv_port_tick_init(void)
{
    static const uint32_t tick_inc_period_ms = 2;
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = lv_tick_inc_cb,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "periodic",
        .skip_unhandled_events  = true
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_PORT_TICK_PERIOD_MS * 1000));    

    return ESP_OK;
}

/**
 * @brief Task to draw and flush for LVGL.
 *
 * @param args Not used.
 */
static void lvgl_task(void *args)
{
    for (;;) {
        xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
        lv_task_handler();
        xSemaphoreGive(lvgl_mutex);
        vTaskDelay(pdMS_TO_TICKS(LV_PORT_TASK_DELAY_MS));
    }
}

/**
 * @brief Callback to flush display
 *
 * @param disp Pointer to the LVGL display driver, containing information about the display.
 * @param area Pointer to an lv_area_t structure that specifies the coordinates of the screen area to be flushed.
 * @param color_p Pointer to the buffer containing the pixel color data to be written to the display.
 */
static void flush_cb( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    lcd.startWrite();
    lcd.setAddrWindow( area->x1, area->y1, w, h );
    lcd.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    lcd.endWrite();

    lv_disp_flush_ready( disp );
}

 

