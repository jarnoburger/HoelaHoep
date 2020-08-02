#include <Arduino.h>
#include "Led.h"
//#include <Adafruit_NeoPixel.h>

Led::Led(int pin){
  _ledState = LOW;             // ledState used to set the LED
  _previousMillis = 0;        // will store last time LED was updated
  _interval = 1000;           // interval at which to blink (milliseconds)

  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Led::Loop()
{
  // here is where you'd put code that needs to be running all the time.
   // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();
 
  if(currentMillis - _previousMillis > _interval) {
    // save the last time you blinked the LED 
    _previousMillis = currentMillis;   
 
    // if the LED is off turn it on and vice-versa:
    if (_ledState == LOW)
      _ledState = HIGH;
    else
      _ledState = LOW;
 
    // set the LED with the ledState of the variable:
    digitalWrite(_pin, _ledState);
  }
}
