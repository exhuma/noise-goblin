#pragma once

#include "../audio.hpp"
#include "../logging.hpp"

struct Esp32Audio : IAudio {
    void setup(ILogging& logger) override;
    void play(const char* url, ILogging& logger) override;
    void tick(ILogging& logger) override;
};
