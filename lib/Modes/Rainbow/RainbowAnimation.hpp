#pragma once

#include "Animation.hpp"

#define MAX_LEDS 255
#define DEFAULT_LEDS 10

class RainbowAnimation : public Animation
{
public:
  void play(CRGB* leds, size_t len) override
  {
    byte *c;
    uint32_t i, j;

    for (j = 0; j < 256 * 5; ++j)
    { // 5 cycles of all colors on wheel
      for (i = 0; i < len; ++i)
      {
        c = Wheel(((i * 256 / len) + j) & 255);
        leds[i].setRGB(*c, *(c + 1), *(c + 2));
      }
      FastLED.show();
      delay(_delay);
    }
  }

  void configure(const ByteData& config) override
  {
    
  }

private:
  byte *Wheel(byte WheelPos)
  {
    if (WheelPos < 85)
    {
      c[0] = WheelPos * 3;
      c[1] = 255 - WheelPos * 3;
      c[2] = 0;
    }
    else if (WheelPos < 170)
    {
      WheelPos -= 85;
      c[0] = 255 - WheelPos * 3;
      c[1] = 0;
      c[2] = WheelPos * 3;
    }
    else
    {
      WheelPos -= 170;
      c[0] = 0;
      c[1] = WheelPos * 3;
      c[2] = 255 - WheelPos * 3;
    }

    return c;
  }

  byte c[3];
  int _delay = 30;
};