/*  OctoWS2811 - High Performance WS2811 LED Display Library
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef OctoSK6812_h
#define OctoSK6812_h

#include <Arduino.h>
#include "DMAChannel.h"

#if TEENSYDUINO < 121
#error "Teensyduino version 1.21 or later is required to compile this library."
#endif
#ifdef __AVR__
#error "OctoSK6812 does not work with Teensy 2.0 or Teensy++ 2.0."
#endif

#define SK6812_RGB  0  // The WS2811 datasheet documents this way
#define SK6812_RBG  1
#define SK6812_GRB  2  // Most LED strips are wired this way
#define SK6812_GBR  3

#define SK6812_RGBW 4
#define SK6812_GRBW 5

class OctoSK6812 {
public:
  OctoSK6812(uint32_t numPerStrip, void *frameBuf, void *drawBuf, uint8_t config = SK6812_RGB);
  void begin(void);

  void setPixel(uint32_t num, int color);
  void setPixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue) {
    setPixel(num, color(red, green, blue));
  }
  void setPixel(uint32_t num, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    setPixel(num, color(red, green, blue, white));
  }
  int getPixel(uint32_t num);

  void show(void);
  int busy(void);

  int numPixels(void) {
    return stripLen * 8;
  }
  int color(uint8_t red, uint8_t green, uint8_t blue) {
    return (red << 16) | (green << 8) | blue;
  }
  int color(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
    return (red << 24) | (green << 16) | (blue << 8) | white;
  }
  

private:
  static uint16_t stripLen;
  static uint8_t pixelBits;
  static void *frameBuffer;
  static void *drawBuffer;
  static uint8_t params;
  static DMAChannel dma1, dma2, dma3;
  static void isr(void);
};

#endif
