#pragma once

struct IUserInterface {
    virtual ~IUserInterface() = default;
    virtual void setup(ILogging& logger) = 0;
    virtual void tick(IAudio& audio, IConfig& config, ILogging& logger) = 0;
};
