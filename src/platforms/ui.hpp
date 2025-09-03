#pragma once

struct IUserInterface {
    virtual ~IUserInterface() = default;
    virtual void setup() = 0;
    virtual void tick() = 0;
};
