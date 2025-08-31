
#pragma once

const char *get_wifi_ssid();
const char *get_wifi_password();
bool is_wifi_connected();
void log(const char *msg);
void logln(const char *msg);
void connect_wifi(const char *ssid, const char *password);
void setup_hardware();
void play_audio(const char *url);
void audio_tick();
