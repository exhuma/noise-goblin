#pragma once

/// @brief Possible application states
/// @details These are used during the main-loop to display useful information
/// to the user and/or request user-input.
enum AppState { RequestingConfig, Normal, NoNetwork, Connecting };
