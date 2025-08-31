#include "../hal.h"
#include "../../config.h"
#include "Arduino.h"
#include "Audio.h"
#include "WiFi.h"

Audio audio;

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

const char *get_wifi_ssid() {
    return WIFI_SSID;
}

const char *get_wifi_password() {
    return WIFI_PASSWORD;
}

void log(const char *msg) {
    Serial.print(msg);
}

void logln(const char *msg) {
    Serial.println(msg);
}

void connect_wifi(const char *ssid, const char *password) {
    logln("Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    logln("Connecting to WiFi");
    WiFi.begin(ssid, password);
    while (!is_wifi_connected()) {
        log(".");
        delay(1500);
    }
    logln("Connected");
}

void setup_hardware() {
    Serial.begin(115200);
    audio.setPinout(17,  // I2S_BCLK
                    16,  // I2S_LRC
                    15   // I2S_DOUT
    );
    audio.setVolume(10);
}

void play_audio(const char *url) {
    logln("Connecting to audio stream");
    if (!audio.connecttohost(url)) {
        logln("Failed to connect to host");
    }
}

void audio_tick() {
    audio.loop();
}
