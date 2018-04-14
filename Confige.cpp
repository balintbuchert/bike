#include "Arduino.h"
#include "Confige.h"

// the #include statment and code go here...


//// retek 

/*
float calcWh( int newValue)
{
   if (millis() - lastRead >= INTERVAL){
    lastRead += INTERVAL;
    curr_rate = newValue;   // or whatever function you call; should take << 1 sec
    totalVolume += curr_rate * HR_TO_SEC;  // update the volume
    Serial.printf("Wh: ");
    Serial.println(totalVolume, 4);

    
  }
  return totalVolume;
}
*/



////


void init(){
  
  pinMode(16, OUTPUT);
  pinMode(34, INPUT);
  pinMode(14, INPUT);

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







