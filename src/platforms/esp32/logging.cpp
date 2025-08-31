#include "../logging.h"
#include "Arduino.h"

void log(const char *msg) {
    Serial.print(msg);
}

void logln(const char *msg) {
    Serial.println(msg);
}

void setup_hardware() {
    Serial.begin(115200);
}
