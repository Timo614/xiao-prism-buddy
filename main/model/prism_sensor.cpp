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
    gesture_event_t gesture_event;

    for(;;) {
        gesture_event.type = GESTURE_NONE;
        gesture_event.data = 0;

        xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
        if (sensor.getResult(__g_sensor_state)) {
            switch (__g_sensor_state.type) {
            case 0:
                if (__g_sensor_state.cursor.type == 1 && __g_sensor_state.cursor.select) {
                    ESP_LOGI(TAG, "Tap");
                    gesture_event.type = GESTURE_TAP;
                }
                break;
            case 6:
                ESP_LOGI(TAG, "Rotate Right %d", __g_sensor_state.rotate);
                gesture_event.type = GESTURE_ROTATE_RIGHT;
                gesture_event.data = __g_sensor_state.rotate;
                break;
            case 7:
                ESP_LOGI(TAG, "Rotate Left %d", __g_sensor_state.rotate);
                gesture_event.type = GESTURE_ROTATE_LEFT;
                gesture_event.data = __g_sensor_state.rotate;
                break;
            case 8:
                ESP_LOGI(TAG, "Swipe Left");
                gesture_event.type = GESTURE_SWIPE_LEFT;
                break;
            case 9:
                ESP_LOGI(TAG, "Swipe Right");
                gesture_event.type = GESTURE_SWIPE_RIGHT;
                break;
            default:
                break;
            }
        }
        xSemaphoreGive(__g_data_mutex);
        
        if (gesture_event.type != GESTURE_NONE) {
            esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_SENSOR_DATA, &gesture_event, sizeof(gesture_event_t), portMAX_DELAY);
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