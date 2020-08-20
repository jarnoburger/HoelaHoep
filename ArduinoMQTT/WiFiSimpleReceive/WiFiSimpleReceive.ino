#include "Wiskunde.h"
#include "Math.h"
#include <ArduinoMqttClient.h>
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
led     led;
taken   taken;
network network;

unsigned long current_millis;
unsigned long previous_millis;
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
  Serial.print("Led built in pin : ");
  Serial.println(LED_BUILTIN);

  led.start();
  
  Serial.println("*** Starting network");
  taken.start(led);
  network.start(taken);
}

void loop() {
  led.loop();
  network.loop();
}