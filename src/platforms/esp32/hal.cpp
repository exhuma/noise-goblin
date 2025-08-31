#include "../hal.h"
#include "../../config.h"
#include "Arduino.h"
#include "Audio.h"
#include "WiFi.h"

#define I2S_DOUT 15  // -> DIN
#define I2S_LRC 16   // -> LRC
#define I2S_BCLK 17  // -> BCLK

String ssid = WIFI_SSID;
String password = WIFI_PASSWORD;
Audio audio;

void setup() {
    Serial.begin(115200);
    Serial.println(F("Resetting WiFi"));
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    Serial.println(F("Connecting to WiFi"));
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(F("."));
        delay(1500);
    }
    Serial.println(F("Connected"));
    Serial.println(F("Connecting to audio stream"));
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(10);
    if (!audio.connecttohost("http://vis.media-ice.musicradio.com/CapitalMP3")) {
        Serial.println("Failed to connect to host");
    }
}

void loop() {
    audio.loop();
}
