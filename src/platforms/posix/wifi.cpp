#include "../wifi.h"
#include <stdio.h>
#include <unistd.h>
#include "../logging.h"

const char *get_wifi_ssid() {
    log("get_wifi_ssid called");
    return "dummy_ssid";
}

const char *get_wifi_password() {
    log("get_wifi_password called");
    return "dummy_password";
}

bool is_wifi_connected() {
    log("is_wifi_connected called");
    return true;
}

void connect_wifi(const char *ssid, const char *password) {
    log("connect_wifi called with ssid: ");
    logln(ssid);
    log("connect_wifi called with password: ");
    logln(password);
}
