#include "prism_weather.h"
#include "freertos/semphr.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "prism_time.h"
#include "json_helper.h"
#include "esp_crt_bundle.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/esp_debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "esp_crt_bundle.h"

#define MAX_HTTP_OUTPUT_BUFFER 1024*3

static const char *TAG = "weather";
static struct view_weather_data __g_weather_data = {
    .temperature = 0.0,
    .humidity = 0,
    .weather = CLEAR_DAY,
};
static SemaphoreHandle_t   __g_weather_http_com_sem;
static bool location_set = false;
static char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
static struct view_data_location __location_data;
static SemaphoreHandle_t       __g_data_mutex;

static int __weather_data_parse(const char *p_str)
{
    cJSON *root = cJSON_Parse(p_str);
    if( root == NULL ) {
        ESP_LOGI(TAG, "cJSON_Parse err");
        return -1;
    }

    int rain, snow, cloud_cover;
    bool is_day;
    cJSON *cjson_item = cJSON_GetObjectItem(root, "current");
    if (cjson_item != NULL) {
        parseJsonDouble(TAG, cjson_item, "temperature_2m", &__g_weather_data.temperature);
        parseJsonInt(TAG, cjson_item, "relative_humidity_2m", &__g_weather_data.humidity);
        parseJsonInt(TAG, cjson_item, "rain", &rain);
        parseJsonInt(TAG, cjson_item, "snowfall", &snow);
        parseJsonInt(TAG, cjson_item, "cloud_cover", &cloud_cover);
        parseJsonBool(TAG, cjson_item, "is_day", &is_day);
    }

    if (rain > 0 || snow > 0) {
        if (snow > 0 && rain >= 0.25 * (rain + snow)) {
            __g_weather_data.weather = SLEET;
        } else if (snow > 0) {
            __g_weather_data.weather = SNOWY;
        } else {
            __g_weather_data.weather = RAINY;
        }
    } else if (cloud_cover >= 37 && cloud_cover <= 62) {
        __g_weather_data.weather = is_day ? PARTLY_CLOUDY_DAY : PARTLY_CLOUDY_NIGHT;
    } else if (cloud_cover > 63) {
        __g_weather_data.weather = CLOUDY;
    } else {
        __g_weather_data.weather = is_day ? CLEAR_DAY : CLEAR_NIGHT;
    }

    cJSON_Delete(root);
    
    return 0;
}

static int https_get_request(esp_tls_cfg_t cfg, const char *WEB_SERVER_URL, const char *REQUEST)
{
    int ret, len;
    int recv_len = 0;
    esp_tls_t *tls = esp_tls_init();
    if (!tls) {
        ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
        goto exit;
    }

    if (esp_tls_conn_http_new_sync(WEB_SERVER_URL, &cfg, tls) == 1) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto cleanup;
    }

    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls,
                                 REQUEST + written_bytes,
                                 strlen(REQUEST) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            goto cleanup;
        }
    } while (written_bytes < strlen(REQUEST));

    ESP_LOGI(TAG, "Reading HTTP response...");
    memset(local_response_buffer, 0x00, sizeof(local_response_buffer));
    recv_len = 0;
    do {
        len = sizeof(local_response_buffer) - recv_len - 1;
       
        ret = esp_tls_conn_read(tls, (char *)local_response_buffer + recv_len, len);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            continue;
        } else if (ret < 0) {
            recv_len = 0;
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }
        recv_len += ret;
        
        break; 
    } while (1);

    if( recv_len > 0 ) {
        printf( "%s", local_response_buffer);
    }
    
cleanup:
    esp_tls_conn_destroy(tls);
exit:
    return recv_len;
}

static int __weather_get(double latitude, double longitude)
{
    esp_tls_cfg_t cfg = {
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    char weather_url[200] = {0};
    char weather_request[300] = {0};
    int len  = 0;
    snprintf(weather_url, sizeof(weather_url),"https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,is_day,precipitation,rain,snowfall,cloud_cover,wind_speed_10m", latitude, longitude);
    snprintf(weather_request, sizeof(weather_request),"GET %s HTTP/1.1\r\nHost: api.open-meteo.com\r\nUser-Agent: prism\r\n\r\n", weather_url);

    len = https_get_request(cfg, weather_url, weather_request);
    if( len > 0) {
        // {"latitude":26.007067,"longitude":-79.99933,"generationtime_ms":0.049948692321777344,"utc_offset_seconds":0,"timezone":"GMT","timezone_abbreviation":"GMT","elevation":0.0,"current_units":{"time":"iso8601","interval":"seconds","temperature_2m":"°C","relative_humidity_2m":"%","is_day":"","precipitation":"mm","rain":"mm","snowfall":"cm","cloud_cover":"%","wind_speed_10m":"km/h"},"current":{"time":"2023-12-21T04:30","interval":900,"temperature_2m":21.9,"relative_humidity_2m":58,"is_day":0,"precipitation":0.00,"rain":0.00,"snowfall":0.00,"cloud_cover":100,"wind_speed_10m":41.9}}
        char *p_json = strstr(local_response_buffer, "\r\n\r\n");
        if( p_json ) {
            p_json =  p_json + 4 + 3; //todo
            return __weather_data_parse(p_json);
        } else {
            return -1;
        }
       
    }
    return -1;
}

static void __prism_weather_http_task(void *p_arg)
{
    int err = -1;

    xSemaphoreTake(__g_weather_http_com_sem, portMAX_DELAY);

    ESP_LOGI(TAG, "start Get weather and time zone");

    while(1) {

        if(  location_set ) {
            xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
            int result = __weather_get(__location_data.latitude, __location_data.longitude); 
            xSemaphoreGive(__g_data_mutex);
            if( result >= 0) {
                
                ESP_LOGI(TAG, "event: temperature %2.2f", __g_weather_data.temperature);
                ESP_LOGI(TAG, "event: humidity %d", __g_weather_data.humidity);
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_WEATHER, &__g_weather_data, sizeof(__g_weather_data), portMAX_DELAY);

                // After weather has been set, delay calling API for 10 minutes to avoid rate limitsVIEW_EVENT_WEATHER
                vTaskDelay(pdMS_TO_TICKS(10 * 60 * 1000));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10 * 1000));
        
    }
    vTaskDelete(NULL);
}

static void __view_event_location_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    if (location_set) {
        return;
    }
    struct view_data_location *location_data = (struct view_data_location *)event_data;
    
    xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
    memcpy(&__location_data, location_data, sizeof(struct view_data_location));
    location_set = true;
    xSemaphoreGive(__g_data_mutex);

    xSemaphoreGive(__g_weather_http_com_sem);
}

int prism_weather_init(void)
{
    __g_weather_http_com_sem = xSemaphoreCreateBinary();
    __g_data_mutex  =  xSemaphoreCreateMutex();
    
    xTaskCreate(&__prism_weather_http_task, "__prism_weather_http_task", 1024 * 4, NULL, 10, NULL);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                        VIEW_EVENT_BASE, VIEW_EVENT_LOCATION, 
                                                        __view_event_location_handler, NULL, NULL));                                                        
    return 0;
}

