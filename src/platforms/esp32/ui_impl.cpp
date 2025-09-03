#include "ui_impl.hpp"
#include <Adafruit_NeoPixel.h>
#include "Arduino.h"

#define RESET_BUTTON 4  // GPIO4
#define PLAY_BUTTON 5   // GPIO5
#define RGB_LED 48
#define NUM_LEDS 1

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_LEDS, RGB_LED, NEO_GRB + NEO_KHZ800);

/**
 * Blink the RGB LED several times in red without blocking
 */
void _blink_strip() {
    static unsigned long lastBlinkTime = 0;
    static int blinkState = 0;
    static int blinkCount = 0;

    unsigned long currentTime = millis();

    if (blinkCount < 3) {
        if (blinkState == 0 && currentTime - lastBlinkTime >= 200) {
            strip.setPixelColor(0, strip.Color(255, 0, 0));  // Red
            strip.show();
            lastBlinkTime = currentTime;
            blinkState = 1;
        } else if (blinkState == 1 && currentTime - lastBlinkTime >= 200) {
            strip.setPixelColor(0, strip.Color(0, 0, 0));  // Off
            strip.show();
            lastBlinkTime = currentTime;
            blinkState = 0;
            blinkCount++;
        }
    } else {
        blinkCount = 0;  // Reset for the next call
    }
}

void Esp32Ui::setup() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    pinMode(PLAY_BUTTON, INPUT_PULLUP);
    pinMode(RGB_LED, OUTPUT);
    strip.begin();
    strip.show();
    strip.setBrightness(5);
    randomSeed(analogRead(0));  // Seed the random number generator
    logger.debug("UI Setup complete");
}

void Esp32Ui::tick() {
    char buffer[16];
    sprintf(buffer, "%d", digitalRead(RESET_BUTTON));
    if (digitalRead(RESET_BUTTON) == LOW) {
        logger.debug("Reset button pressed");
        eventLoop.postEvent(EVENT_RESET_BUTTON_PRESSED);
        delay(200);
    }
    if (digitalRead(PLAY_BUTTON) == LOW) {
        logger.debug("Play button pressed");
        eventLoop.postEvent(EVENT_PLAY_BUTTON_PRESSED);
        delay(200);
    }
    switch (ledState) {
    case LedState::Off:
    default:
        strip.setPixelColor(0, strip.Color(0, 0, 0));
        break;
    case LedState::CaptivePortal:
        strip.setPixelColor(0, strip.Color(0, 0, 255));
        break;
    case LedState::Reset:
        strip.setPixelColor(0, strip.Color(255, 0, 0));
        break;
    case LedState::Normal:
        strip.setPixelColor(0, strip.Color(0, 255, 0));
        break;
    case LedState::Connecting:
        strip.setPixelColor(0, strip.Color(255, 165, 0));
        break;
    }
    strip.show();
}

void Esp32Ui::setState(LedState state) {
    ledState = state;
}
