#include <Arduino.h>
#include <ili9486.h>
#include <WiFi.h>
#include <time.h>
#include <env.h>

//Display Defines
//GPIOs for SPI
#define SPI_MOSI      23
#define SPI_MISO      19
#define SPI_SCK       18

// GPIOs for TFT/TP
#define TFT_CS        22
#define TFT_DC        12
#define TFT_RST       14
#define TP_CS         21
// #define TP_IRQ        
#define TIME_STRING_SIZE 128

TFT tft;

char localTimeString[TIME_STRING_SIZE];
char worldTimeString[TIME_STRING_SIZE];

#define LED_BUILTIN 5

void updateTime();
// put function declarations here:
// int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH);
  tft.begin(TFT_CS, TFT_DC, VSPI, SPI_MOSI, SPI_MISO, SPI_SCK);
  tft.setRotation(1);
  tft.fillScreen(TFT_DEEPSKYBLUE);
  tft.setFont(Times_New_Roman27x21);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 30);
  tft.println("PENIS ACTIVATING!!!");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // char text[1024];
  while(WiFi.status() != WL_CONNECTED){
    tft.fillScreen(TFT_DEEPSKYBLUE);
    tft.setCursor(20, 30);
    Serial.println("Connecting to WiFi...");
    tft.println("CONNECTING PENIS TO WIFI");
    // sprintf((char *)&text, "Wifi status: %d", WiFi.status());
    // tft.println(text);
    delay(1200);
  }
  tft.fillScreen(TFT_DEEPSKYBLUE);
  Serial.println("Successfully connected to WiFI at Address:");
  Serial.println(WiFi.localIP());
  tft.println("PENIS CONNECTED (TO WIFI) UWU~!");
  tft.println(WiFi.localIP());

  Serial.println("Connecting to NTP Server...");
  configTime(GMT_OFFSET, DST_OFFSET, NTP_SERV);
  pinMode (LED_BUILTIN, OUTPUT);
}

void loop() {
  updateTime();
  tft.fillScreen(TFT_DEEPSKYBLUE);
  tft.setCursor(20,30);
  tft.println(localTimeString);
  tft.println(worldTimeString);
  delay(1000);
}

// put function definitions here:
void updateTime() {
  struct tm gmttimeInfo;
  struct tm *tmpTimeInfo;
  struct tm worldTimeInfo;
  struct tm localTimeInfo;
  time_t timeSeconds;

  if(!getLocalTime(&gmttimeInfo)){
    return;
  }
  timeSeconds = mktime(&gmttimeInfo);
  time_t localTimeSeconds = timeSeconds + (LOCAL_GMT_OFFSET * 3600);
  tmpTimeInfo = localtime(&localTimeSeconds);
  memcpy(&localTimeInfo, tmpTimeInfo, sizeof(struct tm));
  time_t worldTimeSeconds = timeSeconds + (WORLD_GMT_OFFSET * 3600);
  tmpTimeInfo = localtime(&worldTimeSeconds);
  memcpy(&worldTimeInfo, tmpTimeInfo, sizeof(struct tm));


  memset(&localTimeString, 0, TIME_STRING_SIZE);
  memset(&worldTimeString, 0, TIME_STRING_SIZE);

  strftime((char *)&localTimeString, TIME_STRING_SIZE, "%A, %B %d %Y %H:%M:%S", &localTimeInfo);
  strftime((char *)&worldTimeString, TIME_STRING_SIZE, "%A, %B %d %Y %H:%M:%S", &worldTimeInfo);

  Serial.println(localTimeString);
  Serial.println(worldTimeString);

  return;
}
