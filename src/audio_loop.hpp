#pragma once

#include "platforms/audio.hpp"

class AudioLoop {
    IAudio& audio_;

  public:
    explicit AudioLoop(IAudio& audio) : audio_(audio) {
    }
    void setup(ILogging& logger) {
        audio_.setup(logger);
    }
    void play(const char* url, ILogging& logger) {
        audio_.play(url, logger);
    }
    void tick(ILogging& logger) {
        audio_.tick(logger);
    }
};
