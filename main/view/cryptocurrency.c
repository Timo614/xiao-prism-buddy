#include "cryptocurrency.h"
#include "ui_helpers.h"

lv_obj_t *screen_cryptocurrency;
lv_obj_t *screen_cryptocurrency_wifi;
lv_obj_t *screen_cryptocurrency_image;
lv_obj_t *screen_cryptocurrency_value;
lv_obj_t *screen_cryptocurrency_text;
lv_obj_t *screen_cryptocurrency_change_image;
lv_obj_t *screen_cryptocurrency_page;

void screen_cryptocurrency_init(void) {
    screen_cryptocurrency = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_cryptocurrency, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_cryptocurrency_wifi = lv_img_create(screen_cryptocurrency);
    lv_img_set_src(screen_cryptocurrency_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_cryptocurrency_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_cryptocurrency_wifi, -20 );
    lv_obj_set_y( screen_cryptocurrency_wifi, 20 );
    lv_obj_set_align( screen_cryptocurrency_wifi, LV_ALIGN_TOP_RIGHT );
    
    screen_cryptocurrency_image = lv_img_create(screen_cryptocurrency);
    lv_img_set_src(screen_cryptocurrency_image, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_cryptocurrency_image, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_image, LV_ALIGN_CENTER );

    screen_cryptocurrency_text = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_text, LV_ALIGN_CENTER );
    lv_label_set_text(screen_cryptocurrency_text,"BTC");
    lv_obj_set_style_text_font(screen_cryptocurrency_text, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_cryptocurrency_value = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_value, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_value, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_value, LV_ALIGN_CENTER );
    lv_label_set_text(screen_cryptocurrency_value,"43,564.80 USD");
    lv_obj_set_style_text_font(screen_cryptocurrency_value, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_cryptocurrency_change_image = lv_img_create(screen_cryptocurrency);
    lv_img_set_src(screen_cryptocurrency_change_image, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_cryptocurrency_change_image, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_change_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_change_image, LV_ALIGN_CENTER );

    screen_cryptocurrency_page = lv_label_create(screen_cryptocurrency);
    lv_obj_set_width( screen_cryptocurrency_page, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_cryptocurrency_page, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_cryptocurrency_page, LV_ALIGN_CENTER );
    lv_label_set_text(screen_cryptocurrency_page,"0/0");
    lv_obj_set_style_text_font(screen_cryptocurrency_page, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);
}