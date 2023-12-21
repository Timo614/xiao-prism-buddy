#ifndef VIEW_H
#define VIEW_H

#include "lvgl.h"
#include "lv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *screen_last;

void prism_view_init(void);

#ifdef __cplusplus
}
#endif

#endif