#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <env.h>
#include <lvgl.h>
#include <lvgl_styles.h>
#include <TFT_eSPI.h>

//Display Defines
//GPIOs for SPI
// #define SPI_MOSI      23
// #define SPI_MISO      19
// #define SPI_SCK       18

// // GPIOs for TFT/TP
// #define TFT_CS        22
// #define TFT_DC        12
//#define TFT_RST       14
// #define TP_CS         21

#define TIME_STRING_SIZE 128
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565_SWAPPED))
#define DISPLAY_BUFFER_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * BYTES_PER_PIXEL)

uint8_t *buf;

// TFT tft;
TFT_eSPI tft = TFT_eSPI();

char *localTimeString;
char *worldTimeString;

lv_obj_t * localClockTime;
lv_obj_t * worldClockTime;

uint32_t get_millis();
void updateTime(char *localTimeString, char *worldTimeString);
void flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf);
void readInput(lv_indev_t * indev, lv_indev_data_t * data);
void clockRefresh(lv_timer_t * timer);

void setup() {
  Serial.begin(115200);
  
  localTimeString = (char *)malloc(TIME_STRING_SIZE);
  worldTimeString = (char *)malloc(TIME_STRING_SIZE);

  //LVGL Initialization
  lv_init();
  lv_tick_set_cb(get_millis);
  lv_display_t * display = lv_display_create(TFT_HEIGHT, TFT_WIDTH);
  lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565_SWAPPED);
  buf = (uint8_t *)calloc(DISPLAY_BUFFER_SIZE, 1);
  lv_display_set_buffers(display, buf, NULL, DISPLAY_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, flush);
  
  //LVGL Input Device Initialization
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, readInput);

  //Home Screen Setup
  setStyles();
  lv_obj_t * clocks = lv_obj_create(lv_screen_active());
  lv_obj_t * localClock = lv_obj_create(clocks);
  lv_obj_t * worldClock = lv_obj_create(clocks);
  lv_obj_t * localClockIcon = lv_image_create(localClock);
  lv_obj_t * worldClockIcon = lv_image_create(worldClock);
  localClockTime = lv_label_create(localClock);
  worldClockTime = lv_label_create(worldClock);
  lv_image_set_src(localClockIcon, &poland);
  lv_image_set_src(worldClockIcon, &unitedStates);
  lv_image_set_scale(localClockIcon, 192);
  lv_image_set_scale(worldClockIcon, 192);
  lv_obj_set_layout(lv_screen_active(), LV_LAYOUT_FLEX);
  lv_obj_set_width(clocks, lv_pct(100));
  lv_obj_set_height(clocks, lv_pct(100));
  lv_obj_set_layout(clocks, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(clocks, LV_FLEX_FLOW_COLUMN);
  lv_obj_add_style(lv_screen_active(), &style_home, 0);
  lv_obj_add_style(clocks, &style_home, 0);
  lv_obj_add_style(localClock, &style_clockContainers, 0);
  lv_obj_add_style(worldClock, &style_clockContainers, 0);
  lv_obj_add_style(localClockIcon, &style_regionIcons, 0);
  lv_obj_add_style(worldClockIcon, &style_regionIcons, 0);
  lv_label_set_text(localClockTime, "Fetching Time...");
  lv_label_set_text(worldClockTime, "Fetching Time...");
  lv_obj_add_style(localClockTime, &style_clocks_fetching, 0);
  lv_obj_add_style(worldClockTime, &style_clocks_fetching, 0);
  lv_timer_t * clockRefreshTimer = lv_timer_create(clockRefresh, 1000, NULL);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  tft.init();
  tft.setRotation(3);

  configTime(GMT_OFFSET, DST_OFFSET, NTP_SERV);  
}

void loop() {
  lv_timer_handler();
  delay(20);
}

void readInput(lv_indev_t * indev, lv_indev_data_t * data) {
  uint16_t x;
  uint16_t y;
  if(tft.getTouch(&x, &y) != 0){
    data->point.x = x;
    data->point.y = y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void updateTime(char *localTimeString, char *worldTimeString) {
  struct tm gmttimeInfo;
  struct tm *worldTimeInfo;
  struct tm *localTimeInfo;
  time_t timeSeconds;
  
  if(localTimeString == NULL || worldTimeString == NULL) {
    return;
  }

  memset(localTimeString, 0, TIME_STRING_SIZE);
  memset(worldTimeString, 0, TIME_STRING_SIZE);
  
  if(!getLocalTime(&gmttimeInfo)){
    return;
  }
  
  timeSeconds = mktime(&gmttimeInfo);
  time_t localTimeSeconds = timeSeconds + (LOCAL_GMT_OFFSET * 3600);
  localTimeInfo = localtime(&localTimeSeconds);
  strftime(localTimeString, TIME_STRING_SIZE, "%H:%M:%S", localTimeInfo);
  time_t worldTimeSeconds = timeSeconds + (WORLD_GMT_OFFSET * 3600);
  worldTimeInfo = localtime(&worldTimeSeconds);
  strftime(worldTimeString, TIME_STRING_SIZE, "%H:%M:%S", worldTimeInfo);
  lv_obj_add_style(localClockTime, &style_clocks, 0);
  lv_obj_add_style(worldClockTime, &style_clocks, 0);
  
  return;
}

void clockRefresh(lv_timer_t * timer) {
  updateTime(localTimeString, worldTimeString);
  lv_label_set_text(localClockTime, localTimeString);
  lv_label_set_text(worldClockTime, worldTimeString);
};

uint32_t get_millis(){
    return esp_timer_get_time() / 1000;
}

void flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf){
  uint32_t w = area->x2 - area->x1 + 1;
  uint32_t h = area->y2 - area->y1 + 1;
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushPixels(px_buf, w * h);
  tft.endWrite();
  lv_display_flush_ready(disp);
}

