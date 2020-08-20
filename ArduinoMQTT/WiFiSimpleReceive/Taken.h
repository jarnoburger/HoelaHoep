#ifndef TAKEN_H
#define TAKEN_H
#include <Arduino.h>
#include "Led.h"

class taken
{
  private:
    led _led;

    void set_color_a(char bericht[]);
    void set_color_b(char bericht[]);
    void set_effect(char bericht[]);
    void set_speed (char bericht[]);
    void set_strength(char bericht[]);

    void test();
    byte hex_color_to_int(char upper, char lower);

  public:
    taken();
    void start(led led);
    void parse(String topic, char message[]);
    void show_error();
};

#endif
