#ifndef Taken_h
#define Taken_h
#include <Arduino.h>
#include "Led.h"

#define EffectSparcle "Sparcle"
#define EffectEye     "Eye"
#define EffectMeteor  "Meteor"
#define FullColor     "FullColor"

class Taken
{
  private:
    Led _led;

    void SetColorA(char bericht[]);
    void SetColorB(char bericht[]);
    void SetEffect(char bericht[]);
    void SetSpeed (char bericht[]);
    void Test();
    
  public:
    Taken();
    void Start(Led led);
    void Parse(String topic, char message[]);
};

#endif
