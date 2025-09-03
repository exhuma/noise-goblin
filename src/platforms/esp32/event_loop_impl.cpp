#include "event_loop_impl.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

constexpr size_t QUEUE_LENGTH = 10;
constexpr size_t ITEM_SIZE = sizeof(int);
TaskHandle_t eventLoopTaskHandle = nullptr;
QueueHandle_t eventQueue = nullptr;

void eventLoopTask(void *param) {
    auto *eventLoop = static_cast<Esp32EventLoop *>(param);
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

Esp32EventLoop::~Esp32EventLoop() {
    stop();
    if (eventQueue) {
        vQueueDelete(eventQueue);
        eventQueue = nullptr;
    }
}

void Esp32EventLoop::setup() {
    eventQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
}

void Esp32EventLoop::start() {
    if (!eventLoopTaskHandle) {
        xTaskCreate(eventLoopTask, "EventLoopTask", 2048, this, 5,
                    &eventLoopTaskHandle);
    }
}

void Esp32EventLoop::stop() {
    if (eventLoopTaskHandle) {
        vTaskDelete(eventLoopTaskHandle);
        eventLoopTaskHandle = nullptr;
    }
}

void Esp32EventLoop::postEvent(int event) {
    if (eventQueue) {
        xQueueSend(eventQueue, &event, portMAX_DELAY);
    }
}

void Esp32EventLoop::setEventCallback(EventCallback callback) {
    eventCallback = std::move(callback);
}
