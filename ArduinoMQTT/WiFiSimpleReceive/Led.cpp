#include <Arduino.h>
#include "Led.h"
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5

// how many leds ?
constexpr auto NUM_LEDS = 487;

// init lib
Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

bool direction;
int c;
bool led_state; // blinks to tell if we are still alive
byte color_a_red   = 255;
byte color_a_green = 0;
byte color_a_blue  = 0;
byte color_b_red   = 0;
byte color_b_green = 255;
byte color_b_blue  = 0;
int frame_rate = 60;
int effect = 0 ;
int strength = 7;
float speed = 0.1f;
unsigned long animation_interval = 5000; // the animation must fit in 2000 milliseconds
unsigned long frames_per_second;
long previous_frame_rate_millis_;
bool first_time = true;
int noise_spot[10];
int noise_count = 10;

led::led(){
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  previous_frame_rate_millis_ = 0;        // will store last time LED was updated
  frames_per_second = 1000/frame_rate;           // interval at which to blink (milliseconds) , so 1 sec / 50 frames = 20 miliseconds
}

void led::start()
{
  Serial.print("Neopixel pin : ");
  Serial.println(LED_BUILTIN);
  pinMode(PIN, OUTPUT);

  Serial.print("LED_BUILTIN pin : ");
  Serial.println(LED_BUILTIN);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("Number of leds : ");
  Serial.println(NUM_LEDS);

  Serial.println("One pixel requires 30 microseconds. Afterwards it needs to weed 50 microseconds");
  Serial.print("A update takes ");
  int microsec =(NUM_LEDS * 30) + 50; 
  Serial.print(microsec);
  Serial.print(" microseconds , ");
  Serial.print(microsec * 1000000);
  Serial.println(" seconds");
  Serial.print("Theoretical max update speed is ");
  int maxfps = 1000000 / microsec;
  Serial.println(maxfps);

  for (int i = 0; i <255; i++)
  {
      set_all(0, i/2, i);
  }
}

void led::loop()
{
  if (first_time)
  {
    for (int i = 255; i >0; i = i - 1)
    {
      int g = i /2;
      set_all(0,g,i);
    }
    first_time = false;
  }

  unsigned long currentMillis = millis();

  // ********************************
  // calculate where we are in the animation
  float animPassed = (float)(currentMillis % animation_interval) / animation_interval; // get playhead percentage

  // ********************************
  // frame rate
  if( currentMillis - previous_frame_rate_millis_ > frames_per_second) {
    // enought time passed , to be futher then current interval  
    // so save the last time you blinked the LED 
    previous_frame_rate_millis_ = currentMillis;   
    
    // set the LED with the ledState of the variable:
    //digitalWrite(LED_BUILTIN, _ledState);
    //_effect = 1;
    //Serial.println(_effect);

    if (effect == 0){ do_full(); }
    if (effect == 1){ do_fade(animPassed); }
    if (effect == 2){ strobe(animPassed); }
    if (effect == 3){ snow_sparkle(animPassed); }
    if (effect == 4){ sparkle(); }
    if (effect == 5){ left_to_right(animPassed); }
    if (effect == 6){ fairydust(animPassed); }
    if (effect == 7){ Noise(animPassed);}
    //delay(1000);
  }
}

void led::show_strip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void led::set_pixel(int Pixel, byte red, byte green, byte blue) {
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

void led::set_all(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    set_pixel(i, red, green, blue);
  }
  show_strip();
}

// *******************************

void led::set_color_a(byte red, byte green, byte blue){
  Serial.print("Color A becomes ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);

  color_a_red = red;
  color_a_green = green;
  color_a_blue = blue;
}

void led::set_color_b(byte red, byte green, byte blue){
  Serial.print("Color B becomes ");
  Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);

  color_b_red = red;
  color_b_green = green;
  color_b_blue = blue;
}

void led::set_effect(int new_effect){
  set_all(0,0,0);
  effect = new_effect;
  Serial.print("Led goes to effect ");
  Serial.println(effect);
}

void led::set_speed(float new_speed)
{
  speed = new_speed;
}

void led::set_strength(int new_strength)
{
  strength = new_strength;
}
// *******************************

// effect 0
void led::do_full(){
  for(int i = 0; i < NUM_LEDS; i++ ) {
    set_pixel(i, color_a_red, color_a_green, color_a_blue);
  }
  show_strip();
}

// effect 1
void led::do_fade(float anim_passed){
  float l = (anim_passed - 0.5);
  if (l < 0) l = -l;
  l = l *2;
  //Serial.println(l);

  int r = lerp(color_a_red  , color_b_red  , l);
  int g = lerp(color_a_green, color_b_green, l);
  int b = lerp(color_a_blue , color_b_blue , l);

  for(int i = 0; i < NUM_LEDS; i++ ) {
    set_pixel(i, r, g, b);
  }
  show_strip();
}

// effect 2
void led::strobe(float anim_passed){
  int x = (speed * 100) * anim_passed;
  x = x % 10;
  if (x < 5){
    set_all(color_a_red, color_a_green, color_a_blue);
  }
  else {
    set_all(0,0,0);
  }
}

// effect 3
void led::snow_sparkle(float anim_passed) {  
  int leds = strength*10;
  int pixel[leds];
  for (int i =0; i < leds;i++)
  {
     pixel[i] = random(NUM_LEDS);
  }

  // turn on
    for (int i = 0 ; i < leds; i ++)
    {
      int p = pixel[i];
      set_pixel(p,color_a_red,color_a_green,color_a_blue);
    }

    // show
    show_strip();

    // turn off
    for (int i = 0 ; i < leds; i ++)
    {
      set_pixel(pixel[i],color_b_red,color_b_green,color_b_blue);
    }
}

//effect 4
void led::sparkle() {
  int pixel = random(NUM_LEDS);
  
  int begin = pixel - strength * 5;
  int end = pixel + strength * 5;
  constrain(pixel, 0, NUM_LEDS);

  // turn on
  for (int i = begin ; i < end; i ++)
  {
    set_pixel(i,color_a_red,color_a_green,color_a_blue);
  }

  // show
  show_strip();

  // turn off
  for (int i = begin ; i < end; i ++)
  {
    set_pixel(i,color_b_red,color_b_green,color_b_blue);
  }
}

//effect 5
void led::left_to_right(float anim_passed) {
  double eye = strength * 4;
  int sp = (speed * 40) - 20;
  anim_passed = fmod((anim_passed * sp),1);
  int cursor = (float)NUM_LEDS * anim_passed;

  // put your main code here, to run repeatedly:
  double s = 1.0f/(eye +1.0);
  double t = 1 -s;

  int r;
  int g;
  int b;
  int c;

  // begin
  for (int i = 0; i <= eye;i++)
  {
    //r = t * color_a_red;
    //g = t * color_a_green;
    //b = t * color_a_blue;

    r = lerp(color_a_red  , color_b_red  , t);
    g = lerp(color_a_green, color_b_green, t);
    b = lerp(color_a_blue , color_b_blue , t);

    c = cursor + i;
    c = fitpixel(c);
    set_pixel(c,r,g,b);

    c = cursor - i;
    c = fitpixel(c);
    set_pixel(c,r,g,b);

    t = t - s;
  }
    
  // mid
  set_pixel(cursor, color_a_red,color_a_green,color_a_blue);

  // show it
  show_strip();

  // begin
  for (int i = 0; i <= eye;i++)
  {
    c = cursor + i;
    c = fitpixel(c);
    set_pixel(c,color_b_red,color_b_green,color_b_blue);

    c = cursor - i;
    c = fitpixel(c);
    set_pixel(c,color_b_red,color_b_green,color_b_blue);
  }
    
  // top
  set_pixel(cursor, color_b_red,color_b_green,color_b_blue);
}

//effect 6
void led::fairydust(float anim_passed){
  int pixel = random(NUM_LEDS);
  int begin = pixel - strength *3;
  int end = pixel + strength * 3;
  constrain(pixel, 0, NUM_LEDS);

  // turn on
  for (int i = begin ; i < end; i ++)
  {
    set_pixel(i,color_a_red,color_a_green,color_a_blue);
  }

  // show
  show_strip();

  // turn off
  for (int i = 0 ; i < NUM_LEDS; i ++)
  {
    fade_to_black(i,50);
    //set_pixel(i,color_b_red,color_b_green,color_b_blue);
  }
}

//effect 7
void led::Noise(float anim_passed)
{
  if (noise_count != strength)
  {
    strength += 1;
    noise_count = strength;
    noise_spot[strength];
    for (int i = 0;i<noise_count;i++)
    {
      noise_spot[i] = random(0, NUM_LEDS);
    }
  }

   int delta = 6;

  for (int i=0; i<noise_count;i++)
  {
    // make a random value between -delta and delta
   
    int a  =  noise_spot[i]  + random(0,delta*2) - delta;
    if (a < 0) { a = NUM_LEDS -1;}
    if (a > NUM_LEDS) { a = 0;}
    noise_spot[i] = a;
    set_pixel(noise_spot[i], 255,255,255);
  }

  // show
  show_strip();

  // this will happen in the next round : (turning the shown pixels off)
  for (int i=0; i<noise_count;i++)
  {
    set_pixel(noise_spot[i], 00,0,0);
  }
}

void led::fade_to_black(int led_no, byte fade_value) {
 #ifdef ADAFRUIT_NEOPIXEL_H
    uint32_t oldColor = pixels.getPixelColor(led_no);
    uint8_t r = (oldColor & 0x00ff0000UL) >> 16;
    uint8_t g = (oldColor & 0x0000ff00UL) >> 8;
    uint8_t b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : static_cast<int>(r)-(r*fade_value/256);
    g=(g<=10)? 0 : static_cast<int>(g)-(g*fade_value/256);
    b=(b<=10)? 0 : static_cast<int>(b)-(b*fade_value/256);
   
    pixels.setPixelColor(led_no, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}

void led::meteor_rain(byte red, byte green, byte blue, byte meteor_size, byte meteor_trail_decay, boolean meteorRandomDecay, int SpeedDelay) {  
  set_all(0,0,0);
 
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fade_to_black(j, meteor_trail_decay );        
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteor_size; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        set_pixel(i-j, red, green, blue);
      }
    }
   
    show_strip();
    delay(SpeedDelay);
  }
}

int led::lerp(uint8_t a, uint8_t b, float x)
{
 return a + x * (b - a);
}

int led::fitpixel(int c)
{
    if (c<0) { c = c+ NUM_LEDS;}
    else if (c>NUM_LEDS) { c = c- NUM_LEDS;}
    return c;
}