#pragma once

/// @brief WiFi SSID configuration key
static const char *WIFI_SSID_KEY = "wifi_ssid";

/// @brief WiFi password configuration key
static const char *WIFI_PASSWORD_KEY = "wifi_password";

/// @brief Library base URL configuration key
static const char *LIBRARY_BASE_URL_KEY = "lib_base_url";

/// @brief Play button pressed event
inline const int EVENT_PLAY_BUTTON_PRESSED = 1;

/// @brief Reset button pressed event
inline const int EVENT_RESET_BUTTON_PRESSED = 2;

/// @brief A sound has started playing
inline const int SOUND_PLAY_STARTED = 3;

/// @brief A sound has stopped playing
inline const int SOUND_PLAY_STOPPED = 4;

/// @brief A new network connection was requested
inline const int EVENT_WIFI_CONNECTING = 5;

/// @brief A new network connection was established
inline const int EVENT_WIFI_CONNECTED = 6;

/// @brief Network connection failed
inline const int EVENT_WIFI_FAILED = 7;
