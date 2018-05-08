/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

*/

/*****
 * Epaper and Node Red test code.
 * Integrated with current sensor. 
 * 
 * Verison : 0.1
 * Author  : B
 */
// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0


#include "Confige.h"



void setup(void){

  init();
  
  //randomSeed(analogRead(0));  
  Serial.println("setup done");
 
}



void loop()
{
 
  mqtt_bike_loop();
  calc_energy();
 
  /// end mqtt init 

  // display data 
 showPartialUpdate();


}





