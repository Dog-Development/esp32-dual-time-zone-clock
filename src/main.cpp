#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <env.h>
#include <lvgl.h>
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

char localTimeString[TIME_STRING_SIZE];
char worldTimeString[TIME_STRING_SIZE];

uint32_t get_millis();
void updateTime();
void flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf);
void readInput(lv_indev_t * indev, lv_indev_data_t * data);

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(3);

  
  //LVGL Initialization
  Serial.println("Beginning LVGL Initialization...");
  lv_init();
  lv_tick_set_cb(get_millis);
  lv_display_t * display = lv_display_create(TFT_HEIGHT, TFT_WIDTH);
  lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565_SWAPPED);
  buf = (uint8_t *)calloc(DISPLAY_BUFFER_SIZE, 1);
  lv_display_set_buffers(display, buf, NULL, DISPLAY_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, flush);
  Serial.println("LVGL Initialized.");
  
  //LVGL Input Device Initialization
  Serial.println("Beginning LVGL Input Device Initialization...");
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, readInput);
  Serial.println("LVGL Input Device Initialized.");

  //Home Screen Setup
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x8F00FF), LV_PART_MAIN);
  lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_layout(lv_screen_active(), LV_LAYOUT_FLEX);
  lv_obj_set_style_pad_all(lv_screen_active(), 10, LV_PART_MAIN);
  lv_obj_t * clocks = lv_obj_create(lv_screen_active());
  lv_obj_set_style_bg_color(clocks, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_t * localClock = lv_obj_create(clocks);
  lv_obj_set_width(localClock, 120);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting to WiFi...");
    delay(500);
  }
  Serial.println("Successfully connected to WiFI at Address:");
  Serial.println(WiFi.localIP());

  configTime(GMT_OFFSET, DST_OFFSET, NTP_SERV);
  Serial.println("Connecting to NTP Server...");
  
}

void loop() {
  updateTime();
  lv_timer_handler();
  delay(17);
}

void readInput(lv_indev_t * indev, lv_indev_data_t * data){
  uint16_t x;
  uint16_t y;
  if(tft.getTouch(&x, &y) != 0){
    data->point.x = x;
    data->point.y = y;
    data->state = LV_INDEV_STATE_PRESSED;
    Serial.println("Touch Detected");
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void updateTime() {
  struct tm gmttimeInfo;
  struct tm *worldTimeInfo;
  struct tm *localTimeInfo;
  time_t timeSeconds;

  memset(&localTimeString, 0, TIME_STRING_SIZE);
  memset(&worldTimeString, 0, TIME_STRING_SIZE);
  
  if(!getLocalTime(&gmttimeInfo)){
    return;
  }
  
  timeSeconds = mktime(&gmttimeInfo);
  time_t localTimeSeconds = timeSeconds + (LOCAL_GMT_OFFSET * 3600);
  localTimeInfo = localtime(&localTimeSeconds);
  strftime((char *)&localTimeString, TIME_STRING_SIZE, "%A, %B %d %Y %H:%M:%S", localTimeInfo);
  time_t worldTimeSeconds = timeSeconds + (WORLD_GMT_OFFSET * 3600);
  worldTimeInfo = localtime(&worldTimeSeconds);
  strftime((char *)&worldTimeString, TIME_STRING_SIZE, "%A, %B %d %Y %H:%M:%S", worldTimeInfo);

  return;
}

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

