/*****
 * energy_monitor.h 
 * 
 * MARCUS BIKE PROJECT 
 * ESP32 & EPAPRER
 * W & Wh coalculator 
 * 
 * Author : B
 *  Version 0.1
 * 2018
 * 
 */

#include "Arduino.h"
#ifndef energy_monitor_h
#define energy_monitor_h



void calc_W();

float get_Watt();

void calc_Wh( int newValue);

float get_Wh();

float calc_Wh(float v);

void calc_energy();




//float calcWh( int newValue);
void calcWh2();
void nullWh();

#endif


