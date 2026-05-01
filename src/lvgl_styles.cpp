#include <lvgl_styles.h>

lv_style_t style_home;
lv_style_t style_clockContainers;
lv_style_t style_clocks;
lv_style_t style_clocks_fetching;
lv_style_t style_regionIcons;

LV_FONT_DECLARE(ndot57_42);
LV_FONT_DECLARE(ndot57_80);
LV_FONT_DECLARE(ndot57_100);
LV_FONT_DECLARE(ndot57_120);

void setStyles() {
    lv_style_init(&style_home);
    lv_style_set_bg_grad_dir(&style_home, LV_GRAD_DIR_VER);
    lv_style_set_bg_color(&style_home, lv_color_hex(0x0021f3));
    lv_style_set_bg_grad_stop(&style_home, 127);
    lv_style_set_bg_grad_color(&style_home, lv_color_hex(0x6c3baa));
    lv_style_set_text_color(&style_home, lv_color_hex(0xffffff));
    lv_style_set_border_width(&style_home, 0);

    lv_style_init(&style_clockContainers);
    lv_style_set_bg_color(&style_clockContainers, lv_color_hex(0x4d4d4d));
    lv_style_set_flex_grow(&style_clockContainers, 1);
    lv_style_set_pad_all(&style_clockContainers, 2);
    lv_style_set_width(&style_clockContainers, lv_pct(100));
    lv_style_set_layout(&style_clockContainers, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_clockContainers, LV_FLEX_FLOW_ROW);
    lv_style_set_border_width(&style_clockContainers, 2);
    lv_style_set_border_color(&style_clockContainers, lv_color_black());

    lv_style_init(&style_clocks);
    lv_style_set_pad_left(&style_clocks, 0);
    lv_style_set_pad_right(&style_clocks, 0);
    lv_style_set_pad_top(&style_clocks, 7);
    lv_style_set_text_font(&style_clocks, &ndot57_80);
    lv_style_set_text_color(&style_clocks, lv_color_hex(0x00f2de));
    lv_style_set_flex_grow(&style_clocks, 1);
    lv_style_set_align(&style_clocks, LV_ALIGN_CENTER);
    lv_style_set_max_height(&style_clocks, lv_pct(100));

    lv_style_init(&style_clocks_fetching);
    lv_style_set_text_font(&style_clocks_fetching, &ndot57_42);
    lv_style_set_text_color(&style_clocks_fetching, lv_color_hex(0x00f2de));

    lv_style_init(&style_regionIcons);
    lv_style_set_height(&style_regionIcons, lv_pct(100));
    lv_style_set_width(&style_regionIcons, lv_pct(20));
    lv_style_set_align(&style_regionIcons, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_left(&style_regionIcons, 2);
    lv_style_set_pad_right(&style_regionIcons, 0);
}
