#include "logging_impl.hpp"
#include <iostream>

void PosixLogging::log(const char *msg) {
    std::cout << msg;
}

void PosixLogging::logln(const char *msg) {
    std::cout << msg << std::endl;
}

void PosixLogging::setup() {
    // No setup required for Posix
}
