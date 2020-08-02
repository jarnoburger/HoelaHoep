#include <Arduino.h>
#include "Taken.h"

// effects
// 1 : alles aan
// 2 : fade tussen 2 kleuren (daardoor ook strobe)
// 3 : roteer een streep met kleur A , de anderen zijn kleur B
// 4 : wave een streep met kleur A , de anderen zijn kleur B
// 5 : random met kleur A over kleur B
// 6 : color wipe

// tussen 0 en 100
void Taken::SetSpeed(int s) {
  pSpeed = constrain(s, -100, 100);
}

// tussen 0 en 100
void Taken::SetEffectIntensity(int i){
  pIntensity = constrain(i, 0, 100);
}

// tussen 0 en het aantal effecten minus 1
void Taken::SetEffect(int e){
  pEffect = constrain(e, 0, 0);
}

// tussen 0 en 1000 , de lichtkracht van kleur A
void Taken::SetPowerA(int p){
  pPowerA = constrain(p, 0, 1000);
}

// tussen 0 en 1000 , de lichtkracht van kleur B
void Taken::SetPowerB(int p){
  pPowerB = constrain(p, 0, 1000);
}

// tussen 0 en 255 , kleur A
void Taken::SetColorA(int r, int g, int b){
  pColorARed   = constrain(r, 0, 255);
  pColorAGreen = constrain(g, 0, 255);
  pColorABlue  = constrain(b, 0, 255);
}

// tussen 0 en 255 , kleur B
void Taken::SetColorB(int r, int g, int b){
  pColorBRed   = constrain(r, 0, 255);
  pColorBGreen = constrain(g, 0, 255);
  pColorBBlue  = constrain(b, 0, 255);
}
