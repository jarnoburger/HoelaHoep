#include <stdlib.h>
#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include <SPI.h>
#include <OctoWS2811.h>

// het kunstwerk veracht
// 560 RGB leds op 1 pin
// het zijn dan 8 pinnen


//------------------------------LED------------------------------------//
const int ledsPerStrip     =       560;                              // 4x140 rgb leds =  560 leds per Pin
const int RGBWByteLength   =         6;                                // 1 Int is 4 Bytes. 6 Ints is 24 Bytes is RGB. 8 Ints is 32 Bytes is RGB
DMAMEM int displayMemory[ledsPerStrip * RGBWByteLength];               // The DMA memory display block needs to be big enough to fill all the RGB leds. Is the memory that is sent to the leds.
int drawingMemory[ledsPerStrip * RGBWByteLength];                      // This is a temp memory block where we park the values in the right order.
const int config = WS2811_GRB | WS2811_800kHz;                                          // WS2811_GRB or WS2811_GRBW
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);   // Object contains the leds

//----------------end of octows2811 inputs----------------//

// ----------------- NETWORK -----------------------------//
byte mac[]     = { 0x90, 0xA2, 0xDA, 0x0D, 0x4C, 0x8C}  ; // the mac adress in HEX of ethernet module/shield
byte ip[]      = {  192,  168,    0,    3};               // the IP adress of your device, that should be in same universe of the network you are using
byte subnet[]  = {  255,  255,  255,    0};               // alleen de ip adressen van 192.168.0.xxx moeten beantwoord worden (bijv udp-reply)
byte gateway[] = {  192,  168,    0,    1};               // hier moet de ip adres van de router staan (deze verdeeld de packets overal (dus ook een udp-reply)
byte dns[]     = {  192,  168,    0,    1};               // hier moet de ip adres van de router staan (is eigenlijk voor WWW , maar moet mee in de initializer
unsigned int localPort = 6454;                            // DO NOT CHANGE artnet UDP port is by default 6454
// -------------------------------------------------------//

//---------------ARTNET and BUFFERS-----------------------//
EthernetUDP Udp;                                          // The class we listen with
const int first_universe_number     = 0;                  // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
const int universeSize              = 32;                 // used for doublecheking if given universe in the artnetpacket  should be dealt with, or not
const int number_of_channels      = 420;                  // number of channels used per universe
byte channel_buffer[number_of_channels];                  // buffer to store filetered DMX data//SHOULD BE SAME AS number_of_channels
byte buff2[number_of_channels * universeSize] ;           // increase buffer for filtered data to cover size of your total array(removed art-net header)
const int MAX_BUFFER_UDP          = 768;                  // leave as is , this is big enough to get artnet packets
char packetBuffer[MAX_BUFFER_UDP]      ;                  // buffer to store incoming data
short incoming_universe             = 0;                  // leave as is (if suspect uni number issues, try changing first_uni number above first.
const int start_address             = 0;                  // 0 if you want to read from channel 1
const int art_net_header_size      = 17;                  // we skip these bytes in the data poacket, to get to the universe and channels
int sequenceCount                   = 0;                  // we dont used this , but this is to get the right order of packets
// -------------------------------------------------------//

#define bytes_to_short(h,l) ( ((h << 8) & 0xff00) | (l & 0x00FF) );

void setup() {
  //setup serial
  Serial.begin(115200);
  Serial.flush();
  Serial.setTimeout(20);
  Serial.println("Hello world!");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);  
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);  
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);  
  delay(250);
  
  // do a init tests for the leds
  Serial.println("*** Initialize leds");
  InitLeds();
    
  //setup ethernet and udp socket
  Serial.println("*** Initialize ethernet");
  InitNetwork();
  
  // show a blue blinkie blinkie to say we are running
  Serial.println("*** Do blue blink blink");
  BlueBlinkieBlinkie();

  Serial.println("*** Ready");
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  
  { 
//      Serial.print("Received packet of size ");//all print can be removed if not debugging
//      Serial.println(packetSize); < geeft 530 bij een pakket
    OnDMXFrame();

  }

}

void OnDMXFrame(){
  Udp.read(packetBuffer, MAX_BUFFER_UDP);
  //Serial.print(packetBuffer);
  //Serial.print("  ");

  //Serial.println("x");

/*  
  Serial.println(packetBuffer[0]);
  Serial.println(packetBuffer[1]);
  Serial.println(packetBuffer[2]);
  Serial.println(packetBuffer[3]);
  Serial.println(packetBuffer[4]);
  Serial.println(packetBuffer[5]);
  Serial.println(packetBuffer[6]);
  Serial.println(packetBuffer[7]);
  Serial.println(packetBuffer[8]);
  Serial.println(packetBuffer[9]);
  Serial.println(packetBuffer[10]);
  Serial.println(packetBuffer[11]);
  Serial.println(packetBuffer[12]);
  Serial.println(packetBuffer[13]);
  Serial.println(packetBuffer[14]);
  Serial.println(packetBuffer[15]);


  int comb = (packetBuffer[15] << 8) | packetBuffer[14];
  Serial.println(comb);
*/
  //-------read incoming universe and sequence number and check for data series-------------//
  incoming_universe = bytes_to_short(packetBuffer[15], packetBuffer[14])
  if (incoming_universe ==        6454) { return; }
  if (incoming_universe <            0) { return; }
  if (incoming_universe > universeSize) { return; }
  //Serial.print("universe number = ");
  //Serial.println(incoming_universe);
  
  //-------read incoming sequence-----------------------------------------------------------//
  byte sequence = packetBuffer[12];
  //Serial.print("  ");
  //Serial.print("sequence n0. = ");
  //Serial.println(sequence);
  //finshing printing to serial to check packets
  
  //-------read incoming un  iverse and put into channel_buffer-------------------------------//
  for (int i = start_address; i < number_of_channels; i++) {
    // strip the header from the packet buffer, and read the rest into or temp channelbuffer
    channel_buffer[i - start_address] = byte(packetBuffer[i + art_net_header_size + 1]);
  }
  
  //------put into the right part of the display buffer-------------------------------------//
  // buff2 bevat alle data voor de ledjes in serie.
  // dus als universe 2 eraan komt , slaan we 512 channels over.
  // buffer index is dus index + (universe*512) met wat opsmuk om nog te spelen met een first universe, wat we toch neit doen.
  for (int i = 0; i < number_of_channels; i++) {
    buff2[i + ((incoming_universe - first_universe_number)*number_of_channels)] = channel_buffer[i - start_address];
  }

  // only , with a chance of that all universes could be updated by artnet, we update the leds
  sequenceCount ++;
  if (sequenceCount > universeSize){
      ShowLeds();
      sequenceCount = 0;
  }

}

void ShowLeds(){
    //------send to leds----//
  int c = 0;
  int strips = 8;
  int pixelCount = ledsPerStrip * strips;
  for (int i = 0; i < pixelCount; i++) {
    c = i * 3;  
    leds.setPixel(i,
    buff2[c + 0], 
    buff2[c + 1], 
    buff2[c + 2]);
  }
  
  leds.show();
}

void InitNetwork(){
  // retrieve mac adress
  Serial.print("Mac                               : ");
  for(uint8_t by=0; by<2; by++) mac[by]=(HW_OCOTP_MAC1 >> ((1-by)*8)) & 0xFF;
    for(uint8_t by=0; by<4; by++) mac[by+2]=(HW_OCOTP_MAC0 >> ((3-by)*8)) & 0xFF;
  Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.print("IP                                : ");
  Serial.printf("%03d.%03d.%03d.%03d\n", ip[0], ip[1], ip[2], ip[3]);

  Serial.print("GATEWAY                           : ");
  Serial.printf("%03d.%03d.%03d.%03d\n", gateway[0], gateway[1], gateway[2], gateway[3]);

  Serial.print("SUBNET                            : ");
  Serial.printf("%03d.%03d.%03d.%03d\n", subnet[0], subnet[1], subnet[2], subnet[3]);

  Serial.print("DNS                               : ");
  Serial.printf("%03d.%03d.%03d.%03d\n", dns[0], dns[1], dns[2], dns[3]);

  Serial.print("UDP-port                          : ");
  Serial.println(localPort);

  Serial.print("UDP incoming-buffer byte-size     : ");
  Serial.println(sizeof(packetBuffer));
      
  Ethernet.begin(mac, ip);
  delay(250);
  Serial.println("Ethernet.begin()                  : SUCCES !");

  Udp.begin(localPort);
  delay(250);  
  Serial.println("UDP.begin()                       : SUCCES !");

  //if nothing appears after this, then the loop is not receiving your artnet package
}

void InitLeds(){
  if (RGBWByteLength == 6){
    Serial.println("The led type used is              : RGB  (3x2 Bytes)");
  }
  if (RGBWByteLength == 8) {
    Serial.println("We are using RGBW leds            : RGBW (4x2 Bytes)");
  }
  
  Serial.print("Leds per strip                    : ");
  Serial.println(ledsPerStrip);

  Serial.print("Incoming channel-buffer byte-size : ");
  Serial.println(sizeof(channel_buffer));

  Serial.print("Display-memory byte-size          : ");
  Serial.println(sizeof(displayMemory)); 

  Serial.print("Drawing-memory byte-size          : ");
  Serial.println(sizeof(drawingMemory));

  Serial.print("First universe                    : ");
  Serial.println(first_universe_number);

  Serial.print("Number of universes               : ");
  Serial.println(universeSize);

  Serial.print("Number of channels in a universe  : ");
  Serial.println(number_of_channels);

  Serial.print("Begin to listen to universe       : ");
  Serial.println(incoming_universe); 

  Serial.print("Begin to listen to channel        : ");
  Serial.println(start_address);
  
  leds.begin();

    for (int w=0; w<ledsPerStrip * 8; w=w+12) 
    {
        for (int i=0;i<12;i++)
        {
          //leds.setPixel(w+i, 255,255,0,0);
          leds.setPixel(w+i, 64,64,0); 
        }
        leds.show();
    }

}

void BlueBlinkieBlinkie() {
  for (int d =0;d<3;d++){
    for (int b=64; b>0;b=b-8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, b, b, 0);
        }
        leds.show();
    }
    
    for (int b =0; b<64; b=b+8){
        for (int i = 0; i < ledsPerStrip * 8; i++) {
          leds.setPixel(i, b, b, 0);
        }
        leds.show();
    }
  } 
  
  for (int b=64; b>0;b=b-8){
    for (int i = 0; i < ledsPerStrip * 8; i++) {
      leds.setPixel(i, b, b, 0);
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
