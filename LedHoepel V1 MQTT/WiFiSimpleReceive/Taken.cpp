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

taken::taken()
= default;

void taken::start(led led)
{
  _led = led;
}

void taken::parse(String topic, char message[]){
  if      (topic.equals(TOPIC_COLOR_A)){
    set_color_a(message);
  }
  else if (topic.equals(TOPIC_COLOR_B)){
    set_color_b(message);
  }
  else if (topic.equals(TOPIC_EFFECT)){
    set_effect(message);
  }
  else if (topic.equals(TOPIC_SPEED)) {
    set_speed (message);
  }
  else if (topic.equals(TOPIC_STRENGTH)) {
    set_strength (message);
  }
  else{
    Serial.print("No taak for topic ");
    Serial.println(topic);
  }
 
  Serial.println();
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void taken::set_color_a(char bericht[]){
  Serial.println("Parsing info for color A ");
  Serial.println(bericht);

  byte red   = hex_color_to_int(bericht[1], bericht[2]);
  byte green = hex_color_to_int(bericht[3], bericht[4]);
  byte blue  = hex_color_to_int(bericht[5], bericht[6]);
  
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);

  _led.set_color_a(red,green,blue);
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void taken::set_color_b(char bericht[]){
  Serial.println("Parsing info for color B ");
  Serial.println(bericht);

  byte red   = hex_color_to_int(bericht[1], bericht[2]);
  byte green = hex_color_to_int(bericht[3], bericht[4]);
  byte blue  = hex_color_to_int(bericht[5], bericht[6]);
  
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);

  _led.set_color_b(red,green,blue);
}

// tussen 0 en het aantal effecten minus 1
void taken::set_effect(char bericht[]){
  int effect = atoi(bericht);

  Serial.print("Setting effect ");
  Serial.println(effect);
  _led.set_effect(effect);
}

// tussen 0 en het aantal effecten minus 1
void taken::set_speed(char bericht[]){
  float speed = atoi(bericht);

  speed = constrain(speed, 0, 100)/100;

  Serial.print("Setting speed ");
  Serial.println(speed);
  _led.set_speed(speed);
}

void taken::set_strength(char bericht[])
{
  int strength = atoi(bericht);
  Serial.print("Setting strength");
  Serial.println(strength);
  _led.set_strength(strength);
}


void taken::show_error(){

}

byte taken::hex_color_to_int(char upper, char lower)
{
  byte uVal = (byte)upper;
  byte lVal = (byte)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  //  Serial.println(uVal+lVal);
  return uVal + lVal;
}

void taken::test(){
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