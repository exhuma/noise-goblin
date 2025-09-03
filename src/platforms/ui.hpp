#pragma once

struct IUserInterface {
    virtual ~IUserInterface() = default;
    virtual void setup() = 0;
    virtual void tick() = 0;
    virtual void displayConfigCleared() = 0;
    virtual void displayCaptivePortal() = 0;
    virtual void displayNormal() = 0;
};
