#include <Adafruit_NeoPixel.h>
#include "../../enum.hpp"
#include "../ui.hpp"
#include "Arduino.h"

#define RESET_BUTTON 4  // GPIO4
#define PLAY_BUTTON 5   // GPIO5
#define RGB_LED 48
#define NUM_LEDS 1

static Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_LEDS, RGB_LED, NEO_GRB + NEO_KHZ800);

/**
 * Blink the RGB LED several times in red without blocking
 */
static void _blink_strip() {
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

class Esp32Ui : public IUserInterface {
  public:
    Esp32Ui(ILogging &logger, IEventLoop &eventLoop)
        : IUserInterface(logger, eventLoop) {
    }

    void setup() override {
        pinMode(RESET_BUTTON, INPUT_PULLUP);
        pinMode(PLAY_BUTTON, INPUT_PULLUP);
        pinMode(RGB_LED, OUTPUT);
        strip.begin();
        strip.show();
        strip.setBrightness(5);
        logger.debug("UI Setup complete");
    }

    void tick(AppState state) override {
        static unsigned long lastResetButtonPress = 0;
        static unsigned long lastPlayButtonPress = 0;
        const unsigned long debounceDelay = 200;
        unsigned long currentTime = millis();
        std::array<char, 16> buffer;
        sprintf(buffer.data(), "%d", digitalRead(RESET_BUTTON));
        if (digitalRead(RESET_BUTTON) == LOW &&
            (currentTime - lastResetButtonPress) > debounceDelay) {
            lastResetButtonPress = currentTime;
            logger.debug("Reset button pressed");
            eventLoop.postEvent(EVENT_RESET_BUTTON_PRESSED);
        }
        if (digitalRead(PLAY_BUTTON) == LOW &&
            (currentTime - lastPlayButtonPress) > debounceDelay) {
            lastPlayButtonPress = currentTime;
            logger.debug("Play button pressed");
            eventLoop.postEvent(EVENT_PLAY_BUTTON_PRESSED);
        }
        switch (state) {
        case AppState::RequestingConfig:
            strip.setPixelColor(0, strip.Color(0, 0, 255));
            break;
        case AppState::Normal:
            strip.setPixelColor(0, strip.Color(0, 255, 0));
            break;
        case AppState::Connecting:
            strip.setPixelColor(0, strip.Color(255, 165, 0));
            break;
        }
        strip.show();
    }
};
