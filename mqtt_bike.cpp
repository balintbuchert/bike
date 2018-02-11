#include "Arduino.h"
#include "mqtt_bike.h"
#include "Confige.h"


/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);


char bikePos = '0';
int  com = 1;     // the tompic command 0 : stop / 1 : start "runing" / 2 : reset 

long lastMsg = 0;
char msg[20];
char msg2[20];
volatile bool onoff = 0;
float randNumber;

const int analogIn = 34;

String output;// json main output string



////////

// init Json objectl
 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 char data[80];

////


void mqtt_bike_init()
{
 /* configure the MQTT server with IPaddress and port */
  client.setServer(mqtt_server, 1883);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);

    /// JSON test config 
  root["wh"] = 12;
  root["w"] = 0.44;
  root["id"] = HW_ID;
  root ["V"] = 12.1;

 
  JsonArray& data = root.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);  
}


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
      //onoff= 1;   
    } else {
     /* we got '0' -> on */
     digitalWrite(16, HIGH);
      //onoff = 0;
    }
  }

  // pos check and load
  if(strcmp(topic, POS_TOPIC) == 0)
    if (!isnan((int)payload[0])) {
        int bikePos= payload[0]; 
      }


  // com topic
  if(strcmp(topic, COM_TOPIC) == 0)   
     if (!isnan((int)payload[0])) {          
        switch ((int)payload[0]-48) {
          case (1):{ // start 
             Serial.println("START");
             //bikePos= '0'; 
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
              //bikePos= '8' ;
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

void mqtt_bike_loop(){
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
    /* need a total volume re instert vith function 
    if (!isnan(totalVolume)){
       snprintf (msg2, 20, "%lf", totalVolume);
      
       client.publish(WH_TOPIC, msg2);
      
      } */     
      
  }

  String value = "\"number\": " + String(analogRead(A0)) ;
 String value2 = ", \"word\": \"Hello World\" " ;
 
 // Add both value together to send as one string. 
  value = value + value2;
  // This sends off your payload. Json file. 
 
  
  String payload = output;//"{ \"devices\": \"*\",\"payload\": {" + value + "}}";
  payload.toCharArray(data, (payload.length() + 1));
  
  client.publish("message", data);

  
  }
///////////////////////////////////////////////////////
