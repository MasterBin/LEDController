#pragma once

#include "Animation.hpp"
#include "FastLED.h"
#include "AllColor/AllColorConfig.pb.h"
#include "pb_decode.h"

#define MAX_LEDS 255

class AllColorAnimation : public Animation
{
public:
    void play(CRGB *leds, size_t len) override
    {
        delay(10000);
    }

    void configure(const ByteData &config) override
    {
        AllColorConfig dconfig;
        pb_istream_t strm = pb_istream_from_buffer(config.data, config.len);
        pb_decode(&strm, AllColorConfig_fields, &dconfig);

        switch (dconfig.which_Settings)
        {
        case AllColorConfig_colorRGB_tag:
        {
            CRGB crgb = CRGB(
                dconfig.Settings.colorRGB.R,
                dconfig.Settings.colorRGB.G,
                dconfig.Settings.colorRGB.B);
            FastLED.showColor(crgb);
            break;
        }
        case AllColorConfig_colorHSV_tag:
        {
            CHSV chsv = CHSV(
                dconfig.Settings.colorHSV.H,
                dconfig.Settings.colorHSV.V,
                dconfig.Settings.colorHSV.S);
            FastLED.showColor(CRGB(chsv));
            break;
        }
        case AllColorConfig_brightness_tag:
        {
            FastLED.setBrightness(dconfig.Settings.brightness);
            FastLED.show();
            break;
        }
        }
    }
};