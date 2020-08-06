#include <Arduino.h>
#include "Taken.h"
#include "Led.h"
#include <ArduinoJson.h>
#include "Network.h"

// effects
// 1 : alles aan
// 2 : fade tussen 2 kleuren (daardoor ook strobe)
// 3 : roteer een streep met kleur A , de anderen zijn kleur B
// 4 : wave een streep met kleur A , de anderen zijn kleur B
// 5 : random met kleur A over kleur B
// 6 : color wipe

StaticJsonDocument<200> doc; // buffer om de JSON tree in te bewaren
char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";  // example string

Taken::Taken(){

}

void Taken::Start(Led led){
  _led = led;
}

void Taken::Parse(String topic, char message[]){
  if      (topic.equals(topiccolorA)){
    SetColorA(message);
  }
  else if (topic.equals(topiccolorB)){
    SetColorB(message);
  }
  else if (topic.equals(topicEffect)){
    SetEffect(message);
  }
  else if (topic.equals(topicSpeed)) {
    SetSpeed (message);
  }
  else{
    Serial.print("No handler for topic ");
    Serial.println(topic);
  }
 
  Serial.println();
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void Taken::SetColorA(char bericht[]){
  byte red = 0;
  byte green = 0;
  byte blue = 0;
  byte power = 0;

  Serial.print("Setting power A ");
  Serial.println(power);
  power = constrain(power, 0, 1000);
    Serial.print("Setting color A ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
  red   = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  blue  = constrain(blue, 0, 255);
  _led.SetColorA(red,green,blue);

  //_led.SetPowerA(power);
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void Taken::SetColorB(char bericht[]){
  byte red = 0;
  byte green = 0;
  byte blue = 0;
  byte power = 0;

  Serial.print("Setting color B ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.println(blue);
  red   = constrain(red, 0, 255);
  green = constrain(green, 0, 255);
  blue  = constrain(blue, 0, 255);
  _led.SetColorB(red,green,blue);
  Serial.print("Setting power B ");
  Serial.println(power);
  power = constrain(power, 0, 1000);
  //_led.SetPowerB(power);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetEffect(char bericht[]){
  int effect = 0;

  Serial.print("Setting effect ");
  Serial.println(effect);
  effect = constrain(effect, 0, 0);
  //_led.SetEffect(effect);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetSpeed(char bericht[]){
  int effect = 0;

  Serial.print("Setting effect ");
  Serial.println(effect);
  effect = constrain(effect, 0, 0);
  //_led.SetEffect(effect);
}

void Taken::Test(){
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  const char* sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}