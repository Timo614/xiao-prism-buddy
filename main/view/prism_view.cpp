#include "prism_view.h"

#include "browser.h"
#include "cryptocurrency.h"
#include "main.h"
#include "setting_24_hour_clock.h"
#include "setting_brightness.h"
#include "setting.h"

#include "config.h"
#include "ui_helpers.h"
#include "esp_wifi.h"
#include <time.h>

static const char *TAG = "view";

void prism_view_init(void)
{
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_color_hex(0x529d53), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    screen_main_init();
    screen_setting_24_hour_clock_init();
    screen_setting_brightness_init();
    screen_setting_init();
}