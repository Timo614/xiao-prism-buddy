#include "prism_model.h"
#include "prism_sensor.h"
#include "prism_display.h"
#include "prism_time.h"
#include "prism_city.h"
#include "prism_cryptocurrency.h"
#include "prism_weather.h"

int prism_model_init(void)
{
    prism_sensor_init();
    prism_time_init();
    prism_city_init();
    prism_cryptocurrency_init();
    prism_weather_init();
    prism_display_init(); 
    return 0;
}