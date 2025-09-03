#include "wifi_impl.hpp"
#include "WiFi.h"

void Esp32Wifi::setup() {
    logger.debug("Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
}

void Esp32Wifi::connect(const char *ssid, const char *password) {
    logger.info("Connecting to WiFi: ", ssid);
    WiFi.begin(ssid, password);
    isConnecting = true;
    remainingRetries = 10;
}

void Esp32Wifi::tick() {
    if (isConnecting && WiFi.status() != WL_CONNECTED) {
        if (remainingRetries > 0) {
            logger.info("Wifi Connecting...");
            delay(1000);  // TODO replace with nonblocking RTC wait
            remainingRetries--;
        } else if (remainingRetries == 0) {
            logger.error("Wifi Connection Failed");
            isConnecting = false;
        }
        return;
    }
}

bool Esp32Wifi::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
