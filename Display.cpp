#include "Arduino.h"
//#include "Confige.h"
#include "Display.h"

// MAIN INCLUDE:
#include <GxEPD.h>

// select the display class to use, only one
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>    // 1.54" b/w
// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

//#include GxEPD_BitmapExamples


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>


 bool onoff2 = 0;
 float randNumber2;
 char bikePos2 = '0';
 
GxIO_Class io(SPI, SS, 17, 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, 19, 4); // arbitrary selection of (16), 4 

const uint32_t partial_update_period_s = 1;
const uint32_t full_update_period_s = 6 * 60 * 60;

uint32_t start_time;
uint32_t next_time;
uint32_t next_full_update;


void initDisplay()
{
   display.init();

  display.setTextColor(GxEPD_BLACK);
  display.setRotation(1);
  
  display.update();
  display.setFont(&FreeMonoBold12pt7b);
  start_time = next_time = millis();
  next_full_update = start_time + full_update_period_s * 1000;
   
  
  }

void initDisplayData(bool of , float r, char p){
 onoff2 = of;
 randNumber2 = r;
 bikePos2 = p;
 

}



  
void print02d(uint32_t d)
{
  if (d < 10) display.print("0");
  display.print(d);
}


void showTestUpdate( )
{
  
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 189;
  uint16_t box_h = 75;
  uint16_t cursor_y = box_y + 16;
  uint32_t elapsed_seconds = (millis() - start_time) / 1000;
  uint32_t seconds = elapsed_seconds % 60;
  uint32_t minutes = (elapsed_seconds / 60) % 60;
  uint32_t hours = (elapsed_seconds / 3600) % 24;
  uint32_t days = (elapsed_seconds / 3600) / 24;
  bool test_v = onoff2;
  int vTmp = randNumber2/20;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  //display.print(days); display.print("d "); print02d(hours); display.print(":"); print02d(minutes); display.print(":"); print02d(seconds);
  display.setCursor(box_x, cursor_y+20);
  //display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  display.printf("LED:%d V:%.1d \n", test_v, vTmp);

  display.setCursor(box_x, cursor_y+40);
  display.printf("POS: %d \n", bikePos2-48 );

  display.fillRect(0   ,box_h, vTmp, 6 ,      GxEPD_BLACK);
  display.fillRect(vTmp,box_h, box_w-vTmp , 6 , GxEPD_WHITE);
  
  //display.updateWindow(box_x, box_y, box_w, box_h, true);
  
  
}
  
  
void showBarUpdate()
{
  uint16_t box_x = 160; // strat
  uint16_t box_y = 99; // start 
  uint16_t box_w = 40;
  uint16_t box_h = 100;
  uint16_t separator_high = 2;
  uint16_t box_bottom_x = box_x + box_w;
  uint16_t box_bottom_y = box_y + box_h;
  int vTmp = randNumber2/40;
  
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  //display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
  display.fillRect(box_x, box_bottom_y, box_w,  -1*vTmp,  GxEPD_BLACK);
  // bar parameters ( start point horisontal, strat point vertical, width , level , colour); 
  
  display.fillRect(box_x, box_y+10, box_w, separator_high, GxEPD_WHITE);
  display.fillRect(box_x, box_y+30, box_w, separator_high, GxEPD_WHITE);
  display.fillRect(box_x, box_y+50, box_w, separator_high, GxEPD_WHITE);
  display.fillRect(box_x, box_y+70, box_w, separator_high, GxEPD_WHITE);
  display.fillRect(box_x, box_y+90, box_w, separator_high, GxEPD_WHITE); 
  
  //display.updateWindow(box_x, box_y, box_w, box_h, true);
  
  }
void showWhUpdate()
{
  uint16_t box_x = 40; // strat
  uint16_t box_y = 110; // start 
  uint16_t box_w = 115;
  uint16_t box_h = 80;
  int vTmp = randNumber2/40;
  
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setFont(&FreeMonoBold24pt7b);
  display.setCursor(box_x, box_y+box_h-24);
  //display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.printf("%-dW",vTmp);
  display.setFont(&FreeMonoBold12pt7b);
  
  //display.updateWindow(box_x, box_y, box_w, box_h, true);
  
  }

void showProcess()
{
  uint16_t box_x = 15;
  uint16_t box_y = 15;
  uint16_t box_w = 189;
  uint16_t box_h = 75;
  uint16_t cursor_y = box_y + 16;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.setFont(&FreeMonoBold12pt7b);
  display.printf("Connecting..." );
  display.updateWindow(box_x, box_y, box_w, box_h, true);
  
  }

  
bool showPartialUpdate()
{
  showTestUpdate();
  showBarUpdate();
  showWhUpdate();
  display.updateWindow(0, 0, 199, 199, true); // update all dispaly on one time 
  next_time += partial_update_period_s * 1000;
  while (millis() < next_time) delay(100);
  return true;
}
