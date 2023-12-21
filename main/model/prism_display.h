#ifndef PRISM_DISPLAY_H
#define PRISM_DISPLAY_H

#include "config.h"
#include "view_data.h"

#ifdef __cplusplus
extern "C" {
#endif


int prism_display_init(void);

bool prism_display_st_get(void);

void prism_display_on(void);

void prism_display_off(void);

int prism_display_sleep_restart(void);

#ifdef __cplusplus
}
#endif

#endif