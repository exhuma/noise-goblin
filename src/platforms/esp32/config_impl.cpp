#include "../config.hpp"
#include "./captive_portal.h"
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "config"

static nvs_handle_t s_nvs_handle = 0;

static void _init_nvs() {
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

static auto _has_config() -> bool {
    _init_nvs();
    esp_err_t err = nvs_get_str(s_nvs_handle, WIFI_SSID_KEY, nullptr, nullptr);
    return (err == ESP_OK);
}

class Esp32Config : public IConfig {
  public:
    Esp32Config(ILogging &logger) : IConfig(logger) {
    }
    auto get(const char *key) -> std::string override {
        if (!key) {
            return {};
        }
        _init_nvs();
        // First call to get required size
        size_t required_size = 0;
        esp_err_t err = nvs_get_str(s_nvs_handle, key, nullptr, &required_size);
        if (err != ESP_OK || required_size == 0) {
            logger.error("Error reading key: ", key,
                         " not found, returning default.");
            return {};
        }
        std::string value;
        value.resize(required_size);
        err = nvs_get_str(s_nvs_handle, key, &value[0], &required_size);
        if (err != ESP_OK) {
            logger.error("Error reading value after allocating buffer");
            return {};
        }
        // nvs_get_str writes the null terminator; shrink to actual length
        if (!value.empty() && value.back() == '\0')
            value.pop_back();
        return value;
    }

    void set(const char *key, const char *value) override {
        _init_nvs();
        logger.debug("set_config_value called with key: ", key,
                     "and value: ", value);
        esp_err_t err = nvs_set_str(s_nvs_handle, key, value);
        nvs_commit(s_nvs_handle);
    }

    auto tick() -> bool override {
        if (_has_config()) {
            return true;
        }
        start_captive_portal(*this, logger);
        process_request(logger);
        return false;
    }

    void clear() override {
        _init_nvs();
        nvs_erase_all(s_nvs_handle);
        nvs_commit(s_nvs_handle);
        logger.info("Configuration cleared.");
    }
};
