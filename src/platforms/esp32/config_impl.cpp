#include "config_impl.hpp"
#include "./captive_portal.h"
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "config"

static nvs_handle_t s_nvs_handle = 0;

void _init_nvs() {
    static bool initialized = false;
    if (!initialized) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
            err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            nvs_flash_erase();
            nvs_flash_init();
        }
        nvs_open(NVS_NAMESPACE, NVS_READWRITE, &s_nvs_handle);
        initialized = true;
    }
}

bool _has_config() {
    _init_nvs();
    esp_err_t err = nvs_get_str(s_nvs_handle, "wifi_ssid", nullptr, nullptr);
    return (err == ESP_OK);
}

void Esp32Config::get(const char *key, char *out_value, size_t out_size) {
    _init_nvs();
    size_t required_size = out_size;
    esp_err_t err = nvs_get_str(s_nvs_handle, key, out_value, &required_size);
    if (err != ESP_OK) {
        logger.logln("Error reading value");
        logger.log("Key ");
        logger.logln(key);
        logger.logln(" not found, returning default.");
        if (out_size > 0) {
            out_value[0] = '\0';  // Ensure the output is an empty string
        }
    }
}

void Esp32Config::set(const char *key, const char *value) {
    _init_nvs();
    logger.log("set_config_value called with key: ");
    logger.logln(key);
    logger.log("and value: ");
    logger.logln(value);
    esp_err_t err = nvs_set_str(s_nvs_handle, key, value);
    nvs_commit(s_nvs_handle);
}

bool Esp32Config::tick() {
    if (_has_config()) {
        return true;
    }
    start_captive_portal(*this, logger);
    process_request(logger);
    return false;
}

void Esp32Config::clear() {
    _init_nvs();
    nvs_erase_all(s_nvs_handle);
    nvs_commit(s_nvs_handle);
    logger.logln("Configuration cleared.");
}
