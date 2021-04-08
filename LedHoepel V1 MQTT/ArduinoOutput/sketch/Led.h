#ifndef Led_h
#define Led_h
#include <Arduino.h>

class Led
{
  private:
    int  _ledState;
    long _previousFrameRateMillis;
    long _interval;  
    int  _pin; 
    int  _numpixels;
    int  _pixelFormat; // NeoPixel color format & data rate. See the strandtest example for information on possible values.
    //int  _speed;      // -100 to 100 , hoe snel het effect gaat
    //int  _effect;     //    0 to 100 , kies het effect
    //int  _colorARed;  //    0 to  255 , color A red
    //int  _colorAGreen;//    0 to  255 , color A green
    //int  _colorABlue; //    0 to  255 , color A blue
    //int  _colorBRed;  //    0 to  255 , color B red
    //int  _colorBGreen;//    0 to  255 , color B green
    //int  _colorBBlue; //    0 to  255 , color B blue
      
    void showStrip();
    void setAll(byte red, byte green, byte blue);
    void setPixel(int Pixel, byte red, byte green, byte blue);
    int lerp(byte a, byte b, float x);

  public:
    Led();
    void Start();
    void Loop();

    void SetColorA(byte red, byte green, byte blue);
    void SetColorB(byte red, byte green, byte blue);
    void SetEffect(int effect);
    void SetSpeed(int speed);

    void DoFull();
    void DoFade(float animPassed);
    void Strobe(float animPassed);
    void SnowSparkle(float animPassed);
    void Sparkle();
    void LeftToRight(float animPassed);
    void RightToLeft(float animPassed);
    
    void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay);
    void fadeToBlack(int ledNo, byte fadeValue);
    
};

#endif
