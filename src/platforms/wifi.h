
#pragma once

const char *get_wifi_ssid();
const char *get_wifi_password();
bool is_wifi_connected();
void connect_wifi(const char *ssid, const char *password);
