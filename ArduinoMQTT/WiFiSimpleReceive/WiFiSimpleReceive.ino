#include "Led.h"
#include "Taken.h"
#include "Network.h"

/*
  ArduinoMqttClient - WiFi Simple Receive

  This example connects to a MQTT broker and subscribes to a single topic.
  When a message is received it prints the message to the serial monitor.

  The circuit:
  - Arduino MKR 1000, MKR 1010 or Uno WiFi Rev.2 board

  This example code is in the public domain.
*/
Led     _led;
Taken   _taken;
Network _network;

unsigned long currentMillis;
unsigned long previousMillis;
unsigned long interval = 1000;
int count;

void setup() {
  count = 0;

    //Initialize serial and wait for port to open:
  Serial.begin(115200);
  //  while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}
  Serial.println("*** Goededag meneertje !");

  //_led.Start();
  //_taken.Start(_led);
  
  Serial.println("*** Starting network");
  _network.Start();
}

void loop() {
  //_led.Loop();
  _network.Loop();
}