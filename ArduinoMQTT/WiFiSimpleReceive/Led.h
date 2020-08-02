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
    int  _numpixels;

    // NeoPixel color format & data rate. See the strandtest example for
    // information on possible values.
    int  _pixelFormat;
    

  public:
    Led(int pin, int numpixels);
    void Loop();
    void Pow();
};

#endif
