/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

*/
 
// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// include library, include base class, make path known
#include <GxEPD.h>
#include <WiFi.h>
#include <PubSubClient.h>

// select the display class to use, only one
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>    // 1.54" b/w

#include GxEPD_BitmapExamples

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

/* ESP32
    eink Display    ESP32
    Busy        IO04
    reset       IO16
    DC          IO17
    CS          IO5
    CLK         IO18
    DIN         IO23
    GND         GND
    3.3V        3.3V
 */

// DISPLAY
GxIO_Class io(SPI, SS, 17, 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, 16, 4); // arbitrary selection of (16), 4

const uint32_t partial_update_period_s = 1;
const uint32_t full_update_period_s = 6 * 60 * 60;

uint32_t start_time;
uint32_t next_time;
uint32_t next_full_update;

////////
// MQTT
/* change it with your ssid-password */
const char* ssid = "PLUSNET-ZM82";
const char* password = "6bfbe7be49";
/* this is the IP of PC/raspberry where you installed MQTT Server 
on Wins use "ipconfig" 
on Linux use "ifconfig" to get its IP address */
const char* mqtt_server = "192.168.0.50";

/* topics */
#define TEMP_TOPIC    "smarthome/room1/temp"
#define LED_TOPIC     "smarthome/room1/led" /* 1=on, 0=off */

long lastMsg = 0;
char msg[20];
bool onoff = 0;

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
  /* we got '1' -> on */
  if ((char)payload[0] == '1') {
    //digitalWrite(led, HIGH); 
    onoff= 1;   
  } else {
    /* we got '0' -> on */
    //digitalWrite(led, LOW);
    onoff = 0;
  }

}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    String clientId = "ESP32Client";
    /* connect now */
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      /* subscribe topic with default QoS 0*/
      client.subscribe(LED_TOPIC);
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

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init();
  Serial.println("setup done");
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(1);
  // draw background

  //display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.update();
  display.setFont(&FreeMonoBold12pt7b);

  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  //display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
  start_time = next_time = millis();
  next_full_update = start_time + full_update_period_s * 1000;

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
  
}


void loop()
{
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
  if (now - lastMsg > 3000) {
    lastMsg = now;
    /* read DHT11/DHT22 sensor and convert to string */
    int temperature = 20;//dht.readTemperature();
    if (!isnan(temperature)) {
      snprintf (msg, 20, "%lf", temperature);
      /* publish the message */
      client.publish(TEMP_TOPIC, msg);
    }
  }
  
  showPartialUpdate(); 
  next_time += partial_update_period_s * 1000;
  while (millis() < next_time) delay(100);

}



void print02d(uint32_t d)
{
  if (d < 10) display.print("0");
  display.print(d);
}

void showPartialUpdate()
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 170;
  uint16_t box_h = 40;
  uint16_t cursor_y = box_y + 16;
  uint32_t elapsed_seconds = (millis() - start_time) / 1000;
  uint32_t seconds = elapsed_seconds % 60;
  uint32_t minutes = (elapsed_seconds / 60) % 60;
  uint32_t hours = (elapsed_seconds / 3600) % 24;
  uint32_t days = (elapsed_seconds / 3600) / 24;
  bool test_v = onoff;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  //display.print(days); display.print("d "); print02d(hours); display.print(":"); print02d(minutes); display.print(":"); print02d(seconds);
  display.setCursor(box_x, cursor_y+20);
  //display.printf("%0dd %02d:%02d:%02d", days, hours, minutes, seconds);
  display.printf("%d\n", test_v);
  display.updateWindow(box_x, box_y, box_w, box_h, true);
}




