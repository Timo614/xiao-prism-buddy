/// Cryptocurrency data Powered by CoinGecko https://www.coingecko.com

#include "prism_cryptocurrency.h"
#include "freertos/semphr.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "prism_time.h"
#include "json_helper.h"
#include "view_data.h"
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

#define MAX_HTTP_OUTPUT_BUFFER 4096

static const char *TAG = "cryptocurrency";
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

static SemaphoreHandle_t   __g_cryptocurrency_http_com_sem;
static bool net_enabled = false;
static char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
static SemaphoreHandle_t       __g_data_mutex;

static int __cryptocurrency_data_parse(const char *p_str)
{
    cJSON *root = cJSON_Parse(p_str);
    if (root == NULL) {
        ESP_LOGI(TAG, "cJSON_Parse err");
        return -1;
    }

    cJSON *cjson_item; 

    cjson_item = cJSON_GetObjectItem(root, "bitcoin");
    if (cjson_item != NULL) {
        parseJsonDouble(TAG, cjson_item, "usd", &__g_cryptocurrency_data.bitcoin.value);
        parseJsonDouble(TAG, cjson_item, "usd_24h_change", &__g_cryptocurrency_data.bitcoin.value_24);
    }

    cjson_item = cJSON_GetObjectItem(root, "dogecoin");
    if (cjson_item != NULL) {
        parseJsonDouble(TAG, cjson_item, "usd", &__g_cryptocurrency_data.dogecoin.value);
        parseJsonDouble(TAG, cjson_item, "usd_24h_change", &__g_cryptocurrency_data.dogecoin.value_24);
    }

    cjson_item = cJSON_GetObjectItem(root, "ripple");
    if (cjson_item != NULL) {
        parseJsonDouble(TAG, cjson_item, "usd", &__g_cryptocurrency_data.ripple.value);
        parseJsonDouble(TAG, cjson_item, "usd_24h_change", &__g_cryptocurrency_data.ripple.value_24);
    }

    cjson_item = cJSON_GetObjectItem(root, "ethereum");
    if (cjson_item != NULL) {
        parseJsonDouble(TAG, cjson_item, "usd", &__g_cryptocurrency_data.ethereum.value);
        parseJsonDouble(TAG, cjson_item, "usd_24h_change", &__g_cryptocurrency_data.ethereum.value_24);
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
        return -1;
    }

    // Establishing TLS connection
    if (esp_tls_conn_http_new_sync(WEB_SERVER_URL, &cfg, tls) != 1) {
        ESP_LOGE(TAG, "Connection failed...");
        esp_tls_conn_destroy(tls);
        return -1;
    }
    ESP_LOGI(TAG, "Connection established...");

    // Sending the request
    size_t written_bytes = 0;
    size_t request_len = strlen(REQUEST);
    do {
        ret = esp_tls_conn_write(tls, REQUEST + written_bytes, request_len - written_bytes);
        if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_write returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            esp_tls_conn_destroy(tls);
            return -1;
        }
        written_bytes += ret;
    } while (written_bytes < request_len);

    ESP_LOGI(TAG, "Request sent, reading response...");

    // Reading the response
    memset(local_response_buffer, 0x00, sizeof(local_response_buffer));
    recv_len = 0;
    while (1) {
        len = sizeof(local_response_buffer) - recv_len - 1;
        ret = esp_tls_conn_read(tls, (char *)local_response_buffer + recv_len, len);
        if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            esp_tls_conn_destroy(tls);
            return -1;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "Connection closed by peer");
            break;
        }
        recv_len += ret;

        // Check if the end of the response is reached
        if (strstr((char *)local_response_buffer, "\r\n0\r\n")) {
            break;
        }
    }

    ESP_LOGI(TAG, "Response received");

    esp_tls_conn_destroy(tls);
    return recv_len;
}

static int __cryptocurrency_get(void)
{
    esp_tls_cfg_t cfg = {
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    char cryptocurrency_url[200] = {0};
    char cryptocurrency_request[300] = {0};
    int len  = 0;
    snprintf(cryptocurrency_url, sizeof(cryptocurrency_url),"https://api.coingecko.com/api/v3/simple/price?ids=bitcoin%%2Cdogecoin%%2Cethereum%%2Cripple&vs_currencies=%s&include_24hr_change=true", USER_CURRENCY);
    snprintf(cryptocurrency_request, sizeof(cryptocurrency_request),"GET %s HTTP/1.1\r\nHost: api.coingecko.com\r\nUser-Agent: prism\r\n\r\n", cryptocurrency_url);

    len = https_get_request(cfg, cryptocurrency_url, cryptocurrency_request);
    if( len > 0) {
        // {"bitcoin":{"usd":43560,"usd_24h_change":-1.1057568556937836},"dogecoin":{"usd":0.091702,"usd_24h_change":-0.9663856680500476},"ethereum":{"usd":2234.97,"usd_24h_change":-0.05076705649333615},"ripple":{"usd":0.616687,"usd_24h_change":-0.681641234851879}} 
        char *p_json = strstr(local_response_buffer, "\r\n\r\n");
        if( p_json ) {
            p_json =  p_json + 4 + 3; //todo
            return __cryptocurrency_data_parse(p_json);
        } else {
            return -1;
        }
       
    }
    return -1;
}

static void __prism_cryptocurrency_http_task(void *p_arg)
{
    int err = -1;

    xSemaphoreTake(__g_cryptocurrency_http_com_sem, portMAX_DELAY);

    ESP_LOGI(TAG, "start Get cryptocurrency and time zone");

    while(1) {

        if(  net_enabled ) {
            xSemaphoreTake(__g_data_mutex, portMAX_DELAY);
            int result = __cryptocurrency_get(); 
            xSemaphoreGive(__g_data_mutex);
            if( result > -1) {
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_CRYPTOCURRENCY, &__g_cryptocurrency_data, sizeof(__g_cryptocurrency_data), portMAX_DELAY);

                // After cryptocurrency has been set, delay calling API for 10 minutes to avoid rate limits
                vTaskDelay(pdMS_TO_TICKS(10 * 60 * 1000));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10 * 1000));
        
    }
    vTaskDelete(NULL);
}

static void __view_event_cryptocurrency_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    switch (id)
    {
        case VIEW_EVENT_WIFI_ST: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            net_enabled = true;
            break;
        }
        case VIEW_EVENT_WEATHER: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WEATHER");
            if(!net_enabled) {
                break;
            }
            xSemaphoreGive(__g_cryptocurrency_http_com_sem); 
            break;
        }
        default:
            break;
    }
}

int prism_cryptocurrency_init(void)
{
    __g_cryptocurrency_http_com_sem = xSemaphoreCreateBinary();
    __g_data_mutex  =  xSemaphoreCreateMutex();
    
    xTaskCreate(&__prism_cryptocurrency_http_task, "__prism_cryptocurrency_http_task", 1024 * 6, NULL, 10, NULL);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                        VIEW_EVENT_BASE, VIEW_EVENT_WIFI_ST, 
                                                        __view_event_cryptocurrency_handler, NULL, NULL));     
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                        VIEW_EVENT_BASE, VIEW_EVENT_WEATHER, 
                                                        __view_event_cryptocurrency_handler, NULL, NULL));                                                        
    return 0;
}

