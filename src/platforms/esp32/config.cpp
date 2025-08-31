#include "../config.h"
#include "../logging.h"
#include "./captive_portal.h"
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "config"

static nvs_handle_t s_nvs_handle = 0;

void _init_nvs() {
    static bool initialized = false;
    if (!initialized) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            nvs_flash_erase();
            nvs_flash_init();
        }
        nvs_open(NVS_NAMESPACE, NVS_READWRITE, &s_nvs_handle);
        initialized = true;
    }
}

const char *get_config_value(const char *key) {
    _init_nvs();
    static char value[128];
    size_t required_size = sizeof(value);
    esp_err_t err = nvs_get_str(s_nvs_handle, key, value, &required_size);
    log("get_config_value called with key: ");
    logln(key);
    if (err == ESP_OK) {
        return value;
    } else {
        logln("Key not found, returning default.");
        return "";
    }
}

const void *set_config_value(const char *key, const char *value) {
    _init_nvs();
    log("set_config_value called with key: ");
    logln(key);
    log("and value: ");
    logln(value);
    esp_err_t err = nvs_set_str(s_nvs_handle, key, value);
    nvs_commit(s_nvs_handle);
    return nullptr;
}

bool has_config() {
    _init_nvs();
    esp_err_t err = nvs_get_str(s_nvs_handle, "wifi_ssid", nullptr, nullptr);
    return (err == ESP_OK);
}

bool config_tick() {
    logln("config_tick called");
    if (has_config()) {
        return true;
    }
    start_captive_portal();
    return false;
}
