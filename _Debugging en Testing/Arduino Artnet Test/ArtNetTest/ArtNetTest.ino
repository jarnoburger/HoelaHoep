// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// libraries
#include <SPI.h>
#include <stdlib.h>      
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//this sketch is a cut down test for receiving Art-net packages via ethernet
#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x4C, 0x8C} 
; //the mac adress in HEX of ethernet shield or uno shield board
byte ip[] = {
  192, 168, 2, 80}; // the IP adress of your device, that should be in same universe of the network you are using

unsigned int localPort = 6454;      // artnet UDP port is by default 6454

//buffers
const int MAX_BUFFER_UDP = 768;
char packetBuffer[MAX_BUFFER_UDP]; //buffer to store incoming data
short incoming_universe=0;
EthernetUDP Udp;


void setup() {
  Serial.begin(115200);
  //setup ethernet and udp socket
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  delay(250);
    Serial.println("end of setup");
    Serial.println("waiting for incoming Artnet to start");
    //if nothing appears after this, then the loop is not receiving your
    //artnet package
}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize)
  { Serial.print("Received packet of size ");
    Serial.println(packetSize);
    
  Udp.read(packetBuffer,MAX_BUFFER_UDP);
  Serial.print(packetBuffer);
  Serial.print("  ");
   //-------read incoming universe and sequence number to check for data series-------------//
        incoming_universe=bytes_to_short(packetBuffer[15],packetBuffer[14])
        Serial.print("universe number = ");
        Serial.print(incoming_universe);
        byte sequence = packetBuffer[12];
        Serial.print("  ");
        Serial.print("sequence n0. = ");
        Serial.println(sequence);
  }
}
