#ifndef _PRISM_UI_HELPERS_H
#define _PRISM_UI_HELPERS_H

#include "lvgl.h"


#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *current_screen;

LV_IMG_DECLARE( ui_img_wifi_disconnect_png);   // assets/wifi_disconnect.png
LV_IMG_DECLARE( ui_img_high_light_png);   // assets/high_light.png
LV_IMG_DECLARE( ui_img_low_light_png);   // assets/low_light.png

LV_IMG_DECLARE( ui_img_wifi_1_png);
LV_IMG_DECLARE( ui_img_wifi_2_png);
LV_IMG_DECLARE( ui_img_wifi_3_png);
LV_IMG_DECLARE( ui_img_lock_png);

void up_Animation( lv_obj_t *TargetObject, int delay);

#define _UI_BASIC_PROPERTY_POSITION_X 0
#define _UI_BASIC_PROPERTY_POSITION_Y 1
#define _UI_BASIC_PROPERTY_WIDTH 2
#define _UI_BASIC_PROPERTY_HEIGHT 3
void _ui_basic_set_property( lv_obj_t *target, int id, int val);

#define _UI_IMAGE_PROPERTY_IMAGE 0
void _ui_image_set_property( lv_obj_t *target, int id, uint8_t *val);

#define _UI_LABEL_PROPERTY_TEXT 0
void _ui_label_set_property( lv_obj_t *target, int id, char *val);

#define _UI_SLIDER_PROPERTY_VALUE 0
#define _UI_SLIDER_PROPERTY_VALUE_WITH_ANIM 1
void _ui_slider_set_property( lv_obj_t *target, int id, int val);

void _ui_screen_change( lv_obj_t *target);

void _ui_slider_increment( lv_obj_t *target, int val, int anm);

#define _UI_MODIFY_FLAG_ADD 0
#define _UI_MODIFY_FLAG_REMOVE 1
#define _UI_MODIFY_FLAG_TOGGLE 2
void _ui_flag_modify( lv_obj_t *target, int32_t flag, int value);

#define _UI_MODIFY_STATE_ADD 0
#define _UI_MODIFY_STATE_REMOVE 1
#define _UI_MODIFY_STATE_TOGGLE 2
void _ui_state_modify( lv_obj_t *target, int32_t state, int value);

void _ui_opacity_set( lv_obj_t *target, int val);

void _ui_anim_callback_set_x(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_y(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_width(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_height(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_opacity(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_image_zoom(lv_anim_t* a, int32_t v);

void _ui_anim_callback_set_image_angle(lv_anim_t* a, int32_t v);

int32_t _ui_anim_callback_get_x(lv_anim_t* a);

int32_t _ui_anim_callback_get_y(lv_anim_t* a);

int32_t _ui_anim_callback_get_width(lv_anim_t* a);

int32_t _ui_anim_callback_get_height(lv_anim_t* a);

int32_t _ui_anim_callback_get_opacity(lv_anim_t* a);

int32_t _ui_anim_callback_get_image_zoom(lv_anim_t* a);

int32_t _ui_anim_callback_get_image_angle(lv_anim_t* a);

void _ui_checked_set_text_value( lv_obj_t *trg, lv_obj_t *src, char *txt_on, char *txt_off);

void _ui_screen_delete( lv_obj_t ** target );

void scr_unloaded_delete_cb(lv_event_t * e);
/** Describes an animation*/
typedef struct _ui_anim_user_data_t {
    lv_obj_t *target;
    lv_img_dsc_t **imgset;
    int32_t imgset_size;
    int32_t val;
} ui_anim_user_data_t;
void _ui_anim_callback_free_user_data(lv_anim_t *a);
void _ui_anim_callback_set_image_frame(lv_anim_t* a, int32_t v);
int32_t _ui_anim_callback_get_image_frame(lv_anim_t* a);
void _ui_spinbox_step( lv_obj_t *target, int val, int anm);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
