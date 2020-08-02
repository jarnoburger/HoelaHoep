#ifndef Network_h
#define Network_h

#include "Arduino.h"
#include "arduino_secrets.h"
#include <ArduinoMqttClient.h>

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

class Network
{
  private:
    int count;
    int interval;
    unsigned long previousMillis;
    
  public:
    Network();
    void Start();
    void Loop();
    void SendHelloWorld();
    void OnMessage(int messageSize);
};

#endif
