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

    std::thread eventThread;
    std::atomic<bool> running;
    std::queue<int> eventQueue;
    std::mutex queueMutex;
    std::condition_variable eventCondition;
    EventCallback eventCallback;
};
