#pragma once

#include <stdio.h>
#include <unistd.h>
#include "../audio.hpp"
#include "../logging.hpp"

struct PosixAudio : IAudio {
    void setup(ILogging &logger) override;
    void play(const char *url, ILogging &logger) override;
    void tick(ILogging &logger) override;
};
