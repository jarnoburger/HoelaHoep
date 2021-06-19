//version3 - 8th March 2014 - uses universeSize and requires first_universe_number
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// libraries
#include <stdlib.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <SPI.h>
#include <OctoWS2811.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* uses libs and open licence stufffrom many sources incl. PJRC and ArtisticLicence.*/
//this sketch is a cut down test for receiving Art-net packages via ethernet
/* ****************************************************
  To get this to work for you, YOU NEED TO CHANGE:
  1) the mac address and IP info to same as your artnet sender
  2) change leds per strip and universe size (universeSize is size of software output universe selected)
  3) ensure buff2 size is large enough to take complete led array data set for each frame
  4) check first_universe_number, can be found using test sketch (usually is 1, sometimes is zero).

  for example, in pixelcontroller i have set the array of 60*8 leds, the universe size set to 30
  and the IP set to 192, 168, 1, 10, and port always remains 6454. At the moment, I can only get
  the frame rate max at 15fps, with 480 leds

  so for the sketch, I have ledsPerStrip=60, and I need to set universe size to 30
  number of channels and channel buffer go to 90 automatically (as 3*30). buff2 size can be smaller or biggger
  depending on the total array buffer size requied.
******************************************************** */

#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

//-------------------- for OctoWS8211--------------------//
const int ledsPerStrip = 12*40;//CHANGE FOR YOUR SETUP
const int RGBWByteLength = 8;
// a int is 4 bytes.
// 6 ints is 24 bytes is RGB
// 8 ints is 32 bytes is RGBW
DMAMEM int displayMemory[ledsPerStrip * RGBWByteLength];
int drawingMemory[ledsPerStrip * RGBWByteLength];
const int config = WS2811_GRBW;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
//----------------end of octows2811 inputs----------------//

byte mac[] = {   0x90, 0xA2, 0xDA, 0x0D, 0x4C, 0x8C}  ; //the mac adress in HEX of ethernet module/shield
byte ip[] = { 192, 168, 2, 200}; // the IP adress of your device, that should be in same universe of the network you are using
unsigned int localPort = 6454;      // DO NOT CHANGE artnet UDP port is by default 6454

//---------------artnet and buffers------------//
const int first_universe_number = 0;//CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int universeSize = 1;//CHANGE FOR YOUR SETUP
const int number_of_channels = universeSize * 512; // 512 channels in 1 universe
byte channel_buffer[number_of_channels];//buffer to store filetered DMX data//SHOULD BE SAME AS number_of_channels
byte buff2[2000];// increase buffer for filtered data to cover size of your total array(removed art-net header)
const int MAX_BUFFER_UDP = 768;//leave as is
char packetBuffer[MAX_BUFFER_UDP]; //buffer to store incoming data
short incoming_universe = 0; //leave as is (if suspect uni number issues, try changing first_uni number above first.
const int start_address = 0; // 0 if you want to read from channel 1
const int art_net_header_size = 17;

EthernetUDP Udp;

void setup() {
  Serial.begin(115200);
  Serial.println("Hellow world!");
  //setup ethernet and udp socket
  Ethernet.begin(mac, ip);
  delay(250);
  Udp.begin(localPort);
  delay(250);
  Serial.println("end of setup");
  Serial.println("waiting for incoming Artnet to start");
  //if nothing appears after this, then the loop is not receiving your
  //artnet package
  leds.begin();
  showInit();//show an led array to confirm
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  { 
    //Serial.print("Received packet of size ");//all print can be removed if not debugging
    //Serial.println(packetSize);

    Udp.read(packetBuffer, MAX_BUFFER_UDP);
    //can comment out serial print if not debugging
    //Serial.print(packetBuffer);// all print can be removed if not debugging
    //Serial.print("  ");

    //-------read incoming universe and sequence number and check for data series-------------//
    incoming_universe = bytes_to_short(packetBuffer[15], packetBuffer[14])
    if (incoming_universe == 6454) { return; }
    
    //-----this section can be commented out if not debugging
    //Serial.print("universe number = ");
    //Serial.print(incoming_universe);
    
    byte sequence = packetBuffer[12];
    //Serial.print("  ");
    ///Serial.print("sequence n0. = ");
    //Serial.println(sequence);
    //finshing printing to serial to check packets

    //read incoming universe and put into channel_buffer
    for (int i = start_address; i < number_of_channels; i++) {
      // strip the header from the packet buffer, and read the rest into or temp channelbuffer
      channel_buffer[i - start_address] = byte(packetBuffer[i + art_net_header_size + 1]);
    }
    // channel buffer is filled now

    //------put into the right part of the display buffer----//
    for (int i = 0; i < number_of_channels; i++) {
      // buff2 bevat alle data voor de ledjes in serie.
      // dus als universe 2 eraan komt , slaan we 512 channels over.
      // buffer index is dus index + (universe*512) met wat opsmuk om nog te spelen met een first universe, wat we toch neit doen.
      buff2[i + ((incoming_universe - first_universe_number)*number_of_channels)] = channel_buffer[i - start_address];
    }
    //Serial.println();

    //------send to leds----//
    for (int i = 0; i < ledsPerStrip * 8; i++) {
      byte r = buff2[(i * 4) + 0];
      byte g = buff2[(i * 4) + 1];
      byte b = buff2[(i * 4) + 2];
      byte w = buff2[(i * 4) + 3];
      //Serial.print("Index");
      //Serial.print(i);
      //Serial.print("R");
      //Serial.print(r);
      //Serial.print(" G");
      //Serial.print(g);
      //Serial.print(" B");
      //Serial.println(b);
      leds.setPixel(i, r , g, b,w);
    }
    leds.show();
  }
}

// --------------------------------------------
//---create initial image---//
// --------------------------------------------
void showInit() {
  for (int d =0;d<3;d++){
    
    for (int b =0; b<255; b=b+8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, (0, 0, b));
        }
        leds.show();
    }
    
    for (int b=255; b>0;b=b-8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, (0, 0, b));
        }
        leds.show();
    }
  } 

  ShowBlack();
}

void ShowBlack(){
    //-----set all black----//
  for (int i = 0; i < 1000; i++) {
    leds.setPixel(i, (0, 0, 0));
  }
  leds.show();
  delay(100);
}
