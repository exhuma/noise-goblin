#pragma once
#include "../eventLoop.hpp"

struct PosixEventLoop : IEventLoop {
    ~PosixEventLoop() override;
    explicit PosixEventLoop() : running(false) {
    }
    using EventCallback = std::function<void(int)>;

    void setup() override;
    void start() override;
    void stop() override;
    void postEvent(int event) override;
    void setEventCallback(EventCallback callback) override;

  private:
    void run();

    /// @brief The event processing thread.
    std::thread eventThread;
    /// @brief Flag indicating whether the event loop is running.
    std::atomic<bool> running;
    /// @brief Queue for incoming events.
    std::queue<int> eventQueue;
    /// @brief Mutex for synchronizing access to the event queue.
    std::mutex queueMutex;
    /// @brief Condition variable for notifying event processing.
    std::condition_variable eventCondition;
    /// @brief Callback function for handling events.
    EventCallback eventCallback;
};
