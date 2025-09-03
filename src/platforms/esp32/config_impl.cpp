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

std::string Esp32Config::get(const char *key) {
    if (!key) {
        return std::string();
    }
    _init_nvs();
    // First call to get required size
    size_t required_size = 0;
    esp_err_t err = nvs_get_str(s_nvs_handle, key, nullptr, &required_size);
    if (err != ESP_OK || required_size == 0) {
        logger.error("Error reading key: ", key,
                     " not found, returning default.");
        return std::string();
    }
    std::string value;
    value.resize(required_size);
    err = nvs_get_str(s_nvs_handle, key, &value[0], &required_size);
    if (err != ESP_OK) {
        logger.error("Error reading value after allocating buffer");
        return std::string();
    }
    // nvs_get_str writes the null terminator; shrink to actual length
    if (!value.empty() && value.back() == '\0')
        value.pop_back();
    return value;
}

void Esp32Config::set(const char *key, const char *value) {
    _init_nvs();
    logger.debug("set_config_value called with key: ", key,
                 "and value: ", value);
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
    logger.info("Configuration cleared.");
}
