#pragma once

#include <sstream>
#include <string>

/// @brief Possible application states
/// @details These are used during the main-loop to display useful information
/// to the user and/or request user-input.
enum AppState { RequestingConfig, Normal, NoNetwork, Connecting, PlayingSound };

/// @brief Converts AppState enum to its string representation
/// @param state The AppState enum value
/// @return The string representation of the AppState
inline auto appStateToString(AppState state) -> std::string {
    switch (state) {
    case AppState::RequestingConfig:
        return "RequestingConfig";
    case AppState::Normal:
        return "Normal";
    case AppState::NoNetwork:
        return "NoNetwork";
    case AppState::Connecting:
        return "Connecting";
    case AppState::PlayingSound:
        return "PlayingSound";
    default: {
        std::string foo =
            "Unknown (" + std::to_string(static_cast<int>(state)) + ")";
        return foo;
    }
    }
}
