#pragma once

#include "../audio.hpp"
#include "../logging.hpp"

struct Esp32Audio : IAudio {
    explicit Esp32Audio(ILogging& logger) : logger(logger) {
    }
    void setup() override;
    void play(const char* url) override;
    void tick() override;

  private:
    ILogging& logger;
};
