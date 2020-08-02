#include <Arduino.h>
#include "arduino_secrets.h"
#include "Led.h"
#include <Adafruit_NeoPixel.h>

// Rather than declaring the whole NeoPixel object here, we just create
// a pointer for one, which we'll then allocate later...
Adafruit_NeoPixel *pixels;

Led::Led(){
  _pixelFormat = NEO_GRB + NEO_KHZ800;
  _ledState = LOW;             // ledState used to set the LED
  _previousMillis = 0;        // will store last time LED was updated
  _interval = 5000;           // interval at which to blink (milliseconds)
}

void Led::Start(int pin, int numpixels)
{
  _pin = pin;
  _numpixels = numpixels;

  pinMode(pin, OUTPUT);

  // populeer hier een loopup tabel als we dat nodig hebben

  pixels = new Adafruit_NeoPixel(_numpixels, _pin, _pixelFormat);
  
   // Going forward from here, code works almost identically to any other
  // NeoPixel example, but instead of the dot operator on function calls
  // (e.g. pixels.begin()), we instead use pointer indirection (->) like so:
  pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
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
     {
      _ledState = HIGH;
      Pow();
     }
    else
     { 
      _ledState = LOW;
     }
     
    // set the LED with the ledState of the variable:
    digitalWrite(_pin, _ledState);

    Pow();
  }
}

void Led::SetEffect(int effect)
{
  _effect = effect;
}

void Led::Pow()
{
  // effects
  // 1 : alles aan
  // 2 : fade tussen 2 kleuren (daardoor ook strobe)
  // 3 : roteer een streep met kleur A , de anderen zijn kleur B
  // 4 : wave een streep met kleur A , de anderen zijn kleur B
  // 5 : random met kleur A over kleur B
  // 6 : color wipe

  if (_effect == 0)
  {
    DoFull();
  }
  else if (_effect == 1)
  {
    DoFade();
  }
  else if (_effect == 2)
  {
    
  }
  else if (_effect == 3)
  {
    
  }
  else if (_effect == 4)
  {
    
  }
  else if (_effect == 5)
  {
    
  }
  else if (_effect == 6)
  {
    
  }
}

void Led::DoFull()
{
    // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<_numpixels; i++) 
  {
    // For each pixel...
    pixels->setPixelColor(i, pixels->Color(_colorARed, _colorAGreen, _colorABlue));
    pixels->show();   // Send the updated pixel colors to the hardware.
  }
}

void Led::DoFade()
{
  // goto color A
  for(int i=0; i<_numpixels; i++) 
  {
    pixels->setPixelColor(i, pixels->Color(_colorARed, _colorAGreen, _colorABlue));
    pixels->show();   // Send the updated pixel colors to the hardware.
  }

  // goto color B
  for(int i=0; i<_numpixels; i++) 
  {
    pixels->setPixelColor(i, pixels->Color(_colorBRed, _colorBGreen, _colorBBlue));
    pixels->show();   // Send the updated pixel colors to the hardware.
  }
}
