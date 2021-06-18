/*
 * PixelInvaders tpm2.net implementation, Copyright (C) 2013 michael vogt <michu@neophob.com>
 * 2014-08-14 Experimental support added for 0xC9 (TPM2.Serial) Block Start Byte by tommy sools <info@tomsoft.nl>
 * 
 * If you like this, make sure you check out http://www.pixelinvaders.ch
 *
 * This file is part of PixelController.
 *
 * PixelController is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * PixelController is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 	
 *
 *
 */
#include <OctoWS2811.h>

const int ledsPerStrip = 22;//32;
DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
//#include <FastSPI_LED2.h>

//---- START USER CONFIG ----

#define DEBUG 1

//how many led pixels are connected
#define NUM_LEDS (7L * ledsPerStrip)//132

// Teensy 3.0 has the LED on pin 13
#define LED_PIN 2
//---- END USER CONFIG ----

#define BAUD_RATE 115200

//define some tpm constants
#define TPM2NET_HEADER_SIZE 4
#define TPM2NET_HEADER_IDENT 0x9c
#define TPM2NET_HEADER_IDENT_SERIAL 0xc9
#define TPM2NET_CMD_DATAFRAME 0xda
#define TPM2NET_CMD_COMMAND 0xc0
#define TPM2NET_CMD_ANSWER 0xaa
#define TPM2NET_FOOTER_IDENT 0x36

//3 byte per pixel or 24bit (RGB)
#define BPP 3

//package size we expect. 
#define MAX_PACKED_SIZE 520

#define PIXELS_PER_PACKET 170

// buffers for receiving and sending data
uint8_t packetBuffer[MAX_PACKED_SIZE]; //buffer to hold incoming packet
uint16_t psize;
uint8_t currentPacket;
uint8_t totalPacket;
uint16_t ledOffset;

//********************************
// SETUP
//********************************
void setup() {  
    memset(packetBuffer, 0, MAX_PACKED_SIZE);
  Serial.begin(BAUD_RATE);
  Serial.flush();
  Serial.setTimeout(20);
#ifdef DEBUG  
  Serial.println("HI");
#endif 

 // pinMode(ledPin, OUTPUT);
 // debugBlink(500);
  //first blink: init
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);  

  leds.begin();
  leds.show();
  
  
  //Flickering issues?
  //...it turned out that as my PSU got hotter, the voltage was dropping towards the end of the LED strip.
  //Tried feeding power to both ends of the strip, only delayed the issue slightly.  Changed to a bigger PSU and fault went away 
  //(dual ends feeding power).

  // For safety (to prevent too high of a power draw), the test case defaults to
  // setting brightness to 50% brightness  
//  LEDS.setBrightness(64);
  
  showInitImage();      // display some colors
}

//********************************
// LOOP 
//********************************
void loop() {  
  
  int16_t res = readCommand();
  if (res > 0) {
//    leds.setPixel(1,250);
//    leds.show();
#ifdef DEBUG      
    Serial.print("FINE: ");
    Serial.print(psize, DEC);    
    Serial.print("/");
    Serial.print(currentPacket, DEC);    
#if defined (CORE_TEENSY_SERIAL)    
    Serial.send_now();
#endif

#endif
    digitalWrite(LED_PIN, HIGH);
    updatePixels();
    digitalWrite(LED_PIN, LOW);    
  }
#ifdef DEBUG      
  else {
    if (res!=-1) {
      Serial.print("ERR: ");
      Serial.println(res, DEC);
#if defined (CORE_TEENSY_SERIAL)          
      Serial.send_now();
#endif
    }
  }
#endif  
}

//********************************
// UPDATE PIXELS
//********************************
void updatePixels() {
  uint8_t nrOfPixels = psize/3;
  
  uint16_t ofs=0;
  //uint16_t ledOffset = PIXELS_PER_PACKET*currentPacket;
  
  for (uint32_t i=0; i<nrOfPixels; i++) {  
    leds.setPixel(i+ledOffset, Color(packetBuffer[ofs++], packetBuffer[ofs++], packetBuffer[ofs++]));
  }
  ledOffset += nrOfPixels;

  //update only if all data packets recieved
  if (currentPacket == totalPacket - 1) {
#ifdef DEBUG      
    Serial.println("DRAW!");
#if defined (CORE_TEENSY_SERIAL)        
    Serial.send_now();
#endif    
#endif    
   // LEDS.show();
leds.show();  
} else {
#ifdef DEBUG     
    Serial.print("NOTUPDATE: ");
    Serial.println(currentPacket, DEC);
#if defined (CORE_TEENSY_SERIAL)   
    Serial.send_now();
#endif
#endif        
  }
}

//********************************
// READ SERIAL PORT
//********************************
int16_t readCommand() {  
  uint8_t startChar = Serial.read();  
  if ((startChar != TPM2NET_HEADER_IDENT) && (startChar != TPM2NET_HEADER_IDENT_SERIAL)) {
    return -1;
  }
  
  uint8_t dataFrame = Serial.read();
  if (dataFrame != TPM2NET_CMD_DATAFRAME) {
    return -2;  
  }
  
  uint8_t s1 = Serial.read();
  uint8_t s2 = Serial.read();  
  psize = (s1<<8) + s2;
  if (psize < 6 || psize > MAX_PACKED_SIZE) {
    return -3;
  }  

  if (startChar == TPM2NET_HEADER_IDENT_SERIAL) {
    currentPacket = 0;
    totalPacket = 1;
  }
  else {
    currentPacket = Serial.read();  
    totalPacket = Serial.read();
  }
  ledOffset = 0;
  
  //get remaining bytes
  uint16_t recvNr = Serial.readBytes((char *)packetBuffer, psize);
  if (recvNr!=psize) {
    return -5;
  }  

  uint8_t endChar = Serial.read();
  if (endChar != TPM2NET_FOOTER_IDENT) {
    return -6;
  }

  return psize;
}


// --------------------------------------------
//     create initial image
// --------------------------------------------
void showInitImage() {

  int a =1;
  for (int i = 0 ; i < NUM_LEDS; i++ ) {
    leds.setPixel(i,Color(i*0.2,i*0.5,i*0.5));
  }
  leds.show();
}

void debugBlink(uint8_t t) {
  digitalWrite(LED_PIN, HIGH);
  delay(t);
  digitalWrite(LED_PIN, LOW);  
}

/* Helper functions */
// Create a 24 bit color value from R,G,B 
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 8 bits of each value and append them end to end
  return( ((unsigned int)b & 0xFF )<<16 | ((unsigned int)r & 0xFF)<<8 | (unsigned int)g & 0xFF);
}
