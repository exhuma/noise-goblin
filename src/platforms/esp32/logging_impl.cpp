#include "logging_impl.hpp"
#include <array>
#include "Arduino.h"

void Esp32Logging::debug(const char *fmt, ...) {
    std::array<char, 128> msg;
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg.data(), msg.size(), fmt, args);
    va_end(args);
    Serial.print("[DEBUG] ");
    Serial.println(msg.data());
}

void Esp32Logging::info(const char *fmt, ...) {
    std::array<char, 128> msg;
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg.data(), msg.size(), fmt, args);
    va_end(args);
    Serial.print("[INFO ] ");
    Serial.println(msg.data());
}

void Esp32Logging::error(const char *fmt, ...) {
    std::array<char, 128> msg;
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg.data(), msg.size(), fmt, args);
    va_end(args);
    Serial.print("[ERROR] ");
    Serial.println(msg.data());
}

void Esp32Logging::setup() {
    Serial.begin(115200);
    debug("Logging setup complete");
}
