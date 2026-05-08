#pragma once
#include <lvgl.h>

// static lv_theme_t default_purple;
extern lv_style_t style_home;
extern lv_style_t style_clockContainers;
extern lv_style_t style_clocks;
extern lv_style_t style_regionIcons;


LV_IMG_DECLARE(poland);
LV_IMG_DECLARE(unitedStates);

void setStyles();
