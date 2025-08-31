#include "../config.h"
#include "../logging.h"

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

static bool save_config() {
    std::lock_guard<std::mutex> lk(g_mutex);
    std::ofstream f(config_path());
    if (!f) {
        log("Failed to open config file for writing: ");
        logln(config_path().c_str());
        return false;
    }
    for (const auto &p : g_config) {
        f << p.first << "=" << p.second << "\n";
    }
    return true;
}

void get_config_value(const char *key, char *out_value, size_t out_size) {
    if (!key || !out_value || out_size == 0) {
        return;
    }
    load_config();
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_config.find(key);
    if (it == g_config.end()) {
        return;
    }
    strncpy(out_value, it->second.c_str(), out_size);
    out_value[out_size - 1] = '\0';  // Ensure null-termination
}

const void *set_config_value(const char *key, const char *value) {
    if (!key)
        return nullptr;
    load_config();
    {
        std::lock_guard<std::mutex> lk(g_mutex);
        if (value)
            g_config[std::string(key)] = std::string(value);
        else
            g_config.erase(key);
    }
    save_config();
    return nullptr;
}

void request_config() {
    std::string ssid;
    std::string password;
    std::cout << "Enter WiFi SSID: ";
    std::getline(std::cin, ssid);
    std::cout << "Enter WiFi Password: ";
    std::getline(std::cin, password);
    set_config_value("wifi_ssid", ssid.c_str());
    set_config_value("wifi_password", password.c_str());
}

bool config_tick() {
    request_config();  // <- blocking
    return true;
}

void clear_config_values() {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_config.clear();
    save_config();
    logln("Configuration cleared.");
}
