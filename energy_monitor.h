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

 void initEnergy();

void calc_W();

float get_Watt();

void calc_Wh();

float get_Wh();
void calc_W();

//float calc_Wh();

void calc_energy();
void calck_A();




//float calcWh( int newValue);
void calcWh2();
void nullWh();

#endif


