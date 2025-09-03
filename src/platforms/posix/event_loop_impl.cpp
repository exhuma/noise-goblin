#include "event_loop_impl.hpp"
#include <iostream>

PosixEventLoop::~PosixEventLoop() {
    stop();
}

void PosixEventLoop::setup() {
    start();
}

void PosixEventLoop::start() {
    running = true;
    eventThread = std::thread(&PosixEventLoop::run, this);
}

void PosixEventLoop::stop() {
    if (running) {
        running = false;
        eventCondition.notify_all();
        if (eventThread.joinable()) {
            eventThread.join();
        }
    }
}

void PosixEventLoop::postEvent(int event) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        eventQueue.push(event);
    }
    eventCondition.notify_one();
}

void PosixEventLoop::setEventCallback(EventCallback callback) {
    eventCallback = callback;
}

void PosixEventLoop::run() {
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
