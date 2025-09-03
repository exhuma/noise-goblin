#include "logging_impl.hpp"
#include <iostream>

void PosixLogging::debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cout << "[DEBUG] ";
    vfprintf(stdout, fmt, args);
    std::cout << std::endl;
    va_end(args);
}

void PosixLogging::info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cout << "[INFO] ";
    vfprintf(stdout, fmt, args);
    std::cout << std::endl;
    va_end(args);
}

void PosixLogging::error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    std::cerr << "[ERROR] ";
    vfprintf(stderr, fmt, args);
    std::cerr << std::endl;
    va_end(args);
}

void PosixLogging::setup() {
    // No setup required for Posix
}
