#include "../hal.h"
#include <stdio.h>
#include <unistd.h>

const char *get_wifi_ssid() {
    printf("get_wifi_ssid called\n");
    return "dummy_ssid";
}

const char *get_wifi_password() {
    printf("get_wifi_password called\n");
    return "dummy_password";
}

bool is_wifi_connected() {
    printf("is_wifi_connected called\n");
    return true;
}

void log(const char *msg) {
    printf("log: %s", msg);
}

void logln(const char *msg) {
    printf("logln: %s\n", msg);
}

void connect_wifi(const char *ssid, const char *password) {
    printf("connect_wifi called with ssid: %s, password: %s\n", ssid, password);
}

void setup_hardware() {
    printf("setup_hardware called\n");
}

void play_audio(const char *url) {
    printf("play_audio called with url: %s\n", url);
}

void audio_tick() {
    logln("Noise goblin loop (PC)");
    sleep(1);
}
