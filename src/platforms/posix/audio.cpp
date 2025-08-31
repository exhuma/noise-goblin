#include "../audio.h"
#include <stdio.h>
#include <unistd.h>
#include "../logging.h"

void setup_audio() {
    logln("setup_audio called");
}

void play_audio(const char *url) {
    log("play_audio called with url: ");
    logln(url);
}

void audio_tick() {
    logln("Noise goblin loop (PC)");
    sleep(1);
}
