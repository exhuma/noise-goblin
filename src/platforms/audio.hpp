
#pragma once

struct IAudio {
    virtual ~IAudio() = default;
    virtual void setup() = 0;
    virtual void play(const char* url) = 0;
    virtual void tick() = 0;
};
