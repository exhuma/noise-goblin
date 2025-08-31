#include "../ui.h"
#include "../config.h"
#include "../logging.h"
#include "Arduino.h"

#define RESET_BUTTON 4  // GPIO4

void setup_ui() {
    pinMode(RESET_BUTTON, INPUT_PULLUP);
}

void ui_tick() {
    char buffer[16];
    sprintf(buffer, "%d", digitalRead(RESET_BUTTON));
    if (digitalRead(RESET_BUTTON) == LOW) {
        clear_config_values();
    }
}
