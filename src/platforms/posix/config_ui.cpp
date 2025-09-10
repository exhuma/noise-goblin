/**
 * This file defines functionality to run a configuration UI on the ESP32 board.
 *
 * The configuration UI provides a UI to enter a WiFi SSID and Password.
 */
#include "../config_ui.hpp"
#include <iostream>
#include <map>

class NoiseConfigUi : public IConfigUi {
  public:
    NoiseConfigUi(ILogging& logger) : IConfigUi(logger) {
    }

    void start() override {
        // no-op
    }

    auto isRunning() -> bool override {
        return true;
    }

    void setDefaults(
        const std::map<std::string, std::string>& defaults) override {
        this->defaults = defaults;
    }

    auto promptFor(const std::string& key, const std::string& message)
        -> std::string override {
        std::string input;
        std::string ourDefault = defaults.count(key) ? defaults[key] : "";
        std::cout << message << " [default: " << ourDefault << "]: ";
        std::getline(std::cin, input);
        return input.empty() ? ourDefault : input;
    }

    void tick() override {
        // no-op
    }

  private:
    std::map<std::string, std::string> defaults = {};
};
