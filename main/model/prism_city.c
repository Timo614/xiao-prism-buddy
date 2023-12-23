#include "prism_city.h"
#include "freertos/semphr.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "cJSON.h"

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

static const char *TAG = "city";
static struct view_data_city __g_city_model;
static SemaphoreHandle_t   __g_city_http_com_sem;
static bool net_flag = false;
static char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

static int __city_data_parse(const char *p_str)
{
    int ret = 0;

    cJSON *root = NULL;
    cJSON* cjson_item = NULL;

    root = cJSON_Parse(p_str);
    if( root == NULL ) {
        return -1;
    }
   
    cjson_item = cJSON_GetObjectItem(root, "status");
    if( cjson_item != NULL  && cjson_item->valuestring != NULL) {
        if( strcmp(cjson_item->valuestring, "success") != 0 ) {
            ret = -2;
            goto parse_end;
        }
    }
    cjson_item = cJSON_GetObjectItem(root, "city");
    if( cjson_item != NULL  && cjson_item->valuestring != NULL) {
        strncpy(__g_city_model.city, cjson_item->valuestring, sizeof(__g_city_model.city)-1);
    }

    cjson_item = cJSON_GetObjectItem(root, "lon");
    if (cjson_item != NULL) {
        __g_city_model.location.longitude = cjson_item->valuedouble;
    }

    cjson_item = cJSON_GetObjectItem(root, "lat");
    if (cjson_item != NULL) {
        __g_city_model.location.latitude = cjson_item->valuedouble;
    }

    cjson_item = cJSON_GetObjectItem(root, "query");
    if( cjson_item != NULL  && cjson_item->valuestring != NULL) {
        strncpy(__g_city_model.ip, cjson_item->valuestring, sizeof(__g_city_model.ip)-1);
    }

    cjson_item = cJSON_GetObjectItem(root, "timezone");
    if( cjson_item != NULL  && cjson_item->valuestring != NULL) {
        strncpy(__g_city_model.timezone,  cjson_item->valuestring, sizeof(__g_city_model.timezone)-1);
    }
parse_end:

    cJSON_Delete(root);
    
    return ret;
}

#define WEB_SERVER "ip-api.com"
#define WEB_PORT "80"
#define WEB_PATH "/json"

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
    "Host: "WEB_SERVER":"WEB_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

static int __city_get(void)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;
    int retry=0;
    while(net_flag) {
        if( retry > 5) {
            return -1;
        }
        retry++;
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        /* Read HTTP response */
        int recv_len = 0;
        bzero(local_response_buffer, sizeof(local_response_buffer));

        do {
            r = read(s, local_response_buffer + recv_len, sizeof(local_response_buffer)-1-recv_len);
            recv_len += r;
            for(int i = 0; i < r; i++) {
                putchar(local_response_buffer[i+recv_len]);
            }
        } while(r > 0);

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", r, errno);
        close(s);

        if( recv_len > 0) {
            char *p_json = strstr(local_response_buffer, "\r\n\r\n");
            if( p_json ) {
                p_json =  p_json + 4;
                return __city_data_parse(p_json);
            } else {
                return -1;
            }
        }

    }
    return 0;
}

/* ---------------------------------------------------------- */
//  time zone 
/* ---------------------------------------------------------- */

static int __time_zone_data_parse(const char *p_str)
{
    //parse
    int ret = 0;

    cJSON *root = NULL;
    cJSON* cjson_item = NULL;
    cJSON* cjson_item_child = NULL;

    root = cJSON_Parse(p_str);
    if( root == NULL ) {
        ESP_LOGI(TAG, "cJSON_Parse err");
        return -1;
    }
   
    cjson_item = cJSON_GetObjectItem(root, "currentUtcOffset");
    if( cjson_item != NULL ) {
        cjson_item_child = cJSON_GetObjectItem(cjson_item, "seconds");
        if( cjson_item_child != NULL) {
            __g_city_model.local_utc_offset = cjson_item_child->valueint;
            ESP_LOGI(TAG, "local_utc_offset:%ds", cjson_item_child->valueint );
        }
    }

    cJSON_Delete(root);
    
    return ret;
}

extern const char timeapi_root_cert_pem_start[] asm("_binary_timeapi_cert_pem_start");
extern const char timeapi_root_cert_pem_end[]   asm("_binary_timeapi_cert_pem_end");

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
        
        break; //todo Let's say I can receive it all at once
    } while (1);

    if( recv_len > 0 ) {
        printf( "%s", local_response_buffer);
    }
    
cleanup:
    esp_tls_conn_destroy(tls);
exit:
    return recv_len;
}

static int __ip_get(char *ip, int buf_len)
{
    esp_tls_cfg_t cfg = {
        .cacert_buf = (const unsigned char *) timeapi_root_cert_pem_start,
        .cacert_bytes = timeapi_root_cert_pem_end - timeapi_root_cert_pem_start,
    };
    char ip_url[128] = {0};
    char ip_request[200] = {0};
    int len;
    snprintf(ip_url, sizeof(ip_url),"https://api.ipify.org");
    snprintf(ip_request, sizeof(ip_request),"GET %s HTTP/1.1\r\nHost: api.ipify.org\r\nUser-Agent: prism\r\n\r\n", ip_url);

    len = https_get_request(cfg, ip_url, ip_request);
    if(len > 0) {
        char *p_ip = strstr(local_response_buffer, "\r\n\r\n");
        if( p_ip ) {
            strncpy(ip, p_ip+4, buf_len);
            return 0;
        } else {
            return -1;
        }
    }
    return -1;
}

static int __time_zone_get(char *ip)
{
    esp_tls_cfg_t cfg = {
        .cacert_buf = (const unsigned char *) timeapi_root_cert_pem_start,
        .cacert_bytes = timeapi_root_cert_pem_end - timeapi_root_cert_pem_start,
    };

    char time_zone_url[128] = {0};
    char time_zone_request[200] = {0};
    int len  = 0;
    snprintf(time_zone_url, sizeof(time_zone_url),"https://www.timeapi.io/api/TimeZone/ip?ipAddress=%s",ip);
    snprintf(time_zone_request, sizeof(time_zone_request),"GET %s HTTP/1.1\r\nHost: www.timeapi.io\r\nUser-Agent: prism\r\n\r\n", time_zone_url);

    len = https_get_request(cfg, time_zone_url, time_zone_request);
    if( len > 0) {
        // TIME ZONE RESPONSE: HTTP/1.1 200 OK
        // Server: nginx/1.18.0 (Ubuntu)
        // Date: Thu, 02 Feb 2023 09:40:26 GMT
        // Content-Type: application/json; charset=utf-8
        // Transfer-Encoding: chunked
        // Connection: keep-alive

        // 128
        // {"timeZone":"UTC","currentLocalTime":"2023-02-02T09:40:26.1233729","currentUtcOffset":{"seconds":0,"milliseconds":0,"ticks":0,"nanoseconds":0},"standardUtcOffset":{"seconds":0,"milliseconds":0,"ticks":0,"nanoseconds":0},"hasDayLightSaving":false,"isDayLightSavingActive":false,"dstInterval":null}
        // 0
        char *p_json = strstr(local_response_buffer, "\r\n\r\n");
        if( p_json ) {
            p_json =  p_json + 4 + 3; //todo
            return __time_zone_data_parse(p_json);
        } else {
            return -1;
        }
       
    }
    return -1;
}

static void __prism_city_http_task(void *p_arg)
{
    int err = -1;

    bool city_flag = false;
    bool ip_flag = false;
    bool time_zone_flag = false;

    xSemaphoreTake(__g_city_http_com_sem, portMAX_DELAY);

    ESP_LOGI(TAG, "start Get city and time zone");

    while(1) {

        if( net_flag  && !city_flag) {
            
            err = __city_get();
            if( err == 0) {
                ESP_LOGI(TAG, "Get succesfully");
                ESP_LOGI(TAG, "ip        : %s", __g_city_model.ip);
                ESP_LOGI(TAG, "longitude : %f", __g_city_model.location.longitude);
                ESP_LOGI(TAG, "latitude  : %f", __g_city_model.location.latitude);
                ESP_LOGI(TAG, "city      : %s", __g_city_model.city);
                ESP_LOGI(TAG, "timezone  : %s", __g_city_model.timezone);
                city_flag = true;
                ip_flag= true;
                
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_CITY, &__g_city_model.city, sizeof(__g_city_model.city), portMAX_DELAY);
                esp_event_post_to(view_event_handle, VIEW_EVENT_BASE, VIEW_EVENT_LOCATION, &__g_city_model.location, sizeof(struct view_data_location), portMAX_DELAY);
            }
        }

        if( net_flag && !ip_flag ) {
            ESP_LOGI(TAG, "Get ip...");
            err = __ip_get(__g_city_model.ip, sizeof(__g_city_model.ip));
            if( err ==0 ) {
                ESP_LOGI(TAG, "ip: %s", __g_city_model.ip);
                ip_flag= true;
            }
        }
        if(  net_flag && ip_flag && !time_zone_flag) {
            ESP_LOGI(TAG, "Get time zone...");
            err =  __time_zone_get(__g_city_model.ip); 
            if( err == 0) {
                char zone_str[32];
                double zone = __g_city_model.local_utc_offset / 3600.0;

                if( zone >= 0) {
                    snprintf(zone_str, sizeof(zone_str) - 1, "UTC-%.1f", zone);
                } else {
                    snprintf(zone_str, sizeof(zone_str) - 1, "UTC+%.1f", 0 - zone);
                }
                time_net_zone_set( zone_str );

                time_zone_flag = true;
            }
        }

        if( city_flag  && time_zone_flag) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        
    }
    vTaskDelete(NULL);
}

static void __view_event_handler(void* handler_args, esp_event_base_t base, int32_t id, void* event_data)
{
    switch (id)
    {
        case VIEW_EVENT_WIFI_ST: {
            ESP_LOGI(TAG, "event: VIEW_EVENT_WIFI_ST");
            struct view_data_wifi_st *p_st = ( struct view_data_wifi_st *)event_data;
            if( p_st->is_connected) {
                net_flag = true;
                xSemaphoreGive(__g_city_http_com_sem); //right away  get city and time zone
            } else {
                net_flag = false;
            }
            break;
        }
        default:
            break;
    }
}

int prism_city_init(void)
{
    __g_city_http_com_sem = xSemaphoreCreateBinary();
    
    xTaskCreate(&__prism_city_http_task, "__prism_city_http_task", 1024 * 5, NULL, 10, NULL);

    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(view_event_handle, 
                                                        VIEW_EVENT_BASE, VIEW_EVENT_WIFI_ST, 
                                                        __view_event_handler, NULL, NULL));                                                        
    return 0;
}

