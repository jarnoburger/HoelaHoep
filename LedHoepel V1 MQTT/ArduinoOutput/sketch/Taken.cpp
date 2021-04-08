#include <Arduino.h>
#include "Taken.h"
#include "Led.h"
#include "Network.h"

// effects
// 1 : alles aan
// 2 : fade tussen 2 kleuren (daardoor ook strobe)
// 3 : roteer een streep met kleur A , de anderen zijn kleur B
// 4 : wave een streep met kleur A , de anderen zijn kleur B
// 5 : random met kleur A over kleur B
// 6 : color wipe

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
    Serial.print("No taak for topic ");
    Serial.println(topic);
  }
 
  Serial.println();
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void Taken::SetColorA(char bericht[]){
  Serial.println("Parsing info for color A ");
  Serial.println(bericht);

  byte red   = HexcolorToInt(bericht[1], bericht[2]);
  byte green = HexcolorToInt(bericht[3], bericht[4]);
  byte blue  = HexcolorToInt(bericht[5], bericht[6]);
  
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);

  _led.SetColorA(red,green,blue);
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void Taken::SetColorB(char bericht[]){
  Serial.println("Parsing info for color B ");
  Serial.println(bericht);

  byte red   = HexcolorToInt(bericht[1], bericht[2]);
  byte green = HexcolorToInt(bericht[3], bericht[4]);
  byte blue  = HexcolorToInt(bericht[5], bericht[6]);
  
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);

  _led.SetColorB(red,green,blue);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetEffect(char bericht[]){
  int effect = atoi(bericht);

  Serial.print("Setting effect ");
  Serial.println(effect);
  _led.SetEffect(effect);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetSpeed(char bericht[]){
  int effect = 0;

  Serial.print("Setting effect ");
  Serial.println(effect);
  effect = constrain(effect, 0, 0);
  //_led.SetEffect(effect);
}

void Taken::ShowError(){

}

byte Taken::HexcolorToInt(char upper, char lower)
{
  byte uVal = (byte)upper;
  byte lVal = (byte)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  //  Serial.println(uVal+lVal);
  return uVal + lVal;
}

void Taken::Test(){
  // Deserialize the JSON document
  //DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  //if (error) {
  //  Serial.print(F("deserializeJson() failed: "));
  //  Serial.println(error.c_str());
  //  return;
  //}

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  //const char* sensor = doc["sensor"];
  //long time = doc["time"];
  //double latitude = doc["data"][0];
  //double longitude = doc["data"][1];

  // Print values.
  //Serial.println(sensor);
  //Serial.println(time);
  //Serial.println(latitude, 6);
  //Serial.println(longitude, 6);
}