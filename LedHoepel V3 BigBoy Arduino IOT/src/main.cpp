#include <Arduino.h>
#include <FastLED.h>
#include <ArtnetWifi.h>
#include <SPI.h>
#include "FastLED_RGBW.h"
#include <WiFiUdp.h>
#include "Extras.h"
#include <SimpleTimer.h>

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
const char* ssid = "Amperage";
const char* password = "MilaSteketee123!";
ArtnetWifi artnet;
IPAddress ip(192, 168, 2, 240);
IPAddress myDns(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
WiFiUDP Udp;
SimpleTimer timerNetwork;

// LED settings
const int numberOfPixels = 36; // change for your setup
#define DATA_PIN 2

// LED MEMORY
CRGBW leds[numberOfPixels];
CRGB *ledsRGB = (CRGB *) &leds[0];
int drawingMemory[numberOfPixels*8];

// Artnet settings
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = numberOfPixels * 4; // Total number of channels you want to receive (1 led = 4 channels)
const int numberOfUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
byte channelBuffer[numberOfChannels]; // Combined universes into a single array
bool universesReceived[numberOfUniverses];
bool showFrame = 1;
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

// declare functies
void WaitForDemoMode();
void InitTest();
void SetStatusToConnecting();
void SetStatusToConnected();
void OnDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
boolean ConnectWifi();
void DoDemoMode();
void rainbow_march();
void DoNetworkCheck();

void setup()
{
  //Udp.begin(6454);  // alleen aanzetten als artnet op een andere poort moet

  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);

  // start de check of we in demo mode verder gaan
  WaitForDemoMode();

  Serial.begin(115200);
  Serial.println("Welcome");
  Serial.println();
  Serial.print("Demo mode is : ");
  Serial.println(demoMode);
  Serial.print("Debug ArtNet is : ");
  Serial.println(debugArtnet);
  Serial.print("Debug Udp is : ");
  Serial.println(debugUdp);

  SetStatusToConnecting();
  ConnectWifi();
  SetStatusToConnected();
  timerNetwork.setInterval(3000, DoNetworkCheck);

  artnet.begin();
  Serial.println("Started Artnet");
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(numberOfPixels));
  Serial.println("Started FastLed");

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

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;
  WiFi.config(ip, myDns, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}

int frames = 0;

void loop()
{
  if (debugUdp){
    DebugUdp(Udp);
  }

  // when we do demo mode , do it and return
  if (demoMode){
    DoDemoMode();
    return;
  }

  // we call the read function inside the loop
  artnet.read();

  timerNetwork.run();
  //digitalWrite(LED_BUILTIN, LOW);
}

unsigned long lastCompleteFrameTime;

void OnDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  digitalWrite(LED_BUILTIN, HIGH);
  
  if (debugArtnet) DebugArtNet(artnet);
  
  // we wachten totdat elke universe gevuld is 
  // als er een universe van de hele mik, nog niet data heeft recieved (universeRecieved[]==0), dan wordt showFrame 0
  // als elke universe gevuld is , dan blijft showFrame 1 , en roepen we de Led Show functie aan.

  showFrame = 1;

  // set brightness of the whole strip
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
    FastLED.show();
  }

  // Store which universe has got in
  if (universe < numberOfUniverses){
    universesReceived[universe] = 1;
  }

  // walk by each universe , to see if they are all populated so show a frame , if not , then showFrame = 0, and we will not shows the leds yet.
  for (int i = 0 ; i < numberOfUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {     
      showFrame = 0;

      break;
    }
  }

  // read the universes and put into the right part of the display buffer
  for (int i = 0; i < length / 4; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 4);
    
    if (led < numberOfPixels)
    {
      // set the leds at ledIndex , to given RGBW channels for 1 pixel
      leds[led] = CRGBW(
        data[(i * 4) + 0], 
        data[(i * 4) + 1], 
        data[(i * 4) + 2],
        data[(i * 4) + 3]);
    }
  }
  previousDataLength = length;

  // if all the universe were filed , we can show the leds.
  if (showFrame)
  {
    lastCompleteFrameTime = millis();
    frames++;
    Serial.println(frames);

    FastLED.show();  
    memset(universesReceived, 0, numberOfUniverses); // Reset universeReceived to 0
  }
}

void DoDemoMode(){
  /*
  for (int i = 0 ; i < numberOfPixels ; i++) {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < numberOfPixels ; i++) {
    leds[i] = CRGBW(0, 255, 0, 0);
  }
  FastLED.show();
  delay(500);
  */

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

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbow_march();
  }

  FastLED.show();
} // part of DoDemoMode()

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels. You would need to change the max_bright value.
  
  if (thisdir == 0) thishue += thisrot; else thishue-= thisrot;  // I could use signed math, but 'thisdir' works with other routines.

  CHSV hsv;
  hsv.hue = thishue;
  hsv.val = 255;
  hsv.sat = 240;
  for( int i = 0; i < numberOfPixels; ++i) {
      leds[i] = hsv;
      hsv.hue += deltahue;
  }
} // rainbow_march()

void SetStatusToConnecting(){
  for (int i = 0 ; i < numberOfPixels; i++)
  {
    leds[i] = CRGBW(255, 255, 255, 255);
  }
    
  FastLED.show();
  delay(1000);
}

void SetStatusToConnected(){
  for (int i = 0 ; i < numberOfPixels; i++)
  {
    leds[i] = CRGBW(0, 255, 0,0);
  }
    
  FastLED.show();
  delay(1000);
}

void DoNetworkCheck(){
  int status = WiFi.status();
  // attempt to connect to Wifi network:
  if (status == WL_CONNECTED)
  {
    Serial.println("Connected");
  }
  else if (status == WL_DISCONNECTED)
  { 
    Serial.println("Disconnected");
  }
  else if (status == WL_CONNECTION_LOST)
  { 
    Serial.println("Connection lost");
  }
  else if (status == WL_CONNECT_FAILED)
  { 
    Serial.println("Connection failed");
  }
}