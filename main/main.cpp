#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_event_base.h"

#include "lv_port.h"
#include "prism_model.h"
#include "prism_controller.h"
#include "prism_view.h"
#include "prism_storage.h"

static const char *TAG = "app_main";

#define VERSION   "v1.0.0"

#define PRISMBUDDY "\n\
   ___          _                     ___            __     __       \n\
  / _ \\  ____  (_)  ___  __ _        / _ ) __ __ ___/ / ___/ /  __ __\n\
 / ___/ / __/ / /  (_-< /  ' \\      / _  |/ // // _  / / _  /  / // /\n\
/_/    /_/   /_/  /___//_/_/_/     /____/ \\_,_/ \\_,_/  \\_,_/   \\_, / \n\
                                                              /___/  \n\
--------------------------------------------------------\n\
 Version: %s %s %s\n\
--------------------------------------------------------\n\
"

ESP_EVENT_DEFINE_BASE(VIEW_EVENT_BASE);
esp_event_loop_handle_t view_event_handle;

extern "C" void app_main(void)
{
    ESP_LOGI("", PRISMBUDDY, VERSION, __DATE__, __TIME__);
    lv_port_init();

    esp_event_loop_args_t view_event_task_args = {
        .queue_size = 10,
        .task_name = "view_event_task",
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 4096,
        .task_core_id = tskNO_AFFINITY
    };

    ESP_ERROR_CHECK(esp_event_loop_create(&view_event_task_args, &view_event_handle));

    prism_storage_init();

    lv_port_sem_take();
    prism_view_init();
    lv_port_sem_give();

    lv_port_sem_take();
    prism_controller_init();
    lv_port_sem_give();

    prism_model_init();

    // static char buffer[128];    /* Make sure buffer is enough for `sprintf` */
    while (1) {
        // sprintf(buffer, "   Biggest /     Free /    Total\n"
        //         "\t  DRAM : [%8d / %8d / %8d]\n"
        //         "\t PSRAM : [%8d / %8d / %8d]",
        //         heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_total_size(MALLOC_CAP_INTERNAL),
        //         heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM),
        //         heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        //         heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
        // ESP_LOGI("MEM", "%s", buffer);
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}