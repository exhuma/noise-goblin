#include "wifi_impl.hpp"

bool PosixWifi::is_wifi_connected() {
    logger.info("is_wifi_connected called");
    return true;
}

void PosixWifi::setup(const char *ssid, const char *password) {
    logger.info("connect_wifi called with ssid: ", ssid);
    logger.info("connect_wifi called with password: ", password);
}

void PosixWifi::tick() {
    logger.info("PosixWifi tick called");
}
