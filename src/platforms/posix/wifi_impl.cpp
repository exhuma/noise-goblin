#include "wifi_impl.hpp"
#include "../logging.hpp"

bool PosixWifi::is_wifi_connected(ILogging &logger) {
    logger.logln("is_wifi_connected called");
    return true;
}

void PosixWifi::setup(const char *ssid, const char *password, ILogging &logger) {
    logger.log("connect_wifi called with ssid: ");
    logger.logln(ssid);
    logger.log("connect_wifi called with password: ");
    logger.logln(password);
}

void PosixWifi::tick(ILogging &logger) {
    logger.logln("PosixWifi tick called");
}
