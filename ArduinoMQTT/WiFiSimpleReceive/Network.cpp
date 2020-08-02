#include <Arduino.h>
#include "Network.h"
#include "arduino_secrets.h"
#include <ArduinoMqttClient.h>
#include <MqttClient.h>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate flashed in the WiFi module.
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char       ssid[]      = SECRET_SSID;     
char       pass[]      = SECRET_PASS;    
const char broker[]    = SECRET_IP;
int        port        = SECRET_PORT;
const char topic[]     = SECRET_TOPIC;
String     topicRemote = SECRET_TOPICONMOBILE; 
String     topicLocal  = SECRET_TOPICONARDUINO;
WiFiClient _wifiClient;
MqttClient _mqttClient(_wifiClient);

Network::Network(){
  count       = 0;
  interval    = SECRET_INTERVAL;;
}

void Network::Start(){
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // _mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  // _mqttClient.setUsernamePassword("username", "password");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!_mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(_mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  Serial.print("Subscribing to topic: ");
  Serial.println(topicLocal);
  Serial.println();

  // subscribe to a topic
  _mqttClient.subscribe(topicLocal);

  // topics can be unsubscribed using:
  // _mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topicLocal);
  Serial.println();
}

void Network::Loop(){
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  _mqttClient.poll();

  // avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    SendHelloWorld();

    count++;
  }
}

void Network::SendHelloWorld(){
    String payload;

    payload += "hello world!";
    payload += " ";
    payload += count;

    Serial.print("Sending message to topic: ");
    Serial.println(topicRemote);
    Serial.println(payload);

    // send message, the Print interface can be used to set the message contents
    // in this case we know the size ahead of time, so the message payload can be streamed

    bool retained = false;
    int qos = 1;
    bool dup = false;

    _mqttClient.beginMessage(topicRemote, payload.length(), retained, qos, dup);
    _mqttClient.print(payload);
    _mqttClient.endMessage();

    Serial.println();
}

void Network::OnMessage(int messageSize){
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(_mqttClient.messageTopic());
  Serial.print("', duplicate = ");
  Serial.print(_mqttClient.messageDup() ? "true" : "false");
  Serial.print(", QoS = ");
  Serial.print(_mqttClient.messageQoS());
  Serial.print(", retained = ");
  Serial.print(_mqttClient.messageRetain() ? "true" : "false");
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (_mqttClient.available()) {
    Serial.print((char)_mqttClient.read());
  }
  Serial.println();

  Serial.println();
}