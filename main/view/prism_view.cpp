#include "prism_view.hpp"

static const char *TAG = "view";

int prism_view_init(void)
{
    const char *test_string = "Test";
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, test_string);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    return 0;
}