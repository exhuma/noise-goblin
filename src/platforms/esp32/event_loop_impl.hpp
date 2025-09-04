#pragma once
#include "../eventLoop.hpp"

struct Esp32EventLoop : IEventLoop {
    ~Esp32EventLoop() override;
    explicit Esp32EventLoop() = default;
    using EventCallback = std::function<void(int)>;

    void setup() override;
    void start() override;
    void stop() override;
    void postEvent(int event) override;
    void setEventCallback(EventCallback callback) override;

  private:
    void run();

    std::condition_variable eventCondition;

  public:
    EventCallback eventCallback;

    auto getEventCallback() -> EventCallback const {
        return eventCallback;
    }
};
