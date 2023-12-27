#include "cryptocurrency.h"
#include "ui_helpers.h"
#include <stdio.h>
#include "config.h"

lv_obj_t *screen_cryptocurrency;
// lv_obj_t *screen_cryptocurrency_wifi;
lv_obj_t *screen_cryptocurrency_image;
lv_obj_t *screen_cryptocurrency_value;
lv_obj_t *screen_cryptocurrency_change_percent;
lv_obj_t *screen_cryptocurrency_text;
lv_obj_t *screen_cryptocurrency_page;

void screen_cryptocurrency_init(void) {
    screen_cryptocurrency = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_cryptocurrency, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    // screen_cryptocurrency_wifi = lv_img_create(screen_cryptocurrency);
    // lv_img_set_src(screen_cryptocurrency_wifi, &ui_img_wifi_disconnect_png);
    // lv_obj_set_width( screen_cryptocurrency_wifi, LV_SIZE_CONTENT);  /// 1
    // lv_obj_set_height( screen_cryptocurrency_wifi, LV_SIZE_CONTENT);   /// 1
    // lv_obj_set_x( screen_cryptocurrency_wifi, -20 );
    // lv_obj_set_y( screen_cryptocurrency_wifi, 20 );
    // lv_obj_set_align( screen_cryptocurrency_wifi, LV_ALIGN_TOP_RIGHT );
    
    screen_cryptocurrency_text = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_text, LV_ALIGN_BOTTOM_MID );
    lv_label_set_text(screen_cryptocurrency_text,"Bitcoin");
    lv_obj_set_y( screen_cryptocurrency_text, -45 );
    lv_obj_set_style_text_font(screen_cryptocurrency_text, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_cryptocurrency_value = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_value, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_value, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_value, LV_ALIGN_CENTER );    
    lv_obj_set_y( screen_cryptocurrency_value, 40 );    
    lv_obj_set_x( screen_cryptocurrency_value, -40 );   
    char cryptocurrency_value[20];
    sprintf(cryptocurrency_value, "%s0.0", CURRENCY_SYMBOL);
    lv_label_set_text(screen_cryptocurrency_value, cryptocurrency_value);

    screen_cryptocurrency_change_percent = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_change_percent, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_change_percent, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_change_percent, LV_ALIGN_CENTER );      
    lv_obj_set_x( screen_cryptocurrency_change_percent, 40 );   
    lv_obj_set_y( screen_cryptocurrency_change_percent, 40 );
    char cryptocurrency_change_value[20];
    sprintf(cryptocurrency_change_value, "%0.2f%%", 0.0);
    lv_label_set_text(screen_cryptocurrency_change_percent, cryptocurrency_change_value);

    screen_cryptocurrency_page = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_page, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_page, LV_SIZE_CONTENT);   /// 1
    lv_label_set_text(screen_cryptocurrency_page,"1 / 4");
    lv_obj_set_y( screen_cryptocurrency_page, -20 );
    lv_obj_set_align( screen_cryptocurrency_page, LV_ALIGN_BOTTOM_MID );
    lv_obj_set_style_text_font(screen_cryptocurrency_page, &lv_font_montserrat_18, LV_PART_MAIN| LV_STATE_DEFAULT);
}