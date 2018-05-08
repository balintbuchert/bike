#include "Arduino.h"
#include "Confige.h"

// the #include statment and code go here...
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void init_system(){
  
  pinMode(16, OUTPUT);
  pinMode(34, INPUT);
  pinMode(14, INPUT);
  
  
  //WiFi.enableSTA(true);
  //delay(2000);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
   
  Serial.begin(BAUDRATE_SERIAL);
  Serial.println();
  Serial.println("setup");

  initDisplay();
  
  
  
  // init networkd
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    WiFi.begin(ssid, password); 
    //WiFi.begin(ssid, password);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  statusScreen();
  delay(10000);
  
  
 
  mqtt_bike_init();
  initEnergy();
  initBackground();
  }







