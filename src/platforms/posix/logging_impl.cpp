#include "logging_impl.hpp"
#include <array>
#include <chrono>
#include <ctime>
#include <iostream>

auto timestamp() -> std::string {
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    std::array<char, 20> buf;
    std::strftime(buf.data(), buf.size(), "%Y-%m-%d %H:%M:%S",
                  std::localtime(&t));
    return {buf.data()};
}

void PosixLogging::debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cout << "[" << timestamp() << "] [DEBUG] ";
    vfprintf(stdout, fmt, args);
    std::cout << std::endl;
    va_end(args);
}

void PosixLogging::info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cout << "[" << timestamp() << "] [INFO ] ";
    vfprintf(stdout, fmt, args);
    std::cout << std::endl;
    va_end(args);
}

void PosixLogging::error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cerr << "[" << timestamp() << "] [ERROR] ";
    vfprintf(stderr, fmt, args);
    std::cerr << std::endl;
    va_end(args);
}

void PosixLogging::setup() {
    debug("Logging setup complete");
}
