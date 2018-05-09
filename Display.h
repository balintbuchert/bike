/*****
 * Dispaly.h 
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
#ifndef Display_h
#define Dislapy_h



// DISPLAY


void initDisplay();

void initDisplayData(bool of, float r, char p);

bool showPartialUpdate(bool con);

void showTestUpdate( );

void showBarUpdate();

void showWhUpdate();

void showProcess();

void statusScreen();

void initDisplay();

void initBackground();

void print02d(uint32_t d);


#endif
