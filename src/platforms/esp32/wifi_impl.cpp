#include "wifi_impl.hpp"
#include "WiFi.h"

bool Esp32Wifi::is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void Esp32Wifi::setup(const char *ssid, const char *password) {
    logger.info("Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    logger.info("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int retries = 0;
    while (!is_wifi_connected() && retries < 10) {
        logger.info(".");
        delay(1500);
        retries++;
    }
    if (retries == 10) {
        logger.error("Failed to Connect");
    } else {
        logger.info("Connected");
    }
}

void Esp32Wifi::tick() {
    // no-op for ESP32
}
