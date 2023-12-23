#ifndef VIEW_DATA_H
#define VIEW_DATA_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

struct view_data_wifi_st
{
    bool   is_connected;
    bool   is_connecting;
    bool   is_network;  //is connect network
    char   ssid[32];
    int8_t rssi;
};

struct view_data_wifi_connet_ret_msg 
{
    uint8_t ret; //0:successful , 1: failure
    char    msg[64];
};

struct view_data_display
{
    int   brightness; //0~100
    bool  fade_text_enabled;  
};

struct view_data_time_cfg
{
    bool    time_format_24;   
}__attribute__((packed));

struct view_data_location
{
    double   longitude;
    double   latitude;
};

struct view_data_city
{
    char  ip[32];
    char  city[32];
    char  timezone[64];
    int   local_utc_offset;
    struct view_data_location location;
};

struct view_cryptocurrency_data_item {
    double value;
    double value_24;
};

struct view_cryptocurrency_data {
   struct view_cryptocurrency_data_item bitcoin;
   struct view_cryptocurrency_data_item ethereum;
   struct view_cryptocurrency_data_item ripple;
   struct view_cryptocurrency_data_item dogecoin;
};

typedef enum {
    CLEAR_DAY = 0,
    CLEAR_NIGHT,
    CLOUDY,
    PARTLY_CLOUDY_DAY,
    PARTLY_CLOUDY_NIGHT,
    RAINY,
    SLEET,
    SNOWY
} weather_type_t;

typedef enum {
    GESTURE_NONE = 0,
    GESTURE_GRAB,
    GESTURE_ROTATE_RIGHT,
    GESTURE_ROTATE_LEFT,
    GESTURE_SWIPE_LEFT,
    GESTURE_SWIPE_RIGHT
} gesture_type_t;

struct gesture_event_t {
    gesture_type_t type;
    int data;
};

enum {
    VIEW_EVENT_SCREEN_START = 0,  // uint8_t, enum start_screen, which screen when start
    VIEW_EVENT_TIME,  //  bool time_format_24    
    VIEW_EVENT_WIFI_ST,   //view_data_wifi_st_t
    VIEW_EVENT_CITY,      // char city[32], max display 24 char
    VIEW_EVENT_LOCATION, // struct view_data_location
    VIEW_EVENT_CRYPTOCURRENCY, // struct view_cryptocurrency_data
    VIEW_EVENT_WEATHER, // struct weather_type_t
    VIEW_EVENT_SENSOR_DATA, // struct view_data_sensor_data
    VIEW_EVENT_SENSOR_GESTURE, 
    VIEW_EVENT_WIFI_CONNECT,    // struct view_data_wifi_config
    VIEW_EVENT_WIFI_CONNECT_RET,   // struct view_data_wifi_connet_ret_msg
    VIEW_EVENT_TIME_CFG_UPDATE,  //  struct view_data_time_cfg
    VIEW_EVENT_TIME_CFG_APPLY,   //  struct view_data_time_cfg
    VIEW_EVENT_DISPLAY_CFG,         // struct view_data_display
    VIEW_EVENT_BRIGHTNESS_UPDATE,   // uint8_t brightness
    VIEW_EVENT_DISPLAY_CFG_APPLY,   // struct view_data_display. will save

    VIEW_EVENT_SHUTDOWN,      //NULL

    VIEW_EVENT_ALL,
};

#ifdef __cplusplus
}
#endif

#endif