#include <Arduino.h>

#include <WiFiUdp.h>
#include <ArtnetWifi.h>

#include <FastLED.h>
#include "FastLED_RGBW.h"
void DebugUdp(WiFiUDP udp);
void DebugArtNet(ArtnetWifi artnet);

byte * Wheel(byte WheelPos);