#pragma once

#include "platforms/audio.hpp"
#include "platforms/config.hpp"
#include "platforms/ui.hpp"

class UiLoop {
    IUserInterface& ui_;

  public:
    explicit UiLoop(IUserInterface& ui) : ui_(ui) {
    }
    void setup(ILogging& logger) {
        ui_.setup(logger);
    }
    void tick(IAudio& audio, IConfig& config, ILogging& logger) {
        ui_.tick(audio, config, logger);
    }
};
