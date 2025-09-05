#include <iostream>
#include "../eventLoop.hpp"

class PosixEventLoop : public IEventLoop {
    using EventCallback = std::function<void(int)>;

  public:
    PosixEventLoop() : running(false) {
    }

    ~PosixEventLoop() override {
        if (running) {
            running = false;
            eventCondition.notify_all();
            if (eventThread.joinable()) {
                eventThread.join();
            }
        }
    }

    void start() override {
        running = true;
        eventThread = std::thread([this] { run(); });
    }

    void stop() override {
        if (running) {
            running = false;
            eventCondition.notify_all();
            if (eventThread.joinable()) {
                eventThread.join();
            }
        }
    }
    void setup() override {
        start();
    }

    void postEvent(int event) override {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            eventQueue.push(event);
        }
        eventCondition.notify_one();
    }

    void setEventCallback(EventCallback callback) override {
        eventCallback = callback;
    }

  private:
    void run() {
        while (running) {
            int event;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                eventCondition.wait(
                    lock, [this] { return !eventQueue.empty() || !running; });

                if (!running && eventQueue.empty()) {
                    break;
                }

                event = eventQueue.front();
                eventQueue.pop();
            }

            if (eventCallback) {
                eventCallback(event);
            }
        }
    }

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
};
