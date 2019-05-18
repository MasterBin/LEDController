#pragma once

#include "Animation.hpp"
#include "FastLED.h"
#include "RunningLights/RunningLightsConfig.pb.h"
#include "pb_decode.h"

#define MAX_LEDS 255

class RunningLightsAnimation : public Animation
{
public:
    RunningLightsAnimation()
    {
        _delay = 30;
        color = { 0xFF, 0x00, 0x00 };
    }

    void play(CRGB *leds, size_t len) override
    {
        int Position = 0;

        for (int i = 0; i < len * 2; i++)
        {
            Position++; // = 0; //Position + Rate;
            for (int i = 0; i < len; i++)
            {
                leds[i]= CRGB(((sin16(i + Position) * 127 + 128) / 255) * color.R,
                         ((sin16(i + Position) * 127 + 128) / 255) * color.G,
                         ((sin16(i + Position) * 127 + 128) / 255) * color.B);
            }

            FastLED.show();
            delay(_delay);
        }
    }

    void configure(const ByteData &data) override
    {
        RunningLightsConfig config;
        pb_istream_t strm = pb_istream_from_buffer(data.data, data.len);
        pb_decode(&strm, RunningLightsConfig_fields, &config);

        switch(config.which_Settings)
        {
            case RunningLightsConfig_color_tag:
                color = config.Settings.color;
                break;
            case RunningLightsConfig_delay_tag:
                _delay = config.Settings.delay;
                break;
        }
    }

private:
    int _delay;
    Color color;

};