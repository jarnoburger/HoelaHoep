#ifndef LED_H
#define LED_H

#include <Arduino.h>

#include "Wiskunde.h"

class led
{
  private:
    void show_strip();
    void set_all(byte red, byte green, byte blue);
    void set_pixel(int Pixel, byte red, byte green, byte blue);
    static int lerp(byte a, byte b, float x);
    int fitpixel(int c);

  public:
    led();
    void start();
    void loop();

    void set_color_a(byte red, byte green, byte blue);
    void set_color_b(byte red, byte green, byte blue);
    void set_effect(int new_effect);
    void set_speed(float new_speed);
    void set_strength(int new_strength);

    void do_full();
    void do_fade(float anim_passed);
    void strobe(float anim_passed);
    void snow_sparkle(float anim_passed);
    void sparkle();
    void left_to_right(float anim_passed);
    void fairydust(float anim_passed);
    void Noise(float anim_passed);

    void meteor_rain(byte red, byte green, byte blue, byte meteor_size, byte meteor_trail_decay, boolean meteorRandomDecay, int SpeedDelay);
    void fade_to_black(int led_no, byte fade_value);  
};

#endif
