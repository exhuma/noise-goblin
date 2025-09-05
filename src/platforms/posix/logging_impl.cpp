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
    void debug(const char *fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        std::cout << "[" << timestamp() << "] [DEBUG] ";
        vfprintf(stdout, fmt, args);
        std::cout << std::endl;
        va_end(args);
    }

    void info(const char *fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        std::cout << "[" << timestamp() << "] [INFO ] ";
        vfprintf(stdout, fmt, args);
        std::cout << std::endl;
        va_end(args);
    }

    void error(const char *fmt, ...) override {
        va_list args;
        va_start(args, fmt);
        std::cerr << "[" << timestamp() << "] [ERROR] ";
        vfprintf(stderr, fmt, args);
        std::cerr << std::endl;
        va_end(args);
    }

    void setup() override {
        debug("Logging setup complete");
    }
};
