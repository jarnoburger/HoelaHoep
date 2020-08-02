#ifndef Led_h
#define Led_h
#include "Arduino.h"

class Led
{
  private:
    int  _ledState;
    long _previousMillis;
    long _interval;  
    int  _pin; 

  public:
    Led(int pin);
    void Loop();
};

#endif