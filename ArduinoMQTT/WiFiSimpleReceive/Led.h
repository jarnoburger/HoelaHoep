#ifndef Led_h
#define Led_h
#include <Arduino.h>

class Led
{
  private:
    int  _ledState;
    long _previousMillis;
    long _interval;  
    int  _pin; 
    int  _numpixels;
    int  _pixelFormat; // NeoPixel color format & data rate. See the strandtest example for information on possible values.
    int  _speed;      // -100 to 100 , hoe snel het effect gaat
    int  _intensity;  //    0 to 100 , hoe intens het effect is , intens is effect specifiek 
    int  _effect;     //    0 to 100 , kies het effect
    int  _powerA;     //    0 to 1000, hoe fel is kleur a
    int  _powerB;     //    0 to 1000, hoe fel is kleur b
    int  _colorARed;  //    0 to  255 , color A red
    int  _colorAGreen;//    0 to  255 , color A green
    int  _colorABlue; //    0 to  255 , color A blue
    int  _colorBRed;  //    0 to  255 , color B red
    int  _colorBGreen;//    0 to  255 , color B green
    int  _colorBBlue; //    0 to  255 , color B blue
    

    
    void showStrip();
    void setAll(byte red, byte green, byte blue);
    void setPixel(int Pixel, byte red, byte green, byte blue);

  public:
    Led();
    void Start();
    void Loop();
    void Pow();

    void SetColorA(byte red, byte green, byte blue);
    void SetColorB(byte red, byte green, byte blue);
    void SetEffect(int effect);
    void SetSpeed(int speed);

    void DoFull();
    void DoFade();
    void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay);
    void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
    void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay);
    void Sparkle(byte red, byte green, byte blue, int SpeedDelay);
    void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay);
    void fadeToBlack(int ledNo, byte fadeValue);
    void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause);
};

#endif
