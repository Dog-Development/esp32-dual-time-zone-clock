#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <env.h>
//#include <lvgl.h>
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

// TFT tft;
TFT_eSPI tft = TFT_eSPI();

char localTimeString[TIME_STRING_SIZE];
char worldTimeString[TIME_STRING_SIZE];

uint32_t get_millis();
void updateTime();

void setup() {
  pinMode (LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_PURPLE);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 30);
  tft.println("PENIS ACTIVATING!!!");

  //LVGL Initialization
  // lv_init();
  // lv_tick_set_cb(get_millis);
  // lv_display_t * display = lv_display_create(480, 320);
  // static uint8_t buf[480 * 320 / 10 * 2];
  // lv_display_set_buffers(display, buf, NULL, LV_DISPLAY_RENDER_MODE_PARTIAL);
  //lv_display_set_flush_cb(display, );

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while(WiFi.status() != WL_CONNECTED){
    tft.fillScreen(TFT_PURPLE);
    tft.setCursor(20, 30);
    Serial.println("Connecting to WiFi...");
    tft.println("CONNECTING PENIS TO WIFI >:3");
    delay(1200);
  }

  tft.fillScreen(TFT_PURPLE);
  Serial.println("Successfully connected to WiFI at Address:");
  Serial.println(WiFi.localIP());
  tft.println("PENIS CONNECTED (TO WIFI) UWU~!");
  tft.println(WiFi.localIP());

  Serial.println("Connecting to NTP Server...");
  configTime(GMT_OFFSET, DST_OFFSET, NTP_SERV);
}

void loop() {
  updateTime();
  tft.fillScreen(TFT_PURPLE);
  tft.setCursor(20,30);
  tft.println("The local time is:");
  tft.println(localTimeString);
  tft.println("The world time is:");
  tft.println(worldTimeString);
  delay(1000);
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

  Serial.println("The local time is:");
  Serial.println(localTimeString);
  Serial.println("The world time is:");
  Serial.println(worldTimeString);

  return;
}
// uint32_t get_millis(){
//     return esp_timer_get_time() / 1000;
// }

// void flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_buf){
//   display_update(area, px_buf);
//   lv_display_flush_ready();
// }