#include "../wifi.h"
#include "../logging.h"
#include "../wifi.h"
#include "WiFi.h"

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}

void connect_wifi(const char *ssid, const char *password) {
    logln("Resetting WiFi");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    logln("Connecting to WiFi");
    WiFi.begin(ssid, password);
    int retries = 0;
    while (!is_wifi_connected() && retries < 10) {
        log(".");
        delay(1500);
        retries++;
    }
    if (retries == 10) {
        logln("Failed to Connect");
    } else {
        logln("Connected");
    }
}
