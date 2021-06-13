#include <Arduino.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "JArtnet.h"
#include <SPI.h>
#include <OctoWS2811.h>

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
byte ip[] = {192, 168, 2, 200};
byte broadcast[] = {192, 255, 255, 255};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};

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
bool sendFrame = 1;

// prut
bool arting = true;
int point = 0;
int range = 13;


// declare functies
void SetStatusToConnecting();
void SetStatusToConnected();
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);

void setup()
{
  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);
  leds.begin();
  
  Serial.begin(115200);
  Serial.println("Welcome, i am connecting");
  SetStatusToConnecting();

  artnet.begin(mac, ip);
  artnet.setBroadcast(broadcast);
  SetStatusToConnected();
  Serial.println("Connected");
  
  // this will be called for each packet received
  if (arting) artnet.setArtDmxCallback(onDmxFrame);

  digitalWrite(LED_BUILTIN, LOW);
}

void SetStatusToConnecting(){
  for (int i = 0 ; i < ledsPerStrip; i++)
  {
    leds.setPixel(i, 255, 255, 255, 255);
  }
    
  leds.show();
  delay(1000);
}

void SetStatusToConnected(){
    for (int i = 0 ; i < ledsPerStrip; i++)
  {
    leds.setPixel(i, 0, 0, 255, 0);
  }
    
  leds.show();
  delay(1000);
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

void DoDemoMode()
{

}

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
  
  if (arting == false) { return; }
  DebugArtNet();
  
  sendFrame = 1;

  // Store which universe has got in
  if (universe < maxUniverses)
    universesReceived[universe] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      
      //Serial.println("Broke");
      sendFrame = 0;
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
  
  if (sendFrame)
  {
    leds.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}