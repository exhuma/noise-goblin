#include <map>
#include "../../const.hpp"
#include "../config.hpp"
#include "nvs.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "config"

static nvs_handle_t s_nvs_handle = 0;

static void _init_nvs(ILogging &logger) {
    static bool initialized = false;
    if (!initialized) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
            err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            logger.debug("NVS flash init failed, erasing...");
            nvs_flash_erase();
            nvs_flash_init();
        }
        esp_err_t open_err =
            nvs_open(NVS_NAMESPACE, NVS_READWRITE, &s_nvs_handle);
        if (open_err != ESP_OK) {
            logger.error("Failed to open NVS namespace: %s",
                         esp_err_to_name(open_err));
            return;
        }
        logger.debug("NVS initialized.");
        initialized = true;
    }
}

static auto _has_config(ILogging &logger) -> bool {
    _init_nvs(logger);
    esp_err_t err = nvs_get_str(s_nvs_handle, WIFI_SSID_KEY, nullptr, nullptr);
    return (err == ESP_OK);
}

class Esp32Config : public IConfig {
  public:
    Esp32Config(ILogging &logger, IConfigUi &configUi)
        : IConfig(logger, configUi) {
    }

    auto get(const char *key) -> std::string override {
        if (!key) {
            return {};
        }
        _init_nvs(logger);
        // First call to get required size
        size_t required_size = 0;
        esp_err_t err = nvs_get_str(s_nvs_handle, key, nullptr, &required_size);
        if (err != ESP_OK || required_size == 0) {
            // logger.error("Error reading key: %s not found, returning
            // default.", key);
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
        _init_nvs(logger);
        esp_err_t err = nvs_set_str(s_nvs_handle, key, value);
        if (err != ESP_OK) {
            logger.error(
                "Failed to set key: %s with value: %s. Error: %s(code=%d)", key,
                value, esp_err_to_name(err), err);
        }
        nvs_commit(s_nvs_handle);
    }

    void tick() override {
        if (!configUi.isRunning()) {
            configUi.start();
        }
        configUi.tick();
    }

    auto getAll() -> std::map<std::string, std::string> override {
        _init_nvs(logger);
        std::map<std::string, std::string> config;
        // Iterate over all keys and get their values
        for (const auto &key :
             {WIFI_SSID_KEY, WIFI_PASSWORD_KEY, LIBRARY_BASE_URL_KEY}) {
            config[key] = get(key);
        }
        return config;
    }

    void clear() override {
        _init_nvs(logger);
        nvs_erase_all(s_nvs_handle);
        nvs_commit(s_nvs_handle);
        logger.info("Configuration cleared.");
    }
};
