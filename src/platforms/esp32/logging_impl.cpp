#include "logging_impl.hpp"
#include "Arduino.h"

void Esp32Logging::debug(const char *fmt, ...) {
    char msg[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    Serial.print("[DEBUG] ");
    Serial.print(msg);
}

void Esp32Logging::info(const char *fmt, ...) {
    char msg[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    Serial.print("[INFO ] ");
    Serial.print(msg);
}

void Esp32Logging::error(const char *fmt, ...) {
    char msg[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    Serial.print("[ERROR] ");
    Serial.print(msg);
}

void Esp32Logging::setup() {
    Serial.begin(115200);
    debug("Logging setup complete");
}
