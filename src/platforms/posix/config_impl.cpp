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

// Simple, zero-dependency key=value config for POSIX.
// Stores per-user config in $HOME/.noise-goblin.conf

static std::string config_path() {
    const char *home = std::getenv("HOME");
    if (!home)
        home = ".";
    return std::string(home) + "/.noise-goblin.conf";
}

static std::map<std::string, std::string> g_config;
static bool g_loaded = false;
static std::mutex g_mutex;

static inline std::string trim(const std::string &s) {
    const char *ws = " \t\r\n";
    auto b = s.find_first_not_of(ws);
    if (b == std::string::npos)
        return std::string();
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

static bool save_config(ILogging &logger) {
    std::lock_guard<std::mutex> lk(g_mutex);
    std::ofstream f(config_path());
    if (!f) {
        logger.log("Failed to open config file for writing: ");
        logger.logln(config_path().c_str());
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
    std::cout << "Enter WiFi SSID: ";
    std::getline(std::cin, ssid);
    std::cout << "Enter WiFi Password: ";
    std::getline(std::cin, password);
    config.set("wifi_ssid", ssid.c_str());
    config.set("wifi_password", password.c_str());
}

std::string PosixConfig::get(const char *key) {
    if (!key) {
        return std::string();
    }
    load_config();
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_config.find(key);
    if (it == g_config.end()) {
        return std::string();
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

bool PosixConfig::tick() {
    request_config(*this);  // <- blocking
    return true;
}

void PosixConfig::clear() {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_config.clear();
    save_config(logger);
    logger.logln("Configuration cleared.");
}
