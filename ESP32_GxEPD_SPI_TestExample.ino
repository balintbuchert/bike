/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

   DESTM32-S2 pinout (top, component side view):
         |-------------------------------------------------
         |  VCC  |o o| VCC 5V  not needed
         |  GND  |o o| GND
         |  3.3  |o o| 3.3     3.3V
         |  nc   |o o| nc
         |  nc   |o o| nc
         |  nc   |o o| nc
   MOSI  |  DIN  |o o| CLK     SCK
   SS    |  CS   |o o| DC      e.g. D3
   D4    |  RST  |o o| BUSY    e.g. D2
         |  nc   |o o| BS      GND
         |-------------------------------------------------
*/
 
// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping suggestion from Waveshare SPI e-Paper to generic ESP8266
// BUSY -> GPIO4, RST -> GPIO2, DC -> GPIO0, CS -> GPIO15, CLK -> GPIO14, DIN -> GPIO13, GND -> GND, 3.3V -> 3.3V

// mapping suggestion for ESP32, e.g. LOLIN32, see .../variants/.../pins_arduino.h for your board
// NOTE: there are variants with different pins for SPI ! CHECK SPI PINS OF YOUR BOARD
// BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V

// new mapping suggestion for STM32F1, e.g. STM32F103C8T6 "BluePill"
// BUSY -> A1, RST -> A2, DC -> A3, CS-> A4, CLK -> A5, DIN -> A7

// mapping suggestion for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, CS-> 10, CLK -> 13, DIN -> 11

// include library, include base class, make path known
#include <GxEPD.h>

// select the display class to use, only one
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>    // 1.54" b/w
//#include <GxGDEW0154Z04/GxGDEW0154Z04.cpp>  // 1.54" b/w/r
//#include <GxGDE0213B1/GxGDE0213B1.cpp>      // 2.13" b/w
//#include <GxGDEW0213Z16/GxGDEW0213Z16.cpp>  // 2.13" b/w/r
//#include <GxGDEH029A1/GxGDEH029A1.cpp>      // 2.9" b/w
//#include <GxGDEW029Z10/GxGDEW029Z10.cpp>    // 2.9" b/w/r
//#include <GxGDEW027C44/GxGDEW027C44.cpp>    // 2.7" b/w/r
//#include <GxGDEW027W3/GxGDEW027W3.cpp>      // 2.7" b/w
//#include <GxGDEW042T2/GxGDEW042T2.cpp>      // 4.2" b/w
//#include <GxGDEW042Z15/GxGDEW042Z15.cpp>    // 4.2" b/w/r
//#include <GxGDEW075T8/GxGDEW075T8.cpp>      // 7.5" b/w
//#include <GxGDEW075Z09/GxGDEW075Z09.cpp>    // 7.5" b/w/r

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// pins_arduino.h, e.g. LOLIN32
//static const uint8_t SS    = 5;   CS
//static const uint8_t MOSI  = 23;  DIN
//static const uint8_t MISO  = 19;  Do
//static const uint8_t SCK   = 18;  CLK

/* ESP32
    eink Display    ESP32
    Busy        IO04
    reset       IO16
    DC          IO17
    CS          IO5
    CLK         IO18
    DIN         IO23
    GND         GND
    3.3V        3.3V
 */

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 17, 16); // arbitrary selection of 17, 16
// GxGDEP015OC1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
GxEPD_Class display(io, 16, 4); // arbitrary selection of (16), 4


//#if defined(_GxGDEP015OC1_H_)
const uint32_t partial_update_period_s = 1;
const uint32_t full_update_period_s = 6 * 60 * 60;
//#elif defined(_GxGDE0213B1_H_) || defined(_GxGDEH029A1_H_) || defined(_GxGDEW042T2_H_) || defined(_GxGDEW042T2_FPU_H_)
//const uint32_t partial_update_period_s = 2;
//const uint32_t full_update_period_s = 1 * 60 * 60;
//#endif

uint32_t start_time;
uint32_t next_time;
uint32_t next_full_update;

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init();
  Serial.println("setup done");
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(1);
  // draw background

  //display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.update();
  display.setFont(&FreeMonoBold12pt7b);

  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  //display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
  start_time = next_time = millis();
  next_full_update = start_time + full_update_period_s * 1000;
}


void loop()
{
  showPartialUpdate(); 
  next_time += partial_update_period_s * 1000;
  while (millis() < next_time) delay(100);
}



void print02d(uint32_t d)
{
  if (d < 10) display.print("0");
  display.print(d);
}

void showPartialUpdate()
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 170;
  uint16_t box_h = 40;
  uint16_t cursor_y = box_y + 16;
  uint32_t elapsed_seconds = (millis() - start_time) / 1000;
  uint32_t seconds = elapsed_seconds % 60;
  uint32_t minutes = (elapsed_seconds / 60) % 60;
  uint32_t hours = (elapsed_seconds / 3600) % 24;
  uint32_t days = (elapsed_seconds / 3600) / 24;
  bool test_v = false;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  //display.print(days); display.print("d "); print02d(hours); display.print(":"); print02d(minutes); display.print(":"); print02d(seconds);
  display.setCursor(box_x, cursor_y+20);
  //display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  display.printf("%d\n", test_v);
  display.updateWindow(box_x, box_y, box_w, box_h, true);
}


/*
void drawCallback()
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 170;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + 16;
  uint32_t elapsed_seconds = (millis() - start_time) / 1000;
  uint32_t seconds = elapsed_seconds % 60;
  uint32_t minutes = (elapsed_seconds / 60) % 60;
  uint32_t hours = (elapsed_seconds / 3600) % 24;
  uint32_t days = (elapsed_seconds / 3600) / 24;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  display.print(days); display.print("d "); print02d(hours); display.print(":"); print02d(minutes); display.print(":"); print02d(seconds);
}

*/


