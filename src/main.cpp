#include <Arduino.h>
#include <FastLED.h>
#include "AS5600.h"
#include "BleMouse.h"

#if defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#else
#include "esp_log.h"
#endif

#define NUM_LEDS 1
#define DATA_PIN 48

CRGB leds[NUM_LEDS];
AS5600L as5600;
BleMouse bleMouse;

void setup()
{
    ESP_LOGI("####", "Setup...");
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(2);
    ESP_LOGI("####", "LED up...");

    Wire.begin(11, 12);
    as5600.begin();
    as5600.setAddress(0x36);
    ESP_LOGI("####", "Mag sensor up...");

    ESP_LOGI("####", "Starting BLE work!");
    bleMouse.begin();
}

uint16_t prevAngle = 0;
int angleIndex = 0;
int16_t prevAngles[100];

int16_t getAvg()
{
    if (angleIndex < 1)
        return 0;

    int16_t avg = 0;
    for (size_t i = 0; i < angleIndex; i++)
    {
        avg += prevAngles[i];
    }

    avg /= angleIndex;
    return avg;
}

void loop()
{
    uint16_t curAngle = as5600.readAngle();
    int16_t deltaAngle = (prevAngle - curAngle);

    // avg will be used to prevent noise scrolling and if real scrolling happens we can have smaller detection
    // maybe switch to time based method and rolling/moving window, as there is still some improvement potential
    if (angleIndex >= 99)
    {
        angleIndex = 0;
    }
    prevAngles[angleIndex++] = deltaAngle;

    if (bleMouse.isConnected())
    {
        int16_t avgAngle = getAvg();
        // prevent noise jumps
        if (abs(deltaAngle) < 1000 && abs(deltaAngle) > abs(avgAngle) + 8)
        {
            //ESP_LOGI("####", "Angle delta: %d", deltaAngle);

            // adjust denominator to adjust speed, adjust report descriptor if more speed is required
            // possibly we could consider acceleration and allow smooth slow and fast accelerated scrolling
            // some experimentation todo

            // clamp to descriptor
            bleMouse.scroll(min(max(deltaAngle / 3, -127), 127));
            prevAngle = curAngle;
        }
        else if (abs(deltaAngle) > 1000)
        {
            prevAngle = curAngle;
        }

        // connected = green
        leds[0] = CRGB::Green;
        FastLED.show();

        // ~100hz
        delay(10);
    }
    else
    {
        delay(250);
        // disconnected = blink blue
        leds[0] = CRGB::Blue;
        FastLED.show();
        delay(250);
    }
    leds[0] = CRGB::Black;
    FastLED.show();
}
