#ifndef TIME_H
#define TIME_H

#include "config.h"
#include "view_data.h"

#ifdef __cplusplus
extern "C" {
#endif


//ntp sync
int time_init(void);

// set TZ
int time_net_zone_set( char *p);

#ifdef __cplusplus
}
#endif

#endif