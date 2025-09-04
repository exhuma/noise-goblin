#include "config_impl.hpp"
#include "../logging.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>

static auto config_path() -> std::string {
    const char *home = std::getenv("HOME");
    if (!home)
        home = ".";
    return std::string(home) + "/.noise-goblin.conf";
}

static std::map<std::string, std::string> g_config;
static bool g_loaded = false;
static std::mutex g_mutex;

static inline auto trim(const std::string &s) -> std::string {
    const char *ws = " \t\r\n";
    auto b = s.find_first_not_of(ws);
    if (b == std::string::npos)
        return {};
    auto e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

static void load_config() {
    std::lock_guard<std::mutex> lk(g_mutex);
    if (g_loaded)
        return;
    std::ifstream f(config_path());
    if (!f) {
        g_loaded = true;
        return;
    }
    std::string line;
    while (std::getline(f, line)) {
        // strip comments
        auto posc = line.find_first_of("#;");
        if (posc != std::string::npos)
            line = line.substr(0, posc);
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));
        if (!key.empty())
            g_config[key] = val;
    }
    g_loaded = true;
}

static auto save_config(ILogging &logger) -> bool {
    // Assume caller holds the lock; do not lock here
    std::ofstream f(config_path());
    if (!f) {
        logger.error("Failed to open config file for writing: ",
                     config_path().c_str());
        return false;
    }
    for (const auto &p : g_config) {
        f << p.first << "=" << p.second << "\n";
    }
    return true;
}

void request_config(IConfig &config) {
    std::string ssid;
    std::string password;
    std::string library_base_url;
    std::cout << "Enter WiFi SSID: ";
    std::getline(std::cin, ssid);
    std::cout << "Enter WiFi Password: ";
    std::getline(std::cin, password);
    std::cout << "Enter Library Base URL: ";
    std::getline(std::cin, library_base_url);
    config.set(WIFI_SSID_KEY, ssid.c_str());
    config.set(WIFI_PASSWORD_KEY, password.c_str());
    config.set(LIBRARY_BASE_URL_KEY, library_base_url.c_str());
}

auto PosixConfig::get(const char *key) -> std::string {
    if (!key) {
        return {};
    }
    load_config();
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_config.find(key);
    if (it == g_config.end()) {
        return {};
    }
    return it->second;
}

void PosixConfig::set(const char *key, const char *value) {
    if (!key)
        return;
    load_config();
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        if (value)
            g_config[std::string(key)] = std::string(value);
        else
            g_config.erase(key);
    }
    save_config(logger);
}

auto PosixConfig::tick() -> bool {
    request_config(*this);  // <- blocking
    return true;
}

void PosixConfig::clear() {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_config.clear();
    save_config(logger);
    logger.info("Configuration cleared.");
}
