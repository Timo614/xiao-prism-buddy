#include "prism_model.h"
#include "prism_sensor.h"
#include "prism_time.h"
#include "prism_city.h"

int prism_model_init(void)
{
    //sensor_init();
    time_init();
    city_init();
    return 0;
}