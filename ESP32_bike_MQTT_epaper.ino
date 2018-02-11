/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

*/

/*****
 * Epaper and Node Red test code.
 * Integrated with current sensor. 
 * 
 * Verison : 0.1
 * Author  : B
 */
// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0


#include "Confige.h"



char bikePos = '0';
int  com = 1;     // the tompic command 0 : stop / 1 : start "runing" / 2 : reset 

long lastMsg = 0;
char msg[20];
char msg2[20];
volatile bool onoff = 0;
float randNumber;

const int analogIn = 34;

String output;// json main output string

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  if(strcmp(topic, LED_TOPIC) == 0){
    /* we got '1' -> on */
    if ((char)payload[0] == '1') {
      digitalWrite(16, LOW); 
      onoff= 1;   
    } else {
     /* we got '0' -> on */
     digitalWrite(16, HIGH);
      onoff = 0;
    }
  }

  // pos check and load
  if(strcmp(topic, POS_TOPIC) == 0)
    if (!isnan((int)payload[0])) {
        bikePos= payload[0]; 
      }


  // com topic
  if(strcmp(topic, COM_TOPIC) == 0)   
     if (!isnan((int)payload[0])) {          
        switch ((int)payload[0]-48) {
          case (1):{ // start 
             Serial.println("START");
             bikePos= '0'; 
          }
          break;        
          case 2: {// reset
            // statements
             Serial.println("RESRT");
             nullWh();;
             
          }
            break;
          case 0: {// stop
              Serial.println("STOP");
              bikePos= '8' ;
          }
            break;
               
          default:
            // statements
              Serial.println("no messgae/n");
        }// end of case                     
      }// end of if
     
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    //showProcess();
    /* client ID */
    String clientId = clientID;
    /* connect now */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      //client.subscribe(LED_TOPIC);
      //client.subscribe(POS_TOPIC);
      //client.subscribe(COM_TOPIC);
      client.subscribe(MAIN_TOPIC);
      
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}

///////////////////////////////////////////////////////

#define INTERVAL 1000  // sampling interval in millisec
#define HR_TO_SEC 2.778E-4  // number of hours in a second

int curr_rate = 0;  // current flow rate in L/hr
unsigned long lastRead = 0;  // the last time the rate was sampled
float totalVolume = 0; 

////////
// init Json objectl
 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 char data[80];

////

void setup(void)
{
  //pinMode(16, OUTPUT);
  //pinMode(34, INPUT);
  init();
  
  randomSeed(analogRead(0));
 

  //initDisplayData(onoff, randNumber, bikePos);
  
  Serial.println("setup done");
 

  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  //display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
  //start_time = next_time = millis();˜
  //next_full_update = start_time + full_update_period_s * 1000;

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
 

  /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);

 initDisplay();
   /// JSON test config 
  root["wh"] = 12;
  root["w"] = 0.44;
  root["id"] = HW_ID;
  root ["V"] = 12.1;

 
  JsonArray& data = root.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);  
}



void loop()
{

  //showInitStatusUpdate();
 
  
  initDisplayData( onoff, randNumber, bikePos);
 
    /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }
  /* this function will listen for incomming 
  subscribed topic-process-invoke receivedCallback */
  client.loop();
  /* we measure temperature every 3 secs
  we count until 3 secs reached to avoid blocking program if using delay()*/
  long now = millis();
  randNumber = analogRead(analogIn);
  switch (com){
    case 1: // start
       calcWh( randNumber/40);
        calcWh2(); 
    }

 
  
  if (now - lastMsg > 3000) {
    lastMsg = now;
    /* read DHT11/DHT22 sensor and convert to string */
    int temperature = 20;//dht.readTemperature();
    //randNumber = random(300) /100.0 *10;
    //randNumber = analogRead(analogIn);
    Serial.println(randNumber);
    
    if (!isnan(randNumber)) {
      snprintf (msg, 20, "%lf", randNumber/40);
      /* publish the message */
       client.publish(TEMP_TOPIC, msg);
       
    }
    if (!isnan(totalVolume)){
       snprintf (msg2, 20, "%lf", totalVolume);
      /* publish the message */
       client.publish(WH_TOPIC, msg2);
      
      }      
      
  }

 String value = "\"number\": " + String(analogRead(A0)) ;
 String value2 = ", \"word\": \"Hello World\" " ;
 
 // Add both value together to send as one string. 
  value = value + value2;
  
  
  // This sends off your payload. Json file. 
 
  
  String payload = output;//"{ \"devices\": \"*\",\"payload\": {" + value + "}}";
  payload.toCharArray(data, (payload.length() + 1));
  client.publish("message", data);

  /// end mqtt init 
if( showPartialUpdate() ){
  
  Serial.print("OKOKOKOKOK");
  }
  else{
    Serial.print("FUCK");
    }
  
  
  //showBarUpdate();
  //showWhUpdate();
//  next_time += partial_update_period_s * 1000;
 // while (millis() < next_time) delay(100);
  delay(1000);



}

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
void nullWh(){
  totalVolume = NULL;  
  
}

void calcWh2(){
   
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





