#include <lvgl_styles.h>

lv_style_t style_home;
lv_style_t style_clockContainers;
lv_style_t style_clocks;

void setStyles() {
    lv_style_init(&style_home);
    lv_style_set_bg_color(&style_home, lv_color_hex(0x8F00FF));
    lv_style_set_text_color(&style_home, lv_color_hex(0xffffff));
    lv_style_set_pad_all(&style_home, 6);
    lv_style_set_border_width(&style_home, 2);
    lv_style_set_border_color(&style_home, lv_color_black());

    lv_style_init(&style_clockContainers);
    lv_style_set_flex_grow(&style_clockContainers, 1);
    lv_style_set_pad_all(&style_clockContainers, 6);
    lv_style_set_width(&style_clockContainers, lv_pct(100));
    lv_style_set_layout(&style_clockContainers, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_clockContainers, LV_FLEX_FLOW_ROW);

    lv_style_init(&style_clocks);
    lv_style_set_width(&style_clocks, lv_pct(35));
}
