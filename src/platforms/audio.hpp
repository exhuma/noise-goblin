
#pragma once
#include <string>

struct IAudio {
    virtual ~IAudio() = default;
    virtual void setup() = 0;
    virtual void play(std::string url) = 0;
    virtual void tick() = 0;
};
