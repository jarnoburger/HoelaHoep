#include <Arduino.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <SimpleTimer.h>
#include <SPI.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "FastLED_RGBW.h"

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

#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

// Network settings
const char* ssid = "BEAM";
const char* password = "MilaSteketee123!";
IPAddress ip(192, 168, 1, 106);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
WiFiUDP Udp;
SimpleTimer timerNetwork;

// LED settings
const int numberOfPixels = (6*144)+8;  // change for your setup
#define DATA_PIN 2
#define FPS 25

// LED MEMORY
CRGBW leds[numberOfPixels];
CRGB *ledsRGB = (CRGB *) &leds[0];
int drawingMemory[numberOfPixels*8];
SimpleTimer timerShowPixels;

// Artnet settings
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = 110*4; // Total number of channels you want to receive (1 led = 4 channels) 110x4 leds in 1 unverse
const int maxUniverses = 1;
const int MAX_BUFFER_UDP          = 768;                  // leave as is , this is big enough to get artnet packets
char packetBuffer[MAX_BUFFER_UDP]      ;                  // buffer to store incoming data
const int art_net_header_size      = 17;                  // we skip these bytes in the data poacket, to get to the universe and channels
unsigned int localPort = 6454;                            // DO NOT CHANGE artnet UDP port is by default 6454
byte channel_buffer[numberOfChannels];                  // buffer to store filetered DMX data//SHOULD BE SAME AS number_of_channels
byte buff2[4000]                       ;                  // increase buffer for filtered data to cover size of your total array(removed art-net header)


// Check if we got all universes
bool universesReceived[maxUniverses];
bool channelBufferIsFull = false;
bool showLeds = false;
int previousDataLength = 0;

// debug
bool demoMode = false;
bool debugArtnet = true;
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
void OnDMXFrame();

void setup()
{
  //Udp.begin(6454);  // alleen aanzetten als artnet op een andere poort moet

  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // start de check of we in demo mode verder gaan
  WaitForDemoMode();

  // do leds
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(numberOfPixels));
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

  Serial.println("Started Artnet");
  Serial.print("Number of pixels : ");
  Serial.println(numberOfPixels);
  Serial.print("Number of channels : ");
  Serial.println(numberOfChannels);
  Serial.print("Number of universes : ");
  Serial.println(maxUniverses);
  Serial.print("Start universe : ");
  Serial.println(startUniverse);

  // led blinker
  digitalWrite(LED_BUILTIN, LOW);

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

int t = 0;
void loop()
{
  // when we do demo mode , do it and return
  if (demoMode){
    DoDemoMode();
    return;
  }

  // we call the read function inside the loop
  int packetSize = Udp.parsePacket();
  if (packetSize > 0 && packetSize< MAX_BUFFER_UDP)
  { 
    OnDMXFrame();
  }
  timerNetwork.run();
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

  //EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    rainbow_march();
  //}

  FastLED.show();
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

void OnDMXFrame()
{
  if (Udp.read(packetBuffer, MAX_BUFFER_UDP) == 0) return;
  Serial.println(sizeof(packetBuffer));
  int incoming_universe = bytes_to_short(packetBuffer[15], packetBuffer[14])
  if (incoming_universe ==        6454) { return; }
  if (incoming_universe <            0) { return; }
  if (incoming_universe > maxUniverses) { return; }
  Serial.println(incoming_universe);

  //-------read incoming universe and put into channel_buffer-------------------------------//
  for (int i = 0; i < numberOfChannels; i++) {
    // strip the header from the packet buffer, and read the rest into or temp channelbuffer
    channel_buffer[i] = byte(packetBuffer[i + art_net_header_size + 1]);
  }

  
  for (int i = 0; i < numberOfChannels; i++) {
    buff2[i + ((incoming_universe )*numberOfChannels)] = channel_buffer[i];
  }
  memset(channel_buffer,0, sizeof(channel_buffer));

  // read the universes and put into the right part of the display buffer
  int row = numberOfChannels / 4;
  for (int i = 0; i < numberOfChannels / 4; i++)
  {
    for (int r = 0; r < 8;r++){
      // set the leds at ledIndex , to given RGBW channels for 1 pixel
      int t = (i* 4);
      leds[i+r*row].r = buff2[t + 0];
      leds[i+r*row].g = buff2[t + 1];
      leds[i+r*row].b = buff2[t + 2];
      leds[i+r*row].w = buff2[t + 3];
    }
  }

  FastLED.show();  
  //FastLED.delay(30);
}

boolean ConnectNetwork(void)
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

    Udp.begin(localPort);

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

  int status = WiFi.status();
  // attempt to connect to Wifi network:
  if (status == WL_CONNECTED)
  {
    Serial.println("Connected");
  }
  else if (status == WL_DISCONNECTED)
  { 
    Serial.println("Disconnected, retry..");
    WiFi.end();
    ConnectNetwork();
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
