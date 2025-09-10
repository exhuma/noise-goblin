#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
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
#define RGB_LED 48
#define NUM_LEDS 1

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
static Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(NUM_LEDS, RGB_LED, NEO_GRB + NEO_KHZ800);

static void displayMessage(Adafruit_SSD1306 &display, std::string message) {
    display.clearDisplay();
    display.setTextSize(1);       // Normal 1:1 pixel scale
    display.setTextColor(WHITE);  // Draw white text
    display.setCursor(0, 0);      // Start at top-left corner
    display.cp437(true);          // Use full 256 char 'Code Page 437' font
    display.println(message.c_str());
    display.display();
}

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

static std::string niceSize(unsigned size) {
    static char buffer[20];
    if (size < 1024) {
        sprintf(buffer, "%uB", size);
    } else if (size < 1024 * 1024) {
        sprintf(buffer, "%.2fKB", size / 1024.0);
    } else {
        sprintf(buffer, "%.2fMB", size / (1024.0 * 1024.0));
    }
    return std::string(buffer);
}

static std::string progressBarText(unsigned progress, unsigned width) {
    unsigned pos = width * progress / 100;
    std::string bar =
        "[" + std::string(pos, '#') + std::string(width - pos, '-') + "]";
    return bar;
}

static void drawProgressBar(unsigned progress, unsigned int posX,
                            unsigned int postY, unsigned int width,
                            unsigned int height) {
    display.drawRect(posX, postY + 1, width, height - 2, WHITE);
    display.fillRect(posX + 1, postY + 1 + 1, (width - 2) * progress / 100,
                     height - 4, WHITE);
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

        Wire.begin(SDA_PIN, SCL_PIN);
        // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
            logger.error("SSD1306 allocation failed");
        }
        display.clearDisplay();

        logger.debug("UI Setup complete");
    }

    void updateMemStats(AppState state) {
        if (millis() - lastMessageUpdate < 20) {
            return;
        }
        lastMessageUpdate = millis();
        size_t free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        size_t min_free_heap = esp_get_minimum_free_heap_size();
        size_t total_heap = heap_caps_get_total_size(MALLOC_CAP_8BIT);
        size_t largest_block =
            heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);

        float fragmentation =
            (1.0f - (float)largest_block / (float)free_heap) * 100.0f;
        float heapPct =
            (float)(total_heap - free_heap) / (float)total_heap * 100.0f;

        display.clearDisplay();
        display.setTextSize(1);       // Normal 1:1 pixel scale
        display.setTextColor(WHITE);  // Draw white text
        display.setCursor(0, 0);      // Start at top-left corner
        display.cp437(true);          // Use full 256 char 'Code Page 437' font
        display.println("=== Memory ===");
        // [Text Display] display.printf("T. heap:     %s\n",
        // [Text Display] niceSize((unsigned)total_heap).c_str());
        // display.printf("F. heap: [Text Display] %s\n",
        // niceSize((unsigned)free_heap).c_str());
        display.printf("Frag.:\n");
        drawProgressBar(fragmentation, 50, 8, 128 - 50, 8);
        display.printf("Heap:\n");
        drawProgressBar(heapPct, 50, 16, 128 - 50, 8);
        display.printf("Min f. heap: %s (since boot)\n",
                       niceSize((unsigned)min_free_heap).c_str());
        display.printf("Lg block:    %s\n",
                       niceSize((unsigned)largest_block).c_str());
        display.display();
    }

    void updateLed(AppState state) {
        if (millis() - lastLedUpdate < 500) {
            return;
        }
        lastLedUpdate = millis();
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
        updateLed(state);
        updateMemStats(state);
    }

  private:
    std::string currentMessage;
    int loops = 0;
    unsigned long lastLoopUpdate = 0;
    unsigned long lastMessageUpdate = 0;
    unsigned long lastLedUpdate = 0;
};
