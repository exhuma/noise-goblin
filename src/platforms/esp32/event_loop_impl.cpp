#include "../eventLoop.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

constexpr size_t QUEUE_LENGTH = 10;
constexpr size_t ITEM_SIZE = sizeof(int);
static TaskHandle_t eventLoopTaskHandle = nullptr;
static QueueHandle_t eventQueue = nullptr;

static void eventLoopTask(void *param) {
    auto *eventLoop = static_cast<IEventLoop *>(param);
    int event;
    while (true) {
        if (xQueueReceive(eventQueue, &event, portMAX_DELAY) == pdPASS) {
            auto callback = eventLoop->getEventCallback();
            if (callback) {
                callback(event);
            }
        }
    }
}

class Esp32EventLoop : public IEventLoop {
  public:
    ~Esp32EventLoop() override {
        stop();
        if (eventQueue) {
            vQueueDelete(eventQueue);
            eventQueue = nullptr;
        }
    }

    void setup() override {
        eventQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
        start();  // Start the event loop task
    }

    void start() override {
        if (!eventLoopTaskHandle) {
            xTaskCreate(eventLoopTask, "EventLoopTask", 2048, this, 5,
                        &eventLoopTaskHandle);
        }
    }

    void stop() override {
        if (eventLoopTaskHandle) {
            vTaskDelete(eventLoopTaskHandle);
            eventLoopTaskHandle = nullptr;
        }
    }

    void postEvent(int event) override {
        if (eventQueue) {
            xQueueSend(eventQueue, &event, portMAX_DELAY);
        }
    }

    void setEventCallback(EventCallback callback) override {
        eventCallback = std::move(callback);
    }

    auto getEventCallback() const -> EventCallback override {
        return eventCallback;
    }
};
