#include "Arduino.h"
//#include "Confige.h"
#include "energy_monitor.h"

const int analogIn = 34;
int sensor_pure_value= 0;
float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;


void watt_calculator()
{
  sensor_pure_value = analogRead(analogIn); // read mv form current sensro.
  sensor_pure_value =  sensor_pure_value/4;
  
   
}

float get_watt()
{
  watt_calculator();
  return sensor_pure_value;
  
}

void calck_A()
{

//((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
//2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
//out to be 2.5 which is out offset. If your arduino is working on different voltage than 
//you must change the offset according to the input voltage)
//0.066v(66mV) is rise in output voltage when 1A current flows at input

   AcsValue = analogRead(34);     //Read current sensor values 
   AcsValueF = ((2.3 - (AcsValue * (3.3 / 4095)) )/0.066 )-1.7;
   
} 
