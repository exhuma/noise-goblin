#pragma once

#include <stdio.h>
#include <unistd.h>
#include "../audio.hpp"
#include "../logging.hpp"

struct PosixAudio : IAudio {
    explicit PosixAudio(ILogging &logger) : logger(logger) {
    }
    void setup() override;
    void play(const char *url) override;
    void tick() override;

  private:
    ILogging &logger;
};
