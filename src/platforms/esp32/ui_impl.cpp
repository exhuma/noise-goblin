#include <SPI.h>
#include "../../const.hpp"
#include "../../enum.hpp"
#include "../ui.hpp"
#include "Arduino.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "stdio.h"

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define RESET_BUTTON 4    // GPIO4
#define PLAY_BUTTON 5     // GPIO5
#define SCL_PIN 6
#define SDA_PIN 7
#define NUM_LEDS 1

class NoiseUi : public IUserInterface {
  public:
    NoiseUi(ILogging &logger) : IUserInterface(logger) {
    }

    void setup() override {
        pinMode(RESET_BUTTON, INPUT_PULLUP);
        pinMode(PLAY_BUTTON, INPUT_PULLUP);
        logger.debug("UI Setup complete");
    }

    auto tick(AppState state) -> std::vector<int> override {
        static unsigned long lastResetButtonPress = 0;
        static unsigned long lastPlayButtonPress = 0;
        const unsigned long debounceDelay = 200;
        unsigned long currentTime = millis();
        std::array<char, 16> buffer;
        sprintf(buffer.data(), "%d", digitalRead(RESET_BUTTON));
        std::vector<int> events = {};
        if (digitalRead(RESET_BUTTON) == LOW &&
            (currentTime - lastResetButtonPress) > debounceDelay) {
            lastResetButtonPress = currentTime;
            logger.debug("Reset button pressed");
            events.push_back(EVENT_RESET_BUTTON_PRESSED);
        }
        if (digitalRead(PLAY_BUTTON) == LOW &&
            (currentTime - lastPlayButtonPress) > debounceDelay) {
            lastPlayButtonPress = currentTime;
            logger.debug("Play button pressed");
            events.push_back(EVENT_PLAY_BUTTON_PRESSED);
        }
        return events;
    }

  private:
    std::string currentMessage;
};
