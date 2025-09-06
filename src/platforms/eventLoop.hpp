#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

/// @brief Play button pressed event
inline const int EVENT_PLAY_BUTTON_PRESSED = 1;

/// @brief Reset button pressed event
inline const int EVENT_RESET_BUTTON_PRESSED = 2;

/// @brief A sound has started playing
inline const int SOUND_PLAY_STARTED = 3;

/// @brief A sound has stopped playing
inline const int SOUND_PLAY_STOPPED = 4;

/// @brief A new network connection was requested
inline const int EVENT_WIFI_CONNECTING = 5;

/// @brief A new network connection was established
inline const int EVENT_WIFI_CONNECTED = 6;

/// @brief The Event-Loop subsystem
/// @details Interface for event loop functionality.
///
/// Provides an abstract interface for event loop operations.
struct IEventLoop {
    /// @brief Event callback function type.
    using EventCallback = std::function<void(int)>;

    /// @brief Virtual destructor for proper cleanup of derived classes.
    virtual ~IEventLoop() = default;

    /// @brief Initialises the event loop.
    virtual void setup() = 0;

    /// @brief Starts the event loop.
    virtual void start() = 0;

    /// @brief Stops the event loop.
    virtual void stop() = 0;

    /// @brief Posts an event to the event loop.
    /// @param event The event to post.
    virtual void postEvent(int event) = 0;

    /// @brief Sets the event callback function.
    /// @param callback The callback function to set.
    virtual void setEventCallback(EventCallback callback) = 0;

    /// @brief Gets the event callback function.
    /// @return The event callback function.
    virtual auto getEventCallback() const -> EventCallback = 0;

  protected:
    EventCallback eventCallback;
};
