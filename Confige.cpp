#include "Arduino.h"
#include "Confige.h"

// the #include statment and code go here...


//// retek 



#define INTERVAL 1000  // sampling interval in millisec
#define HR_TO_SEC 2.778E-4  // number of hours in a second

const int analogIn = 34;

int curr_rate = 0;  // current flow rate in L/hr
unsigned long lastRead = 0;  // the last time the rate was sampled
float totalVolume = 0; 

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
float amps = 0.0;


void nullWh()
{
  totalVolume = NULL;    
}





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
 batVal =  12; // analogRead(batMonPin);    // read the voltage on the divider 
 pinVoltage = batVal * 0.00488;       //  Calculate the voltage on the A/D pin
                                   //  A reading of 1 for the A/D = 0.0048mV
                                   //  if we multiply the A/D reading by 0.00488 then 
                                   //  we get the voltage on the pin.  

 batteryVoltage = pinVoltage * ratio;    //  Use the ratio calculated for the voltage divider
                                         //  to calculate the battery voltage
                                         
                                           // print the results to the serial monitor:
 amps = (float) outputValue / 1000;
 
 Serial.print("Volts = " );                       
 Serial.print(batteryVoltage);      
 Serial.print("\t Current (amps) = ");      
 Serial.print(amps);  
 Serial.print("\t Power (Watts) = ");   
 
 float watts = amps * batteryVoltage;
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

void calcWh( int newValue)
{
   if (millis() - lastRead >= INTERVAL){
    lastRead += INTERVAL;
    curr_rate = newValue;   // or whatever function you call; should take << 1 sec
    totalVolume += curr_rate * HR_TO_SEC;  // update the volume
    Serial.printf("Wh: ");
    Serial.println(totalVolume, 4);
   
  }
}




////


void init(){
  
  pinMode(16, OUTPUT);
  pinMode(34, INPUT);

  Serial.begin(BAUDRATE_SERIAL);
  Serial.println();
  Serial.println("setup");

  // init networkd
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt_bike_init();

  }







