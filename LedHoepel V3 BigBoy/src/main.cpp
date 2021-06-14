#include <Arduino.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <SimpleTimer.h>
#include <SPI.h>
#include <OctoWS2811.h>
#include "Extras.h"
#include "JArtnet.h"

/*
Receive multiple universes via Artnet and control a strip of ws2811 leds via OctoWS2811
This example may be copied under the terms of the MIT license, see the LICENSE file for details
https://github.com/natcl/Artnet
http://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811?p=55589&viewfull=1#post55589
Ideas for improving performance with WIZ820io / WIZ850io Ethernet:
https://forum.pjrc.com/threads/45760-E1-31-sACN-Ethernet-DMX-Performance-help-6-Universe-Limit-improvements
installeer uit de libraries folder op disk de octows2811 library en deinstalleer de pre installed lirbary !
*/

// Network settings
byte mac[]       = {  0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
byte ip[]        = { 192, 168,   2, 241 };
byte dns[]       = { 192, 168,   2,   1 };
byte gateway[]   = { 192, 168,   2,   1 };
byte subnet[]    = { 255, 255, 255,   0 };

byte broadcast[] = { 192, 168,   2, 255 };
SimpleTimer timerNetwork;

// Led Settings
#define ledsPerStrip 13 

// LED MEMORY
DMAMEM int displayMemory[ledsPerStrip*8];
int drawingMemory[ledsPerStrip*8];

const int config = WS2811_BRGW | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Artnet settings
JArtnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = ledsPerStrip * 4; // Total number of channels you want to receive (1 led = 4 channels)
byte channelBuffer[numberOfChannels]; // Combined universes into a single array

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool bufferFull = 1;
bool showFrame = 0;

// debug
bool demoMode = true;
bool debugArtnet = false;
bool debugUdp = false;

// Demo mode variables
uint8_t thisdelay = 40;                                       // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
int8_t thisrot = 1;                                           // Hue rotation speed. Includes direction.
uint8_t deltahue = 1;                                         // Hue change between pixels.
bool thisdir = 0;                                             // I use a direction variable instead of signed math so I can use it in multiple routines.

// declare functies
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
void WaitForDemoMode();
void DoDemoMode();
boolean ConnectNetwork();
void DoNetworkCheck();
void rainbow_march();

void setup()
{
  //Udp.begin(6454);  // alleen aanzetten als artnet op een andere poort moet

  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // start de check of we in demo mode verder gaan
  WaitForDemoMode();

  // do leds
  leds.begin();
  
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
  timerNetwork.setInterval(3000, DoNetworkCheck);

  // do artnet
  artnet.begin();
  artnet.setBroadcast(broadcast);

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);

  digitalWrite(LED_BUILTIN, LOW);
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
  // when we do demo mode , do it and return
  if (demoMode){
    DoDemoMode();
    return;
  }

  if (artnet.read() == ART_DMX)
  { 
    // set the blink led to high , because there was a packet
    digitalWrite(LED_BUILTIN, HIGH);

    // we call the read function inside the loop
    artnet.read();
  }

  // set the blink led to low , because there was no packet
  digitalWrite(LED_BUILTIN, LOW);
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

  //FastLED.show();
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

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  digitalWrite(LED_BUILTIN, HIGH);
  
  if (debugArtnet) DebugArtNet(artnet);
  
  bufferFull = 1;

  // Store which universe has got in
  if (universe < maxUniverses)
    universesReceived[universe] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      
      //Serial.println("Broke");
      bufferFull = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0 ; i < length ; i++)
  {
    int bufferIndex = i + ((universe - startUniverse) * length);
    if (bufferIndex < numberOfChannels) // to verify
      channelBuffer[bufferIndex] = byte(data[i]);
  }      

  // send to leds
  byte first;
  for (int i = 0; i < ledsPerStrip; i++)
  {
    first = i * 4;
    leds.setPixel(i, 
                  channelBuffer[first + 0], 
                  channelBuffer[first + 1], 
                  channelBuffer[first + 2] , 
                  channelBuffer[first + 3]);
  }      
  
  if (bufferFull)
  {
    showFrame = true;
    leds.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}

// connect to wifi – returns true if successful or false if not
boolean ConnectNetwork(void)
{
  boolean state = true;
  int i = 0;

  // Wait for connection
  Serial.print("Waiting for networkcable ");
  while (Ethernet.linkStatus() != LinkON) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  
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
  if (Ethernet.linkStatus() == LinkOFF){
    Serial.println("Ethernet cable is not connected");
  }
  else{
    Serial.println("Ethernet cable is not connected");
  };
}