#ifndef _PRISM_UI_CRYPTOCURRENCY_H
#define _PRISM_UI_CRYPTOCURRENCY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

LV_IMG_DECLARE( ui_coins_btc );
LV_IMG_DECLARE( ui_coins_doge );
LV_IMG_DECLARE( ui_coins_eth );
LV_IMG_DECLARE( ui_coins_xrp );

extern lv_obj_t *screen_cryptocurrency;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif