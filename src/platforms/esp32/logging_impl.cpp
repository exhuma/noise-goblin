#include <array>
#include "../logging.hpp"
#include "Arduino.h"

#define MAX_LOG_SIZE 512

class Esp32Logging : public ILogging {
    void debug(const char *fmt, ...) override {
        std::array<char, MAX_LOG_SIZE> msg;
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg.data(), msg.size(), fmt, args);
        va_end(args);
        Serial.print("[DEBUG] ");
        Serial.println(msg.data());
    }

    void info(const char *fmt, ...) override {
        std::array<char, MAX_LOG_SIZE> msg;
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg.data(), msg.size(), fmt, args);
        va_end(args);
        Serial.print("[INFO ] ");
        Serial.println(msg.data());
    }

    void error(const char *fmt, ...) override {
        std::array<char, MAX_LOG_SIZE> msg;
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg.data(), msg.size(), fmt, args);
        va_end(args);
        Serial.print("[ERROR] ");
        Serial.println(msg.data());
    }

    void setup() override {
        Serial.begin(115200);
        debug("Logging setup complete");
    }
};
