#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

inline const int EVENT_PLAY_BUTTON_PRESSED = 1;
inline const int EVENT_RESET_BUTTON_PRESSED = 2;

struct IEventLoop {
    using EventCallback = std::function<void(int)>;

    virtual ~IEventLoop() = default;
    virtual void setup() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void postEvent(int event) = 0;
    virtual void setEventCallback(EventCallback callback) = 0;
};
