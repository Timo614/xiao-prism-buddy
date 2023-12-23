#include "json_helper.h"

void parseJsonDouble(const char *tag, cJSON *parent, const char *key, double *output) {
    cJSON *item = cJSON_GetObjectItem(parent, key);
    if (item != NULL) {
        *output = item->valuedouble;
        ESP_LOGI(tag, "%s: %f", key, *output);
    } else {
        ESP_LOGI(tag, "%s not found", key);
    }
}

void parseJsonInt(const char *tag, cJSON *parent, const char *key, int *output) {
    cJSON *item = cJSON_GetObjectItem(parent, key);
    if (item != NULL) {
        *output = item->valueint;
        ESP_LOGI(tag, "%s: %d", key, *output);
    } else {
        ESP_LOGI(tag, "%s not found", key);
    }
}

void parseJsonBool(const char *tag, cJSON *parent, const char *key, bool *output) {
    cJSON *item = cJSON_GetObjectItem(parent, key);
    if (item != NULL) {
        *output = (item->valueint == 1);
        ESP_LOGI(tag, "%s: %s", key, *output ? "true" : "false");
    } else {
        ESP_LOGI(tag, "%s not found", key);
    }
}
