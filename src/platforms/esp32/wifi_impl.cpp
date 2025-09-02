#include "wifi_impl.hpp"
#include "WiFi.h"
#include "logging_impl.hpp"

bool Esp32Wifi::is_wifi_connected(ILogging &logger) {
    return WiFi.status() == WL_CONNECTED;
}

void Esp32Wifi::setup(const char *ssid, const char *password, ILogging &logger) {
    logger.logln("Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    logger.logln("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int retries = 0;
    while (!is_wifi_connected(logger) && retries < 10) {
        logger.log(".");
        delay(1500);
        retries++;
    }
    if (retries == 10) {
        logger.logln("Failed to Connect");
    } else {
        logger.logln("Connected");
    }
}

void Esp32Wifi::tick(ILogging &logger) {
    // no-op for ESP32
}
