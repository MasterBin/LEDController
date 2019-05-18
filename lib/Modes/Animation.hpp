#pragma once

#include <FastLED.h>
#include "Animator.hpp"
#include "ByteData.hpp"

#define MAX_LEDS 255

struct Animation
{
    virtual void play(CRGB* leds, size_t len);
    virtual void configure(const ByteData& configuration);
};

