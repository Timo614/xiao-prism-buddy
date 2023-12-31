#include "ui_helpers.h"

lv_obj_t *current_screen;

///////////////////// ANIMATIONS ////////////////////
void up_Animation( lv_obj_t *TargetObject, int delay)
{
lv_anim_t PropertyAnimation_0;
lv_anim_init(&PropertyAnimation_0);
lv_anim_set_time(&PropertyAnimation_0, 200);
lv_anim_set_user_data(&PropertyAnimation_0,TargetObject);
lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y );
lv_anim_set_values(&PropertyAnimation_0, -50, 0 );
lv_anim_set_path_cb( &PropertyAnimation_0, lv_anim_path_ease_out);
lv_anim_set_delay( &PropertyAnimation_0, delay + 0 );
lv_anim_set_playback_time(&PropertyAnimation_0, 0);
lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
lv_anim_set_early_apply( &PropertyAnimation_0, false );
lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y );
lv_anim_start(&PropertyAnimation_0);
lv_anim_t PropertyAnimation_1;
lv_anim_init(&PropertyAnimation_1);
lv_anim_set_time(&PropertyAnimation_1, 100);
lv_anim_set_user_data(&PropertyAnimation_1,TargetObject);
lv_anim_set_custom_exec_cb(&PropertyAnimation_1, _ui_anim_callback_set_opacity );
lv_anim_set_values(&PropertyAnimation_1, 0, 255 );
lv_anim_set_path_cb( &PropertyAnimation_1, lv_anim_path_linear);
lv_anim_set_delay( &PropertyAnimation_1, delay + 0 );
lv_anim_set_playback_time(&PropertyAnimation_1, 0);
lv_anim_set_playback_delay(&PropertyAnimation_1, 0);
lv_anim_set_repeat_count(&PropertyAnimation_1, 0);
lv_anim_set_repeat_delay(&PropertyAnimation_1, 0);
lv_anim_set_early_apply( &PropertyAnimation_1, true );
lv_anim_set_get_value_cb(&PropertyAnimation_1, &_ui_anim_callback_get_opacity );
lv_anim_start(&PropertyAnimation_1);
}

void _ui_basic_set_property( lv_obj_t *target, int id, int val) 
{
   if (id == _UI_BASIC_PROPERTY_POSITION_X) lv_obj_set_x(target, val);
   if (id == _UI_BASIC_PROPERTY_POSITION_Y) lv_obj_set_y(target, val);
   if (id == _UI_BASIC_PROPERTY_WIDTH) lv_obj_set_width(target, val);
   if (id == _UI_BASIC_PROPERTY_HEIGHT) lv_obj_set_height(target, val);
}

void _ui_image_set_property( lv_obj_t *target, int id, uint8_t *val) 
{
   if (id == _UI_IMAGE_PROPERTY_IMAGE) lv_img_set_src(target, val);
}

void _ui_label_set_property( lv_obj_t *target, int id, char *val) 
{
   if (id == _UI_LABEL_PROPERTY_TEXT) lv_label_set_text(target, val);
}

void _ui_slider_set_property( lv_obj_t *target, int id, int val) 
{
   if (id == _UI_SLIDER_PROPERTY_VALUE_WITH_ANIM) lv_slider_set_value(target, val, LV_ANIM_ON);
   if (id == _UI_SLIDER_PROPERTY_VALUE) lv_slider_set_value(target, val, LV_ANIM_OFF);
}

void scr_unloaded_delete_cb(lv_event_t * e)
{
   lv_obj_t ** var = lv_event_get_user_data(e);
   lv_obj_del(*var);
   (*var) = NULL;
}

void _ui_anim_callback_set_image_frame(lv_anim_t* a, int32_t v)
{
    ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
    usr->val = v;
    if ( v<0 ) v=0;
    if ( v>=usr->imgset_size ) v=usr->imgset_size-1;
    lv_img_set_src(usr->target, usr->imgset[v]);
}

void _ui_spinbox_step( lv_obj_t *target, int val, int anm) 
{
   int old = lv_slider_get_value(target);
   lv_slider_set_value(target, old+val, anm);
   lv_event_send(target,LV_EVENT_VALUE_CHANGED, 0);
}


int32_t _ui_anim_callback_get_image_frame(lv_anim_t* a)
{
   ui_anim_user_data_t *usr = (ui_anim_user_data_t *)a->user_data;
   return usr->val;
}

void _ui_anim_callback_free_user_data(lv_anim_t *a)
{
	lv_mem_free(a->user_data);
	a->user_data=NULL;
}

void _ui_screen_delete( lv_obj_t ** target ) 
{
   if(*target == NULL)
   {
      lv_obj_del(*target);
      target = NULL;
   }
}

void _ui_screen_change( lv_obj_t *target) 
{
   current_screen = target;
   lv_scr_load(target);
}

void _ui_slider_increment( lv_obj_t *target, int val, int anm) 
{
   int old = lv_slider_get_value(target);
   lv_slider_set_value(target, old+val, anm);   
}

void _ui_flag_modify( lv_obj_t *target, int32_t flag, int value)
{
    if (value==_UI_MODIFY_FLAG_TOGGLE)
    {
        if ( lv_obj_has_flag(target,flag) ) lv_obj_clear_flag(target,flag);
        else lv_obj_add_flag(target,flag);
    }
    else if (value==_UI_MODIFY_FLAG_ADD) lv_obj_add_flag(target,flag);
    else lv_obj_clear_flag(target,flag);
}
void _ui_state_modify( lv_obj_t *target, int32_t state, int value)
{
    if (value==_UI_MODIFY_STATE_TOGGLE)
    {
        if ( lv_obj_has_state(target,state) ) lv_obj_clear_state(target,state);
        else lv_obj_add_state(target,state);
    }
    else if (value==_UI_MODIFY_STATE_ADD) lv_obj_add_state(target,state);
    else lv_obj_clear_state(target,state);
}

void _ui_opacity_set( lv_obj_t *target, int val) 
{
   lv_obj_set_style_opa(target, val, 0);
}

void _ui_anim_callback_set_x(lv_anim_t* a, int32_t v)
{
   lv_obj_set_x((lv_obj_t *)a->user_data, v);
}

void _ui_anim_callback_set_y(lv_anim_t* a, int32_t v)
{
   lv_obj_set_y((lv_obj_t *)a->user_data, v);
}

void _ui_anim_callback_set_width(lv_anim_t* a, int32_t v)
{
   lv_obj_set_width((lv_obj_t *)a->user_data, v);
}

void _ui_anim_callback_set_height(lv_anim_t* a, int32_t v)
{
   lv_obj_set_height((lv_obj_t *)a->user_data, v);
}

void _ui_anim_callback_set_opacity(lv_anim_t* a, int32_t v)
{
   lv_obj_set_style_opa((lv_obj_t *)a->user_data, v, 0);
}

void _ui_anim_callback_set_image_zoom(lv_anim_t* a, int32_t v)
{
   lv_img_set_zoom((lv_obj_t *)a->user_data, v);
}

void _ui_anim_callback_set_image_angle(lv_anim_t* a, int32_t v)
{
   lv_img_set_angle((lv_obj_t *)a->user_data, v);
}


int32_t _ui_anim_callback_get_x(lv_anim_t* a)
{
   return lv_obj_get_x_aligned((lv_obj_t *)a->user_data);
}

int32_t _ui_anim_callback_get_y(lv_anim_t* a)
{
   return lv_obj_get_y_aligned((lv_obj_t *)a->user_data);
}

int32_t _ui_anim_callback_get_width(lv_anim_t* a)
{
   return lv_obj_get_width((lv_obj_t *)a->user_data);
}

int32_t _ui_anim_callback_get_height(lv_anim_t* a)
{
   return lv_obj_get_height((lv_obj_t *)a->user_data);
}

int32_t _ui_anim_callback_get_opacity(lv_anim_t* a)
{
   return lv_obj_get_style_opa((lv_obj_t *)a->user_data, 0);
}

int32_t _ui_anim_callback_get_image_zoom(lv_anim_t* a)
{
   return lv_img_get_zoom((lv_obj_t *)a->user_data);
}

int32_t _ui_anim_callback_get_image_angle(lv_anim_t* a)
{
   return lv_img_get_angle((lv_obj_t *)a->user_data);
}

void _ui_checked_set_text_value( lv_obj_t *trg, lv_obj_t *src, char *txt_on, char *txt_off) 
{
    if (lv_obj_has_state(src,LV_STATE_CHECKED)) lv_label_set_text(trg,txt_on);
    else lv_label_set_text(trg,txt_off);
}

