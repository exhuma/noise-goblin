#include "../ui.h"
#include <Adafruit_NeoPixel.h>
#include "../config.h"
#include "../logging.h"
#include "Arduino.h"

#define RESET_BUTTON 4  // GPIO4
#define RGB_LED 48
#define NUM_LEDS 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_LED, NEO_GRB + NEO_KHZ800);

void setup_ui() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);
    pinMode(RGB_LED, OUTPUT);
    strip.begin();
    strip.show();
    strip.setBrightness(50);
}
/**
 * Blink the RGB LED several times in red without blocking
 */
void blink_strip() {
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

void ui_tick() {
    char buffer[16];
    sprintf(buffer, "%d", digitalRead(RESET_BUTTON));
    if (digitalRead(RESET_BUTTON) == LOW) {
        clear_config_values();
        blink_strip();
    }
}
