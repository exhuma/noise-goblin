#include <SPI.h>
#include "../../const.hpp"
#include "../../enum.hpp"
#include "../ui.hpp"
#include "Arduino.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "stdio.h"

#define PLAY_BUTTON 6  // GPIO6
#define USER_LED 21

class NoiseUi : public IUserInterface {
  public:
    NoiseUi(ILogging &logger) : IUserInterface(logger) {
    }

    void setup() override {
        pinMode(PLAY_BUTTON, INPUT_PULLUP);
        pinMode(USER_LED, OUTPUT);
        digitalWrite(USER_LED, HIGH);  // turn off user-led (inverted logic)
        logger.debug("XIAO ESP32S3 UI Setup complete");
    }

    auto tick(AppState state) -> std::vector<int> override {
        switch (state) {
        case AppState::RequestingConfig: {
            unsigned long currentTime = millis();
            if (currentTime - lastLightFlashTime >=
                lightFlashDurationRequestingConfig) {
                lastLightFlashTime = currentTime;
                // Toggle the LED state
                int ledState = digitalRead(USER_LED);
                digitalWrite(USER_LED, !ledState);  // Inverted logic
            }
            break;
        }
        case AppState::Normal: {
            unsigned long currentTime = millis();
            if (currentTime - lastLightFlashTime >= lightFlashDurationNormal) {
                lastLightFlashTime = currentTime;
                // Toggle the LED state
                int ledState = digitalRead(USER_LED);
                digitalWrite(USER_LED, !ledState);  // Inverted logic
            }
            break;
        }
        default: {
            // Handle other states
            break;
        }
        }

        std::vector<int> events = {};
        static unsigned long lastResetButtonPress = 0;
        static unsigned long lastPlayButtonPress = 0;
        const unsigned long debounceDelay = 200;
        unsigned long currentTime = millis();
        // if (digitalRead(RESET_BUTTON) == LOW &&
        //     (currentTime - lastResetButtonPress) > debounceDelay) {
        //     lastResetButtonPress = currentTime;
        //     logger.debug("Reset button pressed");
        //     events.push_back(EVENT_RESET_BUTTON_PRESSED);
        // }
        if (digitalRead(PLAY_BUTTON) == LOW &&
            (currentTime - lastPlayButtonPress) > debounceDelay) {
            lastPlayButtonPress = currentTime;
            logger.debug("Play button pressed");
            events.push_back(EVENT_PLAY_BUTTON_PRESSED);
        }
        return events;
    }

  private:
    unsigned long lastLightFlashTime = 0;
    const unsigned long lightFlashDurationRequestingConfig =
        100;                                              // milliseconds
    const unsigned long lightFlashDurationNormal = 1000;  // milliseconds
};
