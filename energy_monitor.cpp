#include "Arduino.h"
//#include "Confige.h"
#include "energy_monitor.h"
#include <ResponsiveAnalogRead.h>
#include <driver/adc.h>

// define the pin you want to use
const int ANALOG_PIN = 34;

const int analogIn = 34;
int sensor_pure_value= 0;
float AcsValue=0.0,Samples=0.0,AvgAcs=0.0,AcsValueF=0.0;

ResponsiveAnalogRead analog(ANALOG_PIN, true);

float amps = 0.0;
float watts = 0.0;


#define INTERVAL 1000  // sampling interval in millisec
#define HR_TO_SEC 2.778E-4  // number of hours in a second


float r = 0;


int curr_rate = 0;  // current flow rate in W/s
unsigned long lastRead = 0;  // the last time the rate was sampled
float totalVolume = 0; 


float get_Watt()
{
  //calck_A();
  
  //calc_W();
  return watts;
  
}
float get_Wh(){   
   return totalVolume;
  }

void calc_energy(){
  calck_A();
  calc_W();
  calc_Wh();
   }

void calc_W(){
   watts= amps* 13;
  
  
  }

int n = 0;
const int  adcPin = 34;
float null_volt = 0.600;
float c = (null_volt / 287)* 1023; 

void calck_A()
{
    analog.update();

  //Serial.print(analog.getRawValue());
  //Serial.print("\t");
  Serial.print(analog.getValue());
  
  //// if the repsonsive value has change, print out 'changed'
  
  //if(analog.hasChanged()) {
    //Serial.print("\tchanged");
 // }

  int i = 100;
    //int val = adc1_get_voltage(ADC1_CHANNEL_6);
    int av =  analog.getValue();//adc1_get_voltage(ADC1_CHANNEL_6);//analogRead(34);                       // ADC12 on GPIO2
    for(n=1; n<i; n++) av += analog.getValue();//adc1_get_voltage(ADC1_CHANNEL_6);//analogRead(34);
    av /= i;
    Serial.print(" ADC12 = ");
    Serial.println(av,DEC);

    float  volt ;
    float d =  (4.535/2043);//0.001221;
     
     volt =  ((av) * d);
     Serial.print("V: ");
     Serial.println(volt);

     float null_volt = 0.627;
     float mv_A= 0.048979;
     float a = (( volt-null_volt )/mv_A);

       a = a-0.5;
      if(a <= 0) 
       a = 0;
       
       amps = a;
       
      Serial.print("amper: ");
      Serial.println(a);
       
       
      
      //Serial.print("amper: ");
      //Serial.println(a);
    
    
    //delay(1000);
} 




void nullWh()
{
  totalVolume = NULL;    
}


void calc_Wh(){
    float newValue = watts;
   if (millis() - lastRead >= INTERVAL){
    lastRead += INTERVAL;
    curr_rate = newValue;   // or whatever function you call; should take << 1 sec
    //curr_rate = sensor_pure_value;   // or whatever function you call; should take << 1 sec
    
    totalVolume += curr_rate * HR_TO_SEC;  // update the volume
    //Serial.printf("Wh: ");
    //Serial.println(totalVolume, 4);
    
    
  }
  
}



//#define INTERVAL 1000  // sampling interval in millisec
//#define HR_TO_SEC 2.778E-4  // number of hours in a second


//const int analogIn = 34;

//int curr_rate = 0;  // current flow rate in L/hr
//unsigned long lastRead = 0;  // the last time the rate was sampled
//float totalVolume = 0; 

int batMonPin = 34;    // input pin for the voltage divider
int batVal = 0;       // variable for the A/D value
float pinVoltage = 0; // variable to hold the calculated voltage
float batteryVoltage = 0;
float ratio = 3.2;  // Change this to match the MEASURED ration of the circuit
int analogInPin = A0;  // Analog input pin that the carrier board OUT is connected to
int sensorValue = 0;        // value read from the carrier board
int outputValue = 0;        // output in milliamps
unsigned long msec = 0;
float time2 = 0.0;
int sample = 0;
float totalCharge = 0.0;
float averageAmps = 0.0;
unsigned long hours = 0;
unsigned long minutes = 0;
float ampHours = 0.0;
float wattHours = 0.0;





void calcWh2()
{
   
 // read the analog in value:
 sensorValue = analogRead(analogIn);            
 // convert to milli amps
 outputValue = (((long)sensorValue * 5000 / 1024) - 500 ) * 1000 / 133;  
 
/* sensor outputs about 100 at rest. 
Analog read produces a value of 0-1023, equating to 0v to 5v.
"((long)sensorValue * 5000 / 1024)" is the voltage on the sensor's output in millivolts.
There's a 500mv offset to subtract. 
The unit produces 133mv per amp of current, so
divide by 0.133 to convert mv to ma
         
*/


 batVal =  13; // analogRead(batMonPin);    // read the voltage on the divider 
 pinVoltage = batVal * 0.0066;       //  Calculate the voltage on the A/D pin
                                   //  A reading of 1 for the A/D = 0.0048mV
                                   //  if we multiply the A/D reading by 0.00488 then 
                                   //  we get the voltage on the pin.  

 batteryVoltage = pinVoltage * ratio;    //  Use the ratio calculated for the voltage divider
                                         //  to calculate the battery voltage
                                         
                                           // print the results to the serial monitor:
 //amps = (float) outputValue / 1000;
 
 Serial.print("Volts = " );                       
 Serial.print(batteryVoltage);      
 Serial.print("\t Current (amps) = ");      
 Serial.print(amps);  
 Serial.print("\t Power (Watts) = ");   
 
 watts = amps * 13;
 Serial.print(watts);   
 
   
 sample = sample + 1;
 
 msec = millis();
 
 totalCharge = totalCharge + amps;
 
 minutes = msec/1000/60;
   
 time2 = (float) minutes / 60;
 
 ampHours = totalCharge/time2;
 
 averageAmps = totalCharge / sample;
 
 ampHours = averageAmps*time2;
 
 wattHours = batteryVoltage * ampHours;
 

 Serial.print("\t Time (hours) = ");
 Serial.print(time2);
 
 Serial.print("\t Amp Hours (ah) = ");
 Serial.print(ampHours);
 Serial.print("\t Watt Hours (wh) = ");
 Serial.println(wattHours);
  
  }

  void initEnergy(){
   adc1_config_width(ADC_WIDTH_11Bit);
   adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_11db);
    
    }

