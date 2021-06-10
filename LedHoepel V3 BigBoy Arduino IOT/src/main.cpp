#include <Arduino.h>
#include <ArtnetWifi.h>
#include <FastLED.h>
#include <SPI.h>
#include "FastLED_RGBW.h"
#include <WiFiUdp.h>

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
*/

// Wifi settings
const char* ssid = "Amperage";
const char* password = "MilaSteketee123!";
ArtnetWifi artnet;
IPAddress ip(192, 168, 2, 240);
IPAddress myDns(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
WiFiUDP Udp;

// LED settings
#define ledsPerStrip 36 // change for your setup
const byte numStrips= 1; // change for your setup
#define DATA_PIN 2
CRGBW leds[ledsPerStrip];
CRGB *ledsRGB = (CRGB *) &leds[0];
int drawingMemory[ledsPerStrip*8];

// Artnet settings
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = ledsPerStrip * numStrips * 4; // Total number of channels you want to receive (1 led = 4 channels)
byte channelBuffer[numberOfChannels]; // Combined universes into a single array

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

// prut
bool artnetting = true;
int point = 0;
int range = 13;

void initTest();
void SetStatusToConnecting();
void SetStatusToConnected();
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
void DoCircle();
boolean ConnectWifi();


char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

void setup()
{
  //Udp.begin(6454);

  // led blinker
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);
  //leds.begin();
  
  Serial.begin(115200);
  Serial.println("Welcome, i am connecting");
  SetStatusToConnecting();

  ConnectWifi();
  
  artnet.begin();
  //artnet.setBroadcast(broadcast);

  //FastLED.addLeds<SK6812 , dataPin, GRB>(leds, ledsPerStrip); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //FastLED with RGBW
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(ledsPerStrip));

  initTest(); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  SetStatusToConnected();
  Serial.println("Connected");
  
  // this will be called for each packet received

  digitalWrite(LED_BUILTIN, LOW);
}

// connect to wifi – returns true if successful or false if not
boolean ConnectWifi(void)
{
  boolean state = true;
  int i = 0;
  WiFi.config(ip, myDns, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
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

void initTest()
{
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(255, 0, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(0, 255, 0, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(0, 0, 255, 0);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(0, 0, 0, 255);
  }
  FastLED.show();
  delay(500);
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(255, 255, 255, 255);
  }
  FastLED.show();
  delay(2000);
  for (int i = 0 ; i < ledsPerStrip ; i++) {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
  FastLED.show();
}

void SetStatusToConnecting(){
  for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
  {
    leds[i] = CRGBW(255, 255, 255, 255);
  }
    
  FastLED.show();
  delay(1000);
}

void SetStatusToConnected(){
    for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
  {
    leds[i] = CRGBW(0, 255, 0,0);
  }
    
  FastLED.show();
  delay(1000);
}

void loop()
{
  if (artnetting == true)
  {  
    // we call the read function inside the loop
    artnet.read();
  }
  else
  {
    DoCircle();
  }

  digitalWrite(LED_BUILTIN, LOW);


  /*
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
  */
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

void DoCircle()
{
  point ++;

  for (int s = 0; s < numStrips; s++){
    for (int i = 0 ; i < ledsPerStrip ; i++)
    {
      int t = i + (s * ledsPerStrip);
      //Serial.println(t);
      if (i == point){
        leds[t] = CRGBW(255, 255, 255, 255);
      }
      else
      {
        leds[t] = CRGBW(0, 0, 0, 0);
      }
    }
  }

  FastLED.show();
  delay(40);

  if (point > ledsPerStrip) { point = 0; }
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  digitalWrite(LED_BUILTIN, HIGH);
  
  if (artnetting == false) { return; }
  DebugArtNet();
  
  sendFrame = 1;

  // set brightness of the whole strip
  if (universe == 15)
  {
    FastLED.setBrightness(data[0]);
    FastLED.show();
  }

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
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    if (led < ledsPerStrip)
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
  }
  previousDataLength = length;

  if (sendFrame)
  {
    FastLED.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}

