#include <Arduino.h>
#include <FastLED.h>
#include "AS5600.h"
#include "BleMouse.h"

#define NUM_LEDS 1
#define DATA_PIN 48

CRGB leds[NUM_LEDS];
AS5600L as5600;
BleMouse bleMouse;

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial up...");
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(2);
    Serial.println("LED up...");

    Wire.begin(11, 12);
    as5600.begin();
    as5600.setAddress(0x36);
    Serial.println("Mag sensor up...");

    Serial.println("Starting BLE work!");
    bleMouse.begin();
}

uint16_t prevAngle = 0;

void loop()
{
    uint16_t curAngle = as5600.readAngle();
    int16_t deltaAngle = (prevAngle - curAngle);

    if (bleMouse.isConnected())
    {
        // prevent jumps
        if (abs(deltaAngle) < 1000 && abs(deltaAngle) > 13)
        {
            //Serial.print("Angle delta: ");
            //Serial.println(deltaAngle);

            bleMouse.scroll(deltaAngle / 5);
            prevAngle = curAngle;
        }
        else if (abs(deltaAngle) > 1000)
        {
            prevAngle = curAngle;
        }

        leds[0] = CRGB::Green;
        FastLED.show();

        // ~100hz
        delay(10);
    }
    else
    {
        delay(250);
        leds[0] = CRGB::Blue;
        FastLED.show();
        delay(250);
    }
    leds[0] = CRGB::Black;
    FastLED.show();
}
