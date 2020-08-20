#ifndef Network_h
#define Network_h

#include <Arduino.h>
#include "Taken.h"

#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

#define TOPIC_STATUS "STATUS" // published om te zien op de phone >> mqttClient.beginMessage() en mqttClient.endMessage
#define TOPIC_COLOR_A "COLORA" // subscribed om te ontvangen op de arduino
#define TOPIC_COLOR_B "COLORB" // subscribed om te ontvangen op de arduino
#define TOPIC_SPEED  "SPEED"  // subscribed om te ontvangen op de arduino
#define TOPIC_EFFECT "EFFECT" // subscribed om te ontvangen op de arduino
#define TOPIC_STRENGTH "STRENGTH" // subscribed om te ontvangen op de arduino
class network
{
  private:
    int count;
    unsigned long message_interval;
    unsigned long previousMessageMillis;

    unsigned long keepalive_interval;
    unsigned long keepalive_Millis;

    taken _taken;


    void on_message(int messageSize);
    static void sub_scribe_to_topic(String topic);
    static void debug_incoming_message(int messageSize);
    static void connect_to_wifi();
    void connect_to_mqtt();

  public:
    network();
    void start(taken taken);
    void loop();
    static void send_debug(String message);   
};

#endif
