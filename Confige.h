/*****
 * Confige.h 
 * 
 * MARCUS BIKE PROJECT 
 * ESP32 & EPAPRER
 * CONFIGURAION FILE  
 * 
 * Author : B
 *  Version 0.1
 * 2018
 * 
 */

#include "Arduino.h"
#ifndef Confige_h
#define Confige_h

// MAIN INCLUDE:

// include library, include base class, make path known

#include <WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h> //
#include <math.h>
#include <ArduinoJson.h>
#include "Display.h"
#include "mqtt_bike.h"

/* bike hardware ID*/
#define HW_ID  1 

/* serial baud rate */
#define BAUDRATE_SERIAL 115200


/* change it with your ssid-password */
#define ssid  "PLUSNET-ZM82"

//const char* password = "6bfbe7be49";
#define password "6bfbe7be49"

//String clientId = "ESP32Client";
#define clientID  "ESP32Client"

/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
//const char* mqtt_server = "192.168.0.51";
#define mqtt_server  "192.168.0.51"


// MQTT topic sytem // temmproarry


/* topics */

#define TEMP_TOPIC    "bikes/bike1/temp"
#define LED_TOPIC     "bikes/bike1/led" /* 1=on, 0=off */
#define POS_TOPIC     "bikes/bike1/pos"
#define COM_TOPIC     "bikes/bike1/com"
#define WH_TOPIC      "bikes/bike1/wh"
#define MAIN_TOPIC    "bikes/bike1/#"



// system int 
void inti();

// retek 

void calcWh( int newValue);
void calcWh2();
void nullWh();

#endif
