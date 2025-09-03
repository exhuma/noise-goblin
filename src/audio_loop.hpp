#pragma once

#include "platforms/audio.hpp"

class AudioLoop {
    IAudio& audio_;

  public:
    explicit AudioLoop(IAudio& audio) : audio_(audio) {
    }
    void setup() {
        audio_.setup();
    }
    void play(const char* url) {
        audio_.play(url);
    }
    void tick() {
        audio_.tick();
    }
};
