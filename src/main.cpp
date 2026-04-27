#include <Arduino.h>
#include <ili9486.h>

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

TFT tft;

// #define LED_BUILTIN 5

// put function declarations here:
// int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  pinMode(TFT_RST, OUTPUT);
  digitalWrite(TFT_RST, HIGH);
  tft.begin(TFT_CS, TFT_DC, VSPI, SPI_MOSI, SPI_MISO, SPI_SCK);
  tft.setRotation(1);
  tft.fillScreen(TFT_DEEPSKYBLUE);
  tft.setFont(Times_New_Roman56x46);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(20, 30);
  tft.print("PENIS!!!");
  // pinMode (LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(uint8_t rotation=0; rotation <4; rotation++){
    tft.setRotation(rotation);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20,30);
    tft.print("PENIS!!!");
    delay(3000);
  }
  // digitalWrite(LED_BUILTIN, HIGH);
  
  // delay(1000);
  
  // digitalWrite(LED_BUILTIN, LOW);
  
  // delay(1000);
}

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }