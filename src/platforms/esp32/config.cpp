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

void get_config_value(const char *key, char *out_value, size_t out_size) {
    _init_nvs();
    size_t required_size = out_size;
    esp_err_t err = nvs_get_str(s_nvs_handle, key, out_value, &required_size);
    if (err != ESP_OK) {
        logln("Error reading value");
        log("Key ");
        logln(key);
        logln(" not found, returning default.");
        if (out_size > 0) {
            out_value[0] = '\0';  // Ensure the output is an empty string
        }
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
    if (has_config()) {
        return true;
    }
    start_captive_portal();
    process_request();
    return false;
}

void clear_config_values() {
    _init_nvs();
    nvs_erase_all(s_nvs_handle);
    nvs_commit(s_nvs_handle);
    logln("Configuration cleared.");
}
