#include "logging_impl.hpp"
#include <chrono>
#include <ctime>
#include <iostream>

std::string timestamp() {
    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
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
    std::cout << "[" << timestamp() << "] [INFO] ";
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
    // No setup required for Posix
}
