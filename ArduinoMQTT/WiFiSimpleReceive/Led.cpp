#include <Arduino.h>
#include "arduino_secrets.h"
#include "Led.h"
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

// how many leds ?
#define NUM_LEDS 10

// init lib
Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

bool direction;
int c;
bool _ledState;
unsigned long _previousMillis;
unsigned long _interval;

Led::Led(){
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  _ledState = LOW;             // ledState used to set the Internal LED
  _previousMillis = 0;        // will store last time LED was updated
  _interval = 5000;           // interval at which to blink (milliseconds)
}

void Led::Start()
{
  pinMode(PIN, OUTPUT);

  // populeer hier een loopup tabel als we dat nodig hebben

  //pixels = new Adafruit_NeoPixel(_numpixels, _pin, _pixelFormat);
  
   // Going forward from here, code works almost identically to any other
  // NeoPixel example, but instead of the dot operator on function calls
  // (e.g. pixels.begin()), we instead use pointer indirection (->) like so:
  //pixels->begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
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

void Led::showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void Led::setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void Led::setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void Led::LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void Led::RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void Led::Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void Led::fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
   
    oldColor = pixels.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
   
    pixels.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void Led::meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      }
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void Led::Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
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

  byte red         = 255;
  byte green       =   0;
  byte blue        =   0;
  int SparkleDelay = 100;
  int SpeedDelay   = 100;
  int ReturnDelay  = 100;
  int EyeSize      =   3;
  byte meteorSize  =   5;
  byte meteorTrailDecay = 10;
  bool meteorRandomDecay = true;
  int StrobeCount = 10;
  int FlashDelay = 50;
  int EndPause = 100;

  if (_effect == 0){
    //DoFull();
  }
  else if (_effect == 1){
    //DoFade();
  }
  else if (_effect == 2){
    SnowSparkle(red, green, blue, SparkleDelay, SpeedDelay);
  }
  else if (_effect == 3){
    //LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  }
  else if (_effect == 4){
    //RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  }
  else if (_effect == 5){
    //Sparkle(red, green, blue, SpeedDelay);
  }
  else if (_effect == 6){
    //meteorRain(red, green, blue, meteorSize, meteorTrailDecay,  meteorRandomDecay, SpeedDelay);
  }
  else if (_effect == 7){
    //Strobe(red, green, blue, StrobeCount, FlashDelay, EndPause);
  }
  else if (_effect == 8){
    
  }
}

void Led::SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {  
  setAll(red,green,blue);
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
}
