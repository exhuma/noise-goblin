#include "wifi_impl.hpp"

void PosixWifi::setup() {
    logger.info("Resetting WiFi");
}

void PosixWifi::connect(const char *ssid, const char *password) {
    logger.info("Connecting to WiFi with SSID %s using password %s", ssid,
                password);
}

void PosixWifi::tick() {
    logger.info("PosixWifi tick called %d times", tickCount);
    tickCount++;
}

auto PosixWifi::isConnected() -> bool {
    // Simulate a connection that interrupts for a few seconds.
    if (tickCount <= 5) {
        return false;
    }
    if (tickCount <= 10) {
        return true;
    }
    if (tickCount <= 15) {
        return false;
    }
    return true;
}
