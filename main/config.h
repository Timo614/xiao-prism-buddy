

#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "prism_storage.h"
#include <stdbool.h>
#include <string.h> 
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event_base.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USER_CURRENCY "usd"
#define CURRENCY_SYMBOL "$"

ESP_EVENT_DECLARE_BASE(VIEW_EVENT_BASE);

extern esp_event_loop_handle_t view_event_handle;



#ifdef __cplusplus
}
#endif

#endif