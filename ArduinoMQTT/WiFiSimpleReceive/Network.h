#ifndef Network_h
#define Network_h

#include <Arduino.h>
#include "arduino_secrets.h"
#include <ArduinoMqttClient.h>
#include "Taken.h"

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

#define topicStatus "STATUS" // published om te zien op de phone >> mqttClient.beginMessage() en mqttClient.endMessage
#define topiccolorA "COLORA" // subscribed om te ontvangen op de arduino
#define topiccolorB "COLORB" // subscribed om te ontvangen op de arduino
#define topicSpeed  "SPEED"  // subscribed om te ontvangen op de arduino
#define topicEffect "EFFECT" // subscribed om te ontvangen op de arduino

class Network
{
  private:
    int count;
    int interval;
    
    Taken _taken;

    unsigned long previousMillis;
    void OnMessage(int messageSize);
    void SubScribeToTopic(String topic);
    void DebugIncomingMessage(int messageSize);
    void Test();

    
  public:
    Network();
    void Start(Taken taken);
    void Loop();
    void SendDebug(String message);   
};

#endif
