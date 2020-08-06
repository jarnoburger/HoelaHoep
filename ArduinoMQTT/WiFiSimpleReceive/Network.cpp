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
char         ssid[]      = SECRET_SSID;     
char         pass[]      = SECRET_PASS;    
const char   broker[]    = SECRET_IP;
int          port        = SECRET_PORT;

//const char topic[]     = SECRET_TOPIC;
//String     topicRemote = SECRET_TOPICONMOBILE; 
//String     topicLocal  = SECRET_TOPICONARDUINO;
WiFiClient _wifiClient;
MqttClient _mqttClient(_wifiClient);

Network::Network(){
  count       = 0;
  interval    = SECRET_INTERVAL;;
}

void Network::Start(Taken taken){
  _taken = taken;

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

// subscribe to the topics that come from the phone
  SubScribeToTopic(topiccolorA);
  SubScribeToTopic(topiccolorB);
  SubScribeToTopic(topicSpeed);
  SubScribeToTopic(topicEffect);
}

void Network::SubScribeToTopic(String topic){
  // subscribe to changes from phone
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();
  _mqttClient.subscribe(topic);
}

void Network::Loop(){
  // read if there are messages for subscribed topics
  int messageSize = _mqttClient.parseMessage();
  if (messageSize) {
    // there are messages ! read them now.
    OnMessage(messageSize);
  }

  // avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    SendDebug("Wazaa puppy!");

    count++;
  }
}

void Network::SendDebug(String message){
    bool retained = false;
    int qos = 1;
    bool dup = false;
    _mqttClient.beginMessage(topicStatus, message.length(), retained, qos, dup);
    _mqttClient.print(message);
    _mqttClient.endMessage();
}

void Network::DebugIncomingMessage(int messageSize){
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
}

void Network::OnMessage(int messageSize){
  // we received a message, print out the topic and contents
  DebugIncomingMessage(messageSize);

  // get the topic
  String topic = _mqttClient.messageTopic();

  // get the value  
  char c;
  int i = 0;
  char bericht[messageSize];
  while (_mqttClient.available()) {
    c = _mqttClient.read();
    bericht[i] = c;
    i++;
    //Serial.print((char)_mqttClient.read());
  }
  Serial.print("Created : ");
  Serial.println(bericht);
  _taken.Parse(topic,bericht);
}

//void Network::SendHelloWorld(){
//    String payload;
//
//    payload += "I am the arduino!";
//    payload += " ";
//    payload += count;
//
//    Serial.print("Sending message to topic: [");
//    Serial.print(topicRemote);
//    Serial.print("] [");
//    Serial.print(payload);
//    Serial.println("]");
//
//    // send message, the Print interface can be used to set the message contents
//    // in this case we know the size ahead of time, so the message payload can be streamed
//
//    bool retained = false;
//    int qos = 1;
//    bool dup = false;
//
//    _mqttClient.beginMessage(topicRemote, payload.length(), retained, qos, dup);
//    _mqttClient.print(payload);
//    _mqttClient.endMessage();
//
//    Serial.println();
//}
