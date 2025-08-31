#include "../wifi.h"
#include "../logging.h"

bool is_wifi_connected() {
    logln("is_wifi_connected called");
    return true;
}

void connect_wifi(const char *ssid, const char *password) {
    log("connect_wifi called with ssid: ");
    logln(ssid);
    log("connect_wifi called with password: ");
    logln(password);
}
