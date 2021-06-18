#include <Arduino.h>

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>

#include "JArtnet.h"

#include <FastLED.h>
#include "FastLED_RGBW.h"
void DebugUdp(EthernetUDP udp);
void DebugArtNet(JArtnet artnet);

byte * Wheel(byte WheelPos);