#include "prism_sensor.h"

pag7660 sensor;
static const char *TAG = "sensor-model";

static SemaphoreHandle_t  __g_data_mutex;
static pag7660_gesture_t  __g_sensor_state;

const char *cursor_str[] = {
    NULL,
    "Tap",
    "Grab",
    "Pinch",
};

static void gesture_sensor_poll_task(void *arg)
{
    for( ;; )
    {
        bool gesture_found = true;
        xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
        if (sensor.getResult(__g_sensor_state)) {
            switch (__g_sensor_state.type) {
            case 0:
                switch (__g_sensor_state.cursor.type) {
                case 1:
                    if (__g_sensor_state.cursor.select)
                        ESP_LOGI(TAG, "Tap");
                    break;
                default:
                    break;
                }
                break;
            case 6:
                ESP_LOGI(TAG, "Rotate Right %d", __g_sensor_state.rotate);
                break;
            case 7:
                ESP_LOGI(TAG, "Rotate Left %d", __g_sensor_state.rotate);
                break;
            case 8:
                ESP_LOGI(TAG, "Swipe Left");
                break;
            case 9:
                ESP_LOGI(TAG, "Swipe Right");
                break;
            default:
                gesture_found = false;
                break;
            }
        }       

        xSemaphoreGive(__g_data_mutex);
        
        if (gesture_found) {
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_SENSOR_DATA, NULL, 0, portMAX_DELAY);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int prism_sensor_init(void)
{
    __g_data_mutex  =  xSemaphoreCreateMutex();

    if (!sensor.init()) {
        ESP_LOGE(TAG, "PAJ7620 I2C error - halting");
        while (true) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    ESP_LOGI(TAG, "PAJ7620 init: OK");
    

    xTaskCreate(gesture_sensor_poll_task, "gesture_sensor_poll_task", 1024, NULL, 2, NULL);

    return 0;
}