/*****
 * mqtt_bike.h 
 * 
 * MARCUS BIKE PROJECT 
 * ESP32 & EPAPRER
 * Epaper libery FILE  
 * 
 * Author : B
 * Version 0.1
 * 2018
 * 
 */

// pinout  
/* ESP32
    eink Display    ESP32
    Busy        IO04
    reset       IO16 // IO19 moved to free up the LED
    DC          IO17
    CS          IO5
    CLK         IO18
    DIN         IO23
    GND         GND
    3.3V        3.3V
 */


#include "Arduino.h"
#ifndef mqtt_bike_h
#define mqtt_bike_h


void receivedCallback(char* topic, byte* payload, unsigned int length) ;
void mqttconnect();
void mqtt_bike_init();
void mqtt_bike_loop();

String getCounter();

#endif
