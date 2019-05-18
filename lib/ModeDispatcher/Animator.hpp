#pragma once

#include <FreeRTOS.h>
#include <FastLED.h>
#include "usfl.h"
#include "Animation.hpp"
#include "configuration.h"

class Animator
{
public:
    Animator(CRGB *leds, size_t leds_len) : _leds(leds),
                                            _leds_len(leds_len)
    {
        _animation = nullptr;
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(_leds, _leds_len);
    }

    Animator(CRGB *leds, size_t leds_len, Animation *animation) : Animator(leds, leds_len)                                                                 
    {
        _animation = animation;
    }

    Animator(const Animator &animator) : 
                    Animator(animator._leds, animator._leds_len, animator._animation)
    {
    }

    void playAnimation()
    {
        if (!_animation)
            return;

        _animation->play(_leds, _leds_len);
    }

    void setAnimation(Animation *animation)
    {
        LOG("Animation seted!");
        _animation = animation;
    }

    ~Animator()
    {
        delete[] _leds;
    }

    Animator &operator=(const Animator &animator) = delete;

private:
    CRGB *_leds;
    size_t _leds_len;
    Animation *_animation;
};