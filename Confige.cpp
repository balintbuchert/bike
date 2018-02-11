#include "Arduino.h"
#include "Confige.h"

// the #include statment and code go here...




void init(){
  
  pinMode(16, OUTPUT);
  pinMode(34, INPUT);

  Serial.begin(BAUDRATE_SERIAL);
  Serial.println();
  Serial.println("setup");
  }


