#pragma once

struct IUserInterface {
    enum class LedState { Off, CaptivePortal, Reset, Normal, Connecting };
    virtual ~IUserInterface() = default;
    virtual void setup() = 0;
    virtual void tick() = 0;
    virtual void setState(LedState state) = 0;
};
