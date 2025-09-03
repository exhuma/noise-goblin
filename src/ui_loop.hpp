#pragma once

#include "platforms/ui.hpp"

class UiLoop {
    IUserInterface& ui_;

  public:
    explicit UiLoop(IUserInterface& ui) : ui_(ui) {
    }
    void setup() {
        ui_.setup();
    }
    void tick() {
        ui_.tick();
    }
};
