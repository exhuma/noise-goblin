#include "wifi_impl.hpp"

bool PosixWifi::is_wifi_connected() {
    logger.logln("is_wifi_connected called");
    return true;
}

void PosixWifi::setup(const char *ssid, const char *password) {
    logger.log("connect_wifi called with ssid: ");
    logger.logln(ssid);
    logger.log("connect_wifi called with password: ");
    logger.logln(password);
}

void PosixWifi::tick() {
    logger.logln("PosixWifi tick called");
}
