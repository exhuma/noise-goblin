#include "../wifi.h"
#include "../../config.h"
#include "../logging.h"
#include "../wifi.h"
#include "WiFi.h"

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

const char *get_wifi_ssid() {
    return WIFI_SSID;
}

const char *get_wifi_password() {
    return WIFI_PASSWORD;
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
