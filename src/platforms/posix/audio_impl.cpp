#include "audio_impl.hpp"
#include <stdio.h>
#include <unistd.h>

void PosixAudio::setup() {
    logger.logln("PosixAudio setup called");
}

void PosixAudio::play(const char *url) {
    logger.log("PosixAudio play called with url: ");
    logger.logln(url);
}

void PosixAudio::tick() {
    logger.logln("PosixAudio tick called");
    sleep(1);
}
