
#pragma once

#include "logging.hpp"

struct IAudio {
    virtual ~IAudio() = default;
    virtual void setup(ILogging& logger) = 0;
    virtual void play(const char* url, ILogging& logger) = 0;
    virtual void tick(ILogging& logger) = 0;
};
