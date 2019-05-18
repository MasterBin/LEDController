#include <HardwareSerial.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <FastLED.h>

#include "BTServer.hpp"
#include "Animator.hpp"
#include "usfl.h"
#include "ModeDispatcher.hpp"
#include "AppStarter.hpp"

CRGB *leds;
Animator *animator;
ModeDispatcher *modeDispatcher;
AppStarter* appstarter;

void animationTask(void *xParam)
{
    for (;;)
    {
        LOG("Animation started!");
        animator->playAnimation();
        LOG("Animation has ended");
    }
}

void startApplication(unsigned int led_count)
{
    leds = new CRGB[led_count];
    animator = new Animator(leds, led_count);
    modeDispatcher = new ModeDispatcher(animator);
    modeDispatcher->setMode(ModeDispatcher::RAINBOW_ID);
    modeDispatcher->startAnimatorTask(animationTask);
    LOG("App has been started!");
}

void setup()
{
    Serial.begin(115200);
    appstarter = new AppStarter();
    appstarter->setStartFunction(startApplication);

    LOG("Initialization complete!");
}

void loop() {}