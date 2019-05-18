#pragma once

#include <HardwareSerial.h>
#define LOG(message) Serial.printf("%s: %s\n", __func__ ,message);
#define LOGF(...) \
    Serial.printf("%s: ", __func__); \
    Serial.printf(__VA_ARGS__); \
    Serial.println("");
            
                