#pragma once
#include <string>
#include "config.hpp"
#include "http.hpp"
#include "logging.hpp"

/// @brief The sound-library subsystem
/// @details Interface for sound library functionality.
///
/// Provides an abstract interface for sound library operations.
struct ILibrary {
    ILibrary(ILogging &logger, IHttp &http, IConfig &config)
        : logger(logger), http(http), config(config) {
    }

    /// @brief Performs periodic tasks for the sound library.
    virtual void tick() = 0;

    /// @brief Retrieves a random sound from the library.
    /// @return A string representing the random sound.
    virtual auto getRandomSound() -> std::string = 0;

  protected:
    ILogging &logger;
    std::vector<std::string> soundByteNames;
    IConfig &config;
    IHttp &http;
};
