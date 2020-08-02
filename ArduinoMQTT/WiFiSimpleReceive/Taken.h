#ifndef Taken_h
#define Taken_h
#include "Arduino.h"

class Taken
{
  private:
    int pSpeed;      // -100 to 100 , hoe snel het effect gaat
    int pIntensity;  //    0 to 100 , hoe intens het effect is , intens is effect specifiek 
    int pEffect;     //    0 to 100 , kies het effect
    int pPowerA;     //    0 to 1000, hoe fel is kleur a
    int pPowerB;     //    0 to 1000, hoe fel is kleur b
    int pColorARed;  //    0 to  255 , color A red
    int pColorAGreen;//    0 to  255 , color A green
    int pColorABlue; //    0 to  255 , color A blue
    int pColorBRed;  //    0 to  255 , color B red
    int pColorBGreen;//    0 to  255 , color B green
    int pColorBBlue; //    0 to  255 , color B blue

  public:
    Taken();
    void SetSpeed(int s);
    void SetEffectIntensity(int i);
    void SetEffect(int e);
    void SetPowerA(int p);
    void SetPowerB(int p);
    void SetColorA(int r, int g, int b);
    void SetColorB(int r, int g, int b);
};

#endif