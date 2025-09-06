#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include "../logging.hpp"

static auto timestamp() -> std::string {
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    std::array<char, 20> buf;
    std::strftime(buf.data(), buf.size(), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&t));
    return {buf.data()};
}

class PosixLogging : public ILogging {
    // NOTE: The functions have been defused (implementation removed) in favour
    // of a more "interactive" CLI ui for testing using proper events to better
    // represent the execution mode on the ESP32 (main-loop).
    void debug(const char *fmt, ...) override {
        // no-op
    }

    void info(const char *fmt, ...) override {
        // no-op
    }

    void error(const char *fmt, ...) override {
        // no-op
    }

    void setup() override {
        debug("Logging setup complete");
    }
};
