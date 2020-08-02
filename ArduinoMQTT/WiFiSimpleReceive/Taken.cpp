#include <Arduino.h>
#include "Taken.h"
#include "Led.h"

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

// tussen 0 en 100
void Taken::SetSpeed(int s) {
  Serial.print("Setting speed ");
  Serial.println(s);
  s = constrain(s, -100, 100);
  _led.SetSpeed(s);
}

// tussen 0 en 100
void Taken::SetEffectIntensity(int intensity){
  Serial.print("Setting intensity ");
  Serial.println(intensity);
  intensity = constrain(intensity, 0, 100);
  _led.SetIntensity(intensity);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetEffect(int effect){
  Serial.print("Setting effect ");
  Serial.println(effect);
  effect = constrain(effect, 0, 0);
  _led.SetEffect(effect);
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void Taken::SetPowerA(int power){
  Serial.print("Setting power A ");
  Serial.println(power);
  power = constrain(power, 0, 1000);
  _led.SetPowerA(power);
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void Taken::SetPowerB(int power){
  Serial.print("Setting power B ");
  Serial.println(power);
  power = constrain(power, 0, 1000);
  _led.SetPowerB(power);
}

// tussen 0 en 255 , kleur A
void Taken::SetColorA(int red, int green, int blue){
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
}

// tussen 0 en 255 , kleur B
void Taken::SetColorB(int red, int green, int blue){
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
}
