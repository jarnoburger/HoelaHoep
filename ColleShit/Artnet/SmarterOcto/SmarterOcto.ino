/*
this example will receive multiple universes via Artnet and control a strip of ws2811 leds via
Paul Stoffregen's excellent OctoWS2811 library: https://www.pjrc.com/teensy/td_libs_OctoWS2811.html
This example may be copied under the terms of the MIT license, see the LICENSE file for details

The leds are hidden behind some mirror layer (can't find the english word for that), so most of what you see in the video is behind the camera (the ballons).
The sound was created by someone in our team, so all is driven by midi events.
We have about 8500 ws2812b leds driven via artnet by 11 teensy. The whole thing is split in two parts, each driven by a computer running lightjams (because lightjams has "only" 32 artnet universes).

For this setup I made a few modifications to the orginial artnet code, so I put it here in case the modification might be useful. Modifications include :
- debug mode activation/desactivation (print useful info to serial)
- automatic MAC adress for teensy (taken from another code somewhere on this forum)
- automatic IP assignation (from the artnet universe number, so in the end I only have to specify the artnet universe and all the network thing is done). You provide a default IP, if autoIP is activated the code will keep the three first value and replace the last one with "artnet universe"+1 (to prevent IP 0). This may be deactivated if not necessary.
- maximum DMX channel to read from : for what I remember I had issues with lightjams sending only 510 "useful" channels (which is good with RGB leds) but teensys reading 512. Lightjams assume you might still want to use the 2 remaining adresses for something else than leds, so they are sent. Teensy code reads as much values DMX frame has, so adress 511 and 512 are read if provided. With my code you may prevent teensy from using those adresses. This issue might be only related to lightjams, other softwares allow to set how many channels are sent on a specific artnet universe, so this might be limited to 510.
- I deactivated "frame_received" verification which led the system to hang frequently. I understand the utility of this feature but with lots of artnet values I found it worked better without that verification.

Two things I had been investigating before I end up using lightjams, if someone thinks they are good ideas perhaps we might give it a shot :
- I wanted to try replacing artnet with OSC control. First because I'm a pure data user, and it doesn't proide any artnet interface. I'm aware of OLA but this seems limited to one universe output, which isn't really useful with RGB leds. Second argument, with a basic OSC impolementation, we might drive each pixel in a artnet way, but we could also code much more complex actions like full on, full off, full fade of all controlled pixels, or even light sequences with only one command flowing through the network. This combined with the SD card reader....wow
- I looked for a sACN implementation on arduinos/teensys but didn't succeed. It seems easier to setup than artnet, and a big argument to that is that lightjams would be able to drive 32 artnet universes + 32 sACN ones. So if we have a sACN version, with a 150$ (****ing good) application we might drive as much as 64 universes !

https://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811/page14?highlight=rgbw
*/

#include <Artnet.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <OctoWS2811.h>

boolean debug = 1; //activate serial printing, !! waits for a serial input to start loop

// OctoWS2811 settings
const int ledsPerStrip = 70; // change for your setup
const byte numStrips= 8; // change for your setup
DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Artnet settings
Artnet artnet;
const int startUniverse = 27; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int numberOfChannels = ledsPerStrip * numStrips * 3; // Total number of channels you want to receive (1 led = 3 channels)
byte channelBuffer[numberOfChannels]; // Combined universes into a single array
const int DMXmax = 510; // limit number of channels read from each universe (in case the artnet software always sends 512 value)

// Check if we got all universes
const int maxUniverses = (int) numberOfChannels / DMXmax + ((numberOfChannels % DMXmax) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;

// Change ip and mac address for your setup
boolean autoIP = 1;
byte ip[] = {192, 168, 0, 26};
char mac_string[20];

//everything on the network needs a unique MAC
#if defined(__MK20DX128__) || defined(__MK20DX256__)
// Teensy 3.x has MAC burned in
static byte mac[6];
void read(uint8_t word, uint8_t *mac, uint8_t offset) {
FTFL_FCCOB0 = 0x41; // Selects the READONCE command
FTFL_FCCOB1 = word; // read the given word of read once area

// launch command and wait until complete
FTFL_FSTAT = FTFL_FSTAT_CCIF;
while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

*(mac+offset) = FTFL_FCCOB5; // collect only the top three bytes,
*(mac+offset+1) = FTFL_FCCOB6; // in the right orientation (big endian).
*(mac+offset+2) = FTFL_FCCOB7; // Skip FTFL_FCCOB4 as it's always 0.
}
void read_mac() {
read(0xe,mac,0);
read(0xf,mac,3);
}
#else
void read_mac() {}
byte mac[] = {
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields
#endif

void print_mac() {
size_t count = 0;
for(uint8_t i = 0; i < 6; ++i) {
if (i!=0) count += Serial.print(":");
count += Serial.print((*(mac+i) & 0xF0) >> 4, 16);
count += Serial.print(*(mac+i) & 0x0F, 16);
}
sprintf(mac_string, "%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
Serial.println();
}



void setup()
{

//if (autoIP){ ip [4]= ( startUniverse == 0 ) ? 1 : startUniverse ;} //
if (autoIP){ ip [3]= (startUniverse + 1);} //avoid ip 0
read_mac();
if (debug){
Serial.begin(115200);
while(!Serial.available()){;} //if debug, wait until you send something to start the sketch
Serial.print("mac : ");
print_mac();
// for (int i ; i<3 ; i++){
// Serial.print(mac[i],HEX);
// Serial.print("-");
// }
// Serial.println(mac[3],HEX);
Serial.print("IP : ");
for (int i ; i<3 ; i++){
Serial.print(ip[i]);
Serial.print(".");
}
Serial.println(ip[3]);
Serial.print(numStrips);
Serial.print(" strips of ");
Serial.print(ledsPerStrip);
Serial.println(" leds");
}
artnet.begin(mac, ip);
leds.begin();
initTest();

// this will be called for each packet received
artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
// we call the read function inside the loop
artnet.read();
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
sendFrame = 1;

// Store which universe has got in
if (universe < maxUniverses)
if (debug){ Serial.print(universe);}
universesReceived[universe] = 1;

// for (int i = 0 ; i < maxUniverses ; i++)
// {
// if (universesReceived[i] == 0)
// {
// if (debug){Serial.println("Broke");}
// sendFrame = 0;
// break;
// }
// }
/* ICI length serait à 512 mais devrait etre 510, d'ou décalage de deux valeurs. Il lit les valeurs 511 et 512 (qui sont à 0), la premiere
valeur de l'univers suivant est donc appliquée au channel trois. Verifier, les 2-3 dernieres leds devraient etre éteintes
Tester en forçant length à 510 ?
cf. http://forum.pjrc.com/threads/24688-...toWS2811/page5 post #119
Tester également de ne pas démarrer à l'univers 0 sur la teensy*/

// read universe and put into the right part of the display buffer
for (int i = 0 ; i < min(length, DMXmax) ; i++)
{
int bufferIndex = i + ((universe - startUniverse) * min(length, DMXmax));
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

void initTest()
{
for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
leds.setPixel(i, 127, 0, 0);
leds.show();
delay(500);
for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
leds.setPixel(i, 0, 127, 0);
leds.show();
delay(500);
for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
leds.setPixel(i, 0, 0, 127);
leds.show();
delay(500);
for (int i = 0 ; i < ledsPerStrip * numStrips ; i++)
leds.setPixel(i, 0, 0, 0);
leds.show();
}
