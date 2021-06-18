#include <Arduino.h>
//#include <FastLED.h>
//#include "FastLED_RGBW.h"
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

#include <SimpleTimer.h>
#include <SPI.h>

#include "Extras.h"
#include "JArtnet.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/*
Receive multiple universes via Artnet and control a strip of ws2811 leds via OctoWS2811
This example may be copied under the terms of the MIT license, see the LICENSE file for details
https://github.com/natcl/Artnet
http://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811?p=55589&viewfull=1#post55589
This example will receive multiple universes via Art-Net and control a strip of
WS2812 LEDs via the FastLED library: https://github.com/FastLED/FastLED
This example may be copied under the terms of the MIT license, see the LICENSE file for details
Ideas for improving performance with WIZ820io / WIZ850io Ethernet:
https://forum.pjrc.com/threads/45760-E1-31-sACN-Ethernet-DMX-Performance-help-6-Universe-Limit-improvements
installeer uit de libraries folder op disk de octows2811 library en deinstalleer de pre installed lirbary !
a single dmx512 frame of 512 channels is referred to as a universe
dus 512 channels per universe
de leds kunnen rgbw
w gaat aan wanneer er puur wit verstuurd wordt
Possible causes:

The .show() command is only in the DMX receive which also write pixel values to the buffer, so it pretty much has to be happening there.

Resolume is sending out some kind of heartbeat packet that is incorrectly getting picked up as a DMX packet by the Artnet library

There is an interrupt conflict between the FastLED and UDP inbound processing, if FastLED starts a pixel write but gets interrupted to handle some other task it will be an early protocol reset.

UDP over Wifi, especially broadcast, is atrocious and undependable. Possible to have malformed or late packets coming in.

You might try re-enabling either the interrrupt retry or interrupts in general before the FastLED include, but that might just make the flickering worse.
*/

// Network settings
byte mac[]       = {  0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
byte ip[]        = { 192, 168,   0, 130 };
byte dns[]       = { 192, 168,   0,   1 };
byte gateway[]   = { 192, 168,   0,   1 };
byte subnet[]    = { 255, 255, 255,   0 };
byte broadcast[] = { 192, 168,   0, 255 };
SimpleTimer timerNetwork;

// LED settings
const int numberOfPixels = 12*34; // change for your setup

#define DATA_PIN 2
#define FPS 25

// LED MEMORY
//CRGB leds[numberOfPixels];
//CRGBW leds[numberOfPixels];
//CRGB *ledsRGB = (CRGB *) &leds[0];
Adafruit_NeoPixel pixels(numberOfPixels, DATA_PIN, NEO_GRBW + NEO_KHZ800);
SimpleTimer timerShowPixels;

// Artnet settings
JArtnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = numberOfPixels * 4; // Total number of channels you want to receive (1 led = 4 channels)
//const int numberOfUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
const int numberOfUniverses = 4;
int channelBuffer[numberOfChannels]; // Combined universes into a single array

// Check if we got all universes
bool universesReceived[numberOfUniverses];
bool channelBufferIsFull = false;
bool showLeds = false;
int previousDataLength = 0;

// debug
bool demoMode = false;
bool debugArtnet = false;
bool debugUdp = false;

// Demo mode variables
uint8_t thisdelay = 40;                                       // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
int8_t thisrot = 1;                                           // Hue rotation speed. Includes direction.
uint8_t deltahue = 1;                                         // Hue change between pixels.
bool thisdir = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.

int frames = 0;
unsigned long lastCompleteFrameTime;

// declare functies
void OnDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
void WaitForDemoMode();
void DoDemoMode();

void InitTest();

boolean ConnectNetwork();
void DoNetworkCheck();
void rainbow_march();
void DoNetworkCheck();
void DoShowPixels();

void setup()
{
  //Udp.begin(6454);  // alleen aanzetten als artnet op een andere poort moet

  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // start de check of we in demo mode verder gaan
  WaitForDemoMode();

  // do leds
  Serial.println(getRGBWsize(numberOfPixels));
  //FastLED.addLeds<WS2812B, DATA_PIN>(leds, numberOfPixels);
  //FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(numberOfPixels));
  //FastLED.show();
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.println("Started FastLed");

  // do debug
  Serial.begin(115200);
  Serial.println("Welcome");
  Serial.println();
  Serial.print("Demo mode is : ");
  Serial.println(demoMode);
  Serial.print("Debug ArtNet is : ");
  Serial.println(debugArtnet);
  Serial.print("Debug Udp is : ");
  Serial.println(debugUdp);

  // do network
  ConnectNetwork();
  timerNetwork.setInterval(30000, DoNetworkCheck);

  // do artnet
  artnet.begin();
  artnet.setBroadcast(broadcast);

  Serial.println("Started Artnet");
  Serial.print("Number of pixels : ");
  Serial.println(numberOfPixels);
  Serial.print("Number of channels : ");
  Serial.println(numberOfChannels);
  Serial.print("Number of universes : ");
  Serial.println(numberOfUniverses);
  Serial.print("Start universe : ");
  Serial.println(startUniverse);

  // led blinker
  digitalWrite(LED_BUILTIN, LOW);

  artnet.setArtDmxCallback(OnDmxFrame);
  Serial.println("Artnet ready to recieve");

  timerShowPixels.setInterval(1000/50, DoShowPixels);
}

// wait 3 seconds to see if demo mode gets activated
void WaitForDemoMode(){
  pinMode(4, INPUT);  // zet de pin klaar om te luisteren
  
  int d = 100; // time between on/off or off/on
  
  for (int i = 0; i < 3000; i=i+d){

    // do blinkie op de achterkant
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d/2);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d/2);

    if (digitalRead(4) == true){
      demoMode = true;
    }
  }
}

void loop()
{
  if (debugUdp){
    //DebugUdp(Udp);
  }

  // when we do demo mode , do it and return
  if (demoMode){
    DoDemoMode();
    return;
  }

  timerNetwork.run();
  timerShowPixels.run();

  // we call the read function inside the loop
  //artnet.read();
  if (artnet.read() == ART_DMX)
  { 
    // set the blink led to high , because there was a packet
    digitalWrite(LED_BUILTIN, HIGH);

    // we call the read function inside the loop
    //artnet.read();
  }

  // set the blink led to low , because there was no packet
  digitalWrite(LED_BUILTIN, LOW);
  
  // if all the universe were filed , we can show the leds.
  if (channelBufferIsFull)
  {
    showLeds = true;
  }
}

void DoDemoMode(){
  for (int i = 0 ; i < numberOfPixels ; i++) {
    //Serial.println("i");
    pixels.setPixelColor(i, pixels.Color(255, 255, 255,255));
    //leds[i] = CRGBW(0, 0, 0, 0);
    //leds[i] = CRGB(0, 255, 0);
  }
  //FastLED.show();
  pixels.show();
  delay(30);

  for (int j = 0 ; j < numberOfPixels ; j++) {
    //Serial.println("j");
    pixels.setPixelColor(j, pixels.Color(0, 0, 0,0));
    //leds[j] = CRGBW(255, 255, 255, 255);
    //leds[i] = CRGB(0, 0, 0);
  }
  //FastLED.show();
  pixels.show();
  delay(30);
  
/*
    // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 60;                // Change '60' to a different value to change length of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: thisrot=1; deltahue=5; break;
      case  5: thisdir=-1; deltahue=10; break;
      case 10: thisrot=5; break;
      case 15: thisrot=5; thisdir=-1; deltahue=20; break;
      case 20: deltahue=30; break;
      case 25: deltahue=2; thisrot=5; break;
      case 30: break;
    }
  }

  //EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbow_march();
  //}

  FastLED.show();
  */
} // part of DoDemoMode()

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.
  
  if (thisdir == 0) thishue += thisrot; else thishue-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.

  //CHSV hsv;
  //hsv.hue = thishue;
  //hsv.val = 255;
  //hsv.sat = 240;
  //for( int i = 0; i < numberOfPixels; ++i) {
  //    leds[i] = hsv;
  //    hsv.hue += deltahue;
  //}
} // rainbow_march()

void DebugArtNet(){
    // print out our data
    Serial.print("universe number = ");
    Serial.print(artnet.getUniverse());
    Serial.print("\tdata length = ");
    Serial.print(artnet.getLength());
    Serial.print("\tsequence n0. = ");
    Serial.println(artnet.getSequence());
    Serial.print("DMX data: ");
    for (int i = 0 ; i < artnet.getLength() ; i++)
    {
      Serial.print(artnet.getDmxFrame()[i]);
      Serial.print("  ");
    }
    Serial.println();
    Serial.println();
}

// universe = universe
// length = lengte van data
// sequence = voor de orde van supergrote pakketten die in dele komen
// data = pointer naar de data
void OnDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  ///Serial.println("x");
  //Serial.print("Universe : ");
  //Serial.println(universe);
  digitalWrite(LED_BUILTIN, HIGH);
  
  if (debugArtnet) DebugArtNet(artnet);
  
  // we wachten totdat elke universe gevuld is 
  // als er een universe van de hele mik, nog niet data heeft recieved (universeRecieved[]==0), dan wordt channelBufferIsFull 0
  // als elke universe gevuld is , dan blijft channelBufferIsFull 1 , en roepen we de Led Show functie aan.
  channelBufferIsFull = 1;

  // set brightness of the whole strip
  //if (universe == 15)
  //{
  //  FastLED.setBrightness(data[0]);
  //  FastLED.show();
  //}

  // Store which universe has got in
  if (universe < numberOfUniverses)
  {
    universesReceived[universe] = 1;
  }

  // walk by each universe , to see if they are all populated so show a frame , if not , then channelBufferIsFull = 0, and we will not shows the leds yet.
  for (int i = 0 ; i < numberOfUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {     
      //Serial.println("Broke");
      channelBufferIsFull = 0;

      break;
    }
  }

  // read the universes and put into the right part of the display buffer
  // for (int i = 0; i < length / 4; i++)
  for (int i = 0; i < length/4 ; i++)
  {
    int ledIndex = i + (universe - startUniverse) * (previousDataLength/4);
    //Serial.println(ledIndex);
    if (ledIndex < numberOfPixels)
    {
	    //channelBuffer[ledIndex] = byte(data[i]);
      pixels.setPixelColor(i, 
                  data[i*4+0], 
                  data[i*4+1], 
                  data[i*4+2], 
                  0);
    }

  }
  previousDataLength = length;  
  
  //int c;
  //for(int i = 0 ; i < numberOfChannels; i++){
  //
  //  Serial.print(channelBuffer[i]);
  //  Serial.print(" ");
  //  c++;
  //  if (c > 50){
  //    c =0;
  //    Serial.println();
  //  }
  // }
  //Serial.println("***********************");
  // data is dus nog heel hier..

  // send data to leds
  //byte first;
  //byte second;
  //byte third;
  //byte fourth;
  //for (int i = 0; i < numberOfChannels; i = i + 4)
 // {
  //  first = i * 4 + 0;
  //  second = first + 1 ;
  //  third = second + 1;
  //  fourth = third + 1;
  //  
  //  Serial.print(channelBuffer[first]);
  //  Serial.print(" ");
  //  Serial.print(channelBuffer[second]);
  //  Serial.print(" ");
  // Serial.print(channelBuffer[third]);
  //  Serial.print(" ");
  //  Serial.print(channelBuffer[fourth]);
  //  Serial.println();;
//
  //  pixels.setPixelColor(i, 
  //                channelBuffer[first], 
  //                channelBuffer[second], 
  //                channelBuffer[third], 
  //                channelBuffer[fourth]);
  //}      
  //Serial.println("***************"  );

  // set the blink led to low , because there was no packet
  digitalWrite(LED_BUILTIN, LOW);

  //showLeds = true;
    pixels.show();

    memset(universesReceived, 0, numberOfUniverses); // Reset universeReceived to 0
    //memset(channelBuffer, 0, numberOfChannels);

    digitalWrite(LED_BUILTIN, LOW);
  //  showLeds = false;
}

boolean ConnectNetwork(void)
{
  boolean state = true;
  int i = 0;

  // Wait for connection
  Serial.print("Connecting ");
  //while (Ethernet.linkStatus() != LinkON) {
  //  delay(500);
  //  Serial.print(".");
  //  if (i > 20){
  //    state = false;
  //    break;
  //  }
  //  i++;
  //}
    
  // handle cable connection status
  if (state == false)
  {
    Serial.println();
    Serial.println("Network is not connected , reverting to demo mode");
    demoMode = true;
    Serial.print("Demo mode is : ");
    Serial.println(demoMode);
    return false;
  }
  else 
  {
    Serial.println("Network cable is connected !");
  }

  Serial.println("Connecting to network");
  Ethernet.begin(mac,ip, dns, gateway , subnet);

  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("IP address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

void DoNetworkCheck(){
  if (demoMode) {
    Serial.println("Demo mode");
    return;
  }
  if (Ethernet.linkStatus() == LinkOFF){
    Serial.println("Connection lost");
  }
  else{
    Serial.println("Connection failed");
  }
}

void DoShowPixels(){
  //return;
  //Serial.println("x");
  if (showLeds == true){
    //Serial.println("x");

    pixels.show();

    memset(universesReceived, 0, numberOfUniverses); // Reset universeReceived to 0
    //memset(channelBuffer, 0, numberOfChannels);

    digitalWrite(LED_BUILTIN, LOW);
    showLeds = false;
  }

}