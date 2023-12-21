#include "browser.h"
#include "ui_helpers.h"

lv_obj_t *screen_browser;
lv_obj_t *screen_browser_wifi;
lv_obj_t *screen_browser_image;
lv_obj_t *screen_browser_text;
lv_obj_t *screen_browser_page;

void screen_browser_init(void)
{
    screen_browser = lv_obj_create(NULL);
    lv_obj_clear_flag( screen_browser, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

    screen_browser_wifi = lv_img_create(screen_browser);
    lv_img_set_src(screen_browser_wifi, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_browser_wifi, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_browser_wifi, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x( screen_browser_wifi, -20 );
    lv_obj_set_y( screen_browser_wifi, 20 );
    lv_obj_set_align( screen_browser_wifi, LV_ALIGN_TOP_RIGHT );
    
    screen_browser_image = lv_img_create(screen_browser);
    lv_img_set_src(screen_browser_image, &ui_img_wifi_disconnect_png);
    lv_obj_set_width( screen_browser_image, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_browser_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_browser_image, LV_ALIGN_CENTER );

    screen_browser_text = lv_label_create(screen_browser);
    lv_obj_set_width( screen_browser_text, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_browser_text, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_browser_text, LV_ALIGN_CENTER );
    lv_label_set_text(screen_browser_text,"Item");
    lv_obj_set_style_text_font(screen_browser_text, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);

    screen_browser_page = lv_label_create(screen_browser);
    lv_obj_set_width( screen_browser_page, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height( screen_browser_page, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_align( screen_browser_page, LV_ALIGN_CENTER );
    lv_label_set_text(screen_browser_page,"0/0");
    lv_obj_set_style_text_font(screen_browser_page, &ui_font_font4, LV_PART_MAIN| LV_STATE_DEFAULT);
}