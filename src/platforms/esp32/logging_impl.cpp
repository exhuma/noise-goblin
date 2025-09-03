#include "logging_impl.hpp"
#include "Arduino.h"

void Esp32Logging::log(const char *msg) {
    Serial.print(msg);
}

void Esp32Logging::logln(const char *msg) {
    Serial.println(msg);
}

void Esp32Logging::setup() {
    Serial.begin(115200);
}
