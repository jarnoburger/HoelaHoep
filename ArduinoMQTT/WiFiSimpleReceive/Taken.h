#ifndef Taken_h
#define Taken_h
#include "Arduino.h"
#include "Led.h"

class Taken
{
  private:
    Led _led;

  public:
    Taken();
    void Start(Led led);
    void SetSpeed(int speed);
    void SetEffectIntensity(int intensity);
    void SetEffect(int effect);
    void SetPowerA(int power);
    void SetPowerB(int power);
    void SetColorA(int red, int green, int blue);
    void SetColorB(int red, int green, int blue);
};

#endif
