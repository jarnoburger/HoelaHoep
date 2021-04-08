#include <Arduino.h>
#include "arduino_secrets.h"
#include "Led.h"
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

// how many leds ?
#define NUM_LEDS 487

// init lib
Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 5 // Time (in milliseconds) to pause between pixels

bool direction;
int c;
bool _ledState; // knippert om aan te geven dat hij leeft
byte colorARed   = 255;
byte colorAGreen = 0;
byte colorABlue  = 0;
byte colorBRed   = 0;
byte colorBGreen = 255;
byte colorBBlue  = 0;
int frameRate = 25;
int _effect = 4 ;
unsigned long _animationinterval = 5000; // the animation must fit in 2000 miliseconds
unsigned long _interval;

Led::Led(){
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  _ledState = LOW;             // ledState used to set the Internal LED
  _previousFrameRateMillis = 0;        // will store last time LED was updated
  _interval = 1000/frameRate;           // interval at which to blink (milliseconds) , so 1 sec / 50 frames = 20 miliseconds
}

void Led::Start()
{
  Serial.print("Neopixel pin : ");
  Serial.println(LED_BUILTIN);
  pinMode(PIN, OUTPUT);

  Serial.print("LED_BUILTIN pin : ");
  Serial.println(LED_BUILTIN);
  pinMode(LED_BUILTIN, OUTPUT);

  setAll(0, 128, 255);
}

void Led::Loop()
{
  unsigned long currentMillis = millis();

  // ********************************
  // calculate where we are in the animation
  float animPassed = (float)(currentMillis % _animationinterval) / _animationinterval; // get playhead percentage

  // ********************************
  // frame rate
  if( currentMillis - _previousFrameRateMillis > _interval) {
    // enought time passed , to be futher then current interval  
    // so save the last time you blinked the LED 
    _previousFrameRateMillis = currentMillis;   
    
    // set the LED with the ledState of the variable:
    //digitalWrite(LED_BUILTIN, _ledState);
    //_effect = 1;
    //Serial.println(_effect);

    if (_effect == 0){ DoFull(); }
    if (_effect == 1){ DoFade(animPassed); }
    if (_effect == 2){ Strobe(animPassed); }
    if (_effect == 3){ SnowSparkle(animPassed); }
    if (_effect == 4){ Sparkle(); }
    if (_effect == 5){ LeftToRight(animPassed); }
    if (_effect == 6){ RightToLeft(animPassed); }
    //delay(1000);
  }
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

// *******************************

void Led::SetColorA(byte red, byte green, byte blue){
  Serial.print("Color A becomes ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);

  colorARed = red;
  colorAGreen = green;
  colorABlue = blue;
}

void Led::SetColorB(byte red, byte green, byte blue){
  Serial.print("Color B becomes ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);

  colorBRed = red;
  colorBGreen = green;
  colorBGreen = blue;
}

void Led::SetEffect(int effect){
  setAll(0,0,0);
  _effect = effect;
  Serial.print("Led goes to effect ");
  Serial.println(_effect);
}

void Led::SetSpeed(int speed){}

// *******************************

void Led::DoFull(){
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, colorARed, colorAGreen, colorABlue);
  }
  showStrip();
}

void Led::DoFade(float animPassed){
  float l = (animPassed - 0.5);
  if (l < 0) l = -l;
  l = l *2;
  //Serial.println(l);

  int r = lerp(colorARed  , colorBRed  , l);
  int g = lerp(colorAGreen, colorBGreen, l);
  int b = lerp(colorABlue , colorBBlue , l);

  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, r, g, b);
  }
  showStrip();
}

void Led::Strobe(float animPassed){
  int x = animPassed  * 100;
  x = x % 10;
  if (x < 5){
    setAll(colorARed, colorAGreen, colorABlue);
  }
  else {
    setAll(0,0,0);
  }
}

void Led::SnowSparkle(float animPassed) {  
  int Pixel = random(NUM_LEDS);

  if (animPassed < 0.1f)
  {
    setAll(colorBRed,colorBGreen,colorBBlue);
  }
  else if (animPassed >= 0.1f && animPassed <= 0.2f)
  {
    setPixel(Pixel,colorARed,colorAGreen,colorABlue);
    showStrip();
  }
  if (animPassed > 0.2f){
    setPixel(Pixel,colorBRed,colorBGreen,colorBBlue);
    showStrip();
  }
}

void Led::Sparkle() {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,colorARed,colorAGreen,colorABlue);
  showStrip();
  setPixel(Pixel,0,0,0);
}

void Led::LeftToRight(float animPassed) {
  int eye = 3;
  int cursor = (float)NUM_LEDS * animPassed;

  setAll(0,0,0);

  // set minus side
  for(int i = cursor - eye; i < cursor; i++ ){
    int p = i;
    if (p <= 0){ p = p + NUM_LEDS; }
    Serial.print(p);
    Serial.print("-");
    setPixel(p, colorARed/10, colorAGreen/10, colorABlue/10);
  }

  Serial.print("<");

  // set center
  Serial.print(cursor);
  setPixel(cursor, colorARed, colorARed, colorARed);
  
  Serial.print(">");

  // set maxus side
  for(int i = cursor + 1; i <= cursor + eye; i++ ){
    int p = i;
    if (p >= NUM_LEDS){ p = p - NUM_LEDS; }
    Serial.print(p);
    Serial.print("-");
    setPixel(p, colorARed/10, colorAGreen/10, colorABlue/10);
  }

  Serial.println();

  showStrip();
}

void Led::RightToLeft(float animPassed){
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

int Led::lerp(byte a, byte b, float x)
{
 return a + x * (b - a);
}