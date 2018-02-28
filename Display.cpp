#include "Arduino.h"
#include "Confige.h"
#include "Display.h"

// MAIN INCLUDE:
#include <GxEPD.h>

// select the display class to use, only one
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>    // 1.54" b/w
// FreeFonts from Adafruit_GFX
//#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
//#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include "IMG_0001.h"
#include "custom_fonts.h"

//#include GxEPD_BitmapExamples


#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

 
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
  
   uint16_t r = GxEPD::bm_invert ;
  // draw background
  uint16_t x = 0;//;(display.width() - 64) / 2;
  uint16_t y = 0;
  //display.fillScreen(GxEPD_WHITE);
  display.drawExampleBitmap(gImage_IMG_0001, x, y, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE,r);
  display.update();
  display.setFont(&SansSerif_bold_32);
  
  //display.update();
  //display.setFont(&FreeMonoBold12pt7b);
  start_time = next_time = millis();
  next_full_update = start_time + full_update_period_s * 1000;
   
  
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
  bool test_v = 0;
  char buf[30];
  
  int bikePos = getBikePos(); 
  getCounter().toCharArray(buf,  getCounter().length()+1);// "0:0";
  
  int vTmp = 20;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  //display.print(days); display.print("d "); print02d(hours); display.print(":"); print02d(minutes); display.print(":"); print02d(seconds);
  display.setCursor(box_x, cursor_y+20);
  //display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  //display.printf("LED:%d V:%.1d \n", test_v, vTmp);
  display.printf("T: %s",buf);
  display.setCursor(box_x, cursor_y+40);
  display.printf("POS: %d \n", bikePos-48 );

  display.fillRect(0   ,box_h, vTmp, 6 ,      GxEPD_BLACK);
  display.fillRect(vTmp,box_h, box_w-vTmp , 6 , GxEPD_WHITE);
  
  //display.updateWindow(box_x, box_y, box_w, box_h, true); 
}
  
  /*
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
*/
 

  int l = 0;
  int max_W = 250;
  int max_hight = 9;
  int bar_step = max_W/ max_hight;

void bar(){

  uint16_t box_x = 6; // strat
  uint16_t box_y = 114; // start 
  uint16_t box_w = 16;
  uint16_t box_h = 100;
  uint16_t separator_high = 4;
  uint16_t box_bottom_x = box_x + box_w;
  uint16_t box_bottom_y = box_y + box_h;
  int vTmp = 40;
  int x_step  = 19;
  int y_step  = 1;
  int h_step  = 1 ;

 
  
  
  int bar_high = floorf(get_Watt() / bar_step)-1; ;// 0-9 
  int m= 9;
  int i = 0;
  
   
  for (i = 0 ; i <= bar_high; i++){
       display.fillRect(box_x+x_step*i, box_y-y_step*i, box_w, separator_high+h_step*i, GxEPD_BLACK);         
    }
  //int j = l;
  for ( int j= bar_high+1  ; j <= 9; j++){
     display.fillRect(box_x+x_step*j, box_y-y_step*j, box_w, (separator_high)+h_step*j, GxEPD_WHITE);   
    }
    //l++;
    if (m != l ){
       l = l+1;
       //Serial.println(l);
      }
       else
       l = 0 ;
 
  //display.updateWindow(0, 0, 200, 200, false); // 
  }


void showMainUpdatea()
{
  uint16_t box_x_W = 18;
  uint16_t box_y_W = 63;
  uint16_t box_w_W = 125;
  uint16_t box_h_W = 40;

  uint16_t box_x_Wh = 18;
  uint16_t box_y_Wh = 120;
  uint16_t box_w_Wh = 125;
  uint16_t box_h_Wh = 38;

  uint16_t box_x_P = 149;
  uint16_t box_y_P = 168;
  uint16_t box_w_P = 48;
  uint16_t box_h_P = 30;

  int bikePos = getBikePos(); 

  //
 //float w = randNumber2/40;
  float w = get_Watt();
  float wh = get_Wh();
  
  char a[3];
  char b[3];
 
  // W
  // float/ min width / is precision/ buffer
  dtostrf(w, 4, 1, a);
  display.setCursor(box_x_W, box_y_W +box_h_W);
  display.fillRect(box_x_W, box_y_W, box_w_W, box_h_W, GxEPD_WHITE);  
  display.printf(a);
  //display.updateWindow(box_x_W, box_y_W, box_w_W, box_h_W, true);

   // Wh   
  dtostrf(wh, 4, 1, b);
  display.setCursor(box_x_Wh, box_y_Wh +box_h_Wh);
  display.fillRect(box_x_Wh, box_y_Wh, box_w_Wh, box_h_Wh, GxEPD_WHITE);  
  display.printf(b);
  
  // position:
  display.setCursor(box_x_P, box_y_P +box_h_P);
  display.fillRect(box_x_P, box_y_P, box_w_P, box_h_P, GxEPD_WHITE);  
  display.printf("%d ",bikePos-48 );
  
   bar();
  display.updateWindow(0, 0, 200, 200, false); // update all dispaly on one time 

 
}
  



  
bool showPartialUpdate()
{
  //showTestUpdate();
  //showBarUpdate();
  //showWhUpdate();
   uint16_t r = GxEPD::bm_invert ;
  // draw background
  uint16_t x = 0;//;(display.width() - 64) / 2;
  uint16_t y = 0;
  //display.fillScreen(GxEPD_WHITE);
  //display.drawExampleBitmap(gImage_IMG_0001, x, y, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE,r);
    
  //display.drawBitmap(gImage_IMG_0001, sizeof(gImage_IMG_0001));
  //display.drawExampleBitmap(gImage_IMG_0001, sizeof(gImage_IMG_0001), GxEPD::bm_default /*| GxEPD::bm_partial_update */| GxEPD::bm_invert | GxEPD::bm_flip_y);
  showMainUpdatea();
  
  //display.update();  
  //display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true); // update all dispaly on one time 
  //next_time += partial_update_period_s * 1000;
  delay(1000);
  //while (millis() < next_time) delay(100);
  return true;
}



