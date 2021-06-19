 // Receive multiple universes via Artnet and control a strip of ws2811 leds via OctoWS2811
//
// This example may be copied under the terms of the MIT license, see the LICENSE file for details
//  https://github.com/natcl/Artnet
// 
// http://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811?p=55589&viewfull=1#post55589

#include <Artnet.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <SPI.h>
#include <OctoWS2811.h>

// Ideas for improving performance with WIZ820io / WIZ850io Ethernet:
// https://forum.pjrc.com/threads/45760-E1-31-sACN-Ethernet-DMX-Performance-help-6-Universe-Limit-improvements

//-------------------- for OctoWS8211--------------------//
const int ledsPerStrip = 12*40;
const byte numStrips= 1; // change for your setup
const int RGBWByteLength = 8; 
// a int is 4 bytes.
// 6 ints is 24 bytes is RGB
// 8 ints is 32 bytes is RGBW
DMAMEM int displayMemory[ledsPerStrip*RGBWByteLength];
int drawingMemory[ledsPerStrip*RGBWByteLength];
const int config = WS2811_GRBW | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
//----------------end of octows2811 inputs----------------//

// --------------- for NETWORK ---------------------------//
// Change ip and mac address for your setup
byte ip[] = {192, 168, 2, 80};
byte mac[] = {0x04, 0xE9, 0xE5, 0x00, 0x69, 0xEC};
// -------------end of NETWORK ---------------------------//

//---------------artnet and buffers-----------------------//
Artnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = ledsPerStrip * numStrips * 4; // Total number of channels you want to receive (1 led = 4 channels)
byte channelBuffer[numberOfChannels]; // Combined universes into a single array
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
// ------------end of arnet and buffers ------------------//

void setup(){
  // setup serial
  Serial.begin(115200);
  Serial.println("Hellow world!");

  // do a init test for the leds
  Serial.println("Initialize leds");
  leds.begin();
  InitLeds();
    
  //setup ethernet and udp socket
  Serial.println("Setup ethernet");
  InitNetwork();

  // show a blue blinkie blinkie to say we are running
  BlueBlinkieBlinkie();

  // this will be called for each packet received
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop(){
  // we call the read function inside the loop
  artnet.read();

  //Serial.println("x");
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data){
  Serial.println(universe);
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
  for (int i = 0; i < ledsPerStrip * numStrips; i++)
  {
    leds.setPixel(i, channelBuffer[(i) * 3], channelBuffer[(i * 3) + 1], channelBuffer[(i * 3) + 2]);
  }      
  
  if (sendFrame)
  {
    leds.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}

void InitNetwork(){
  artnet.begin(mac, ip);
  delay(250);
  
  Serial.print("Mac : ");
  for (int i=0;i<5;i++){
      Serial.print(":");
      Serial.print(mac[i]);
  }
  Serial.println();
  
  Serial.print("IP : ");
  for (int i=0;i<5;i++){
    Serial.print(".");
    Serial.print(ip[i]);
  }
  Serial.println();
  
  // if nothing appears after this, then the loop is not receiving your artnet package
}

void InitLeds(){
  leds.begin();
  // blink 12 leds in a row , blinking alle the leds
  for (int w=0;w<ledsPerStrip;w=w+12){
    for (int t=0;t<3;t++){
      for (int i=0;i<12;i++){
        leds.setPixel(w+i, 0,0,255,0); 
      }
    leds.show();
    }
  }
}

void BlueBlinkieBlinkie() {
  for (int d =0;d<3;d++){
    for (int b=255; b>0;b=b-8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, 0, 0, b);
        }
        leds.show();
    }
    
    for (int b =0; b<255; b=b+8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, 0, 0, b);
        }
        leds.show();
    }
  } 
  
  for (int b=255; b>0;b=b-8){
    for (int i = 0; i < ledsPerStrip * 8; i++) {
      leds.setPixel(i, 0, 0, b);
    }
    leds.show();
  }
}

void ShowBlack(){
    //-----set all black----//
  for (int i = 0; i < 1000; i++) {
    leds.setPixel(i, 0, 0, 0);
  }
  leds.show();
  delay(100);
}
