#pragma once
#include <string>

struct ILibrary {
    virtual ~ILibrary() = default;
    virtual void setup() = 0;
    virtual std::string getRandomSound() = 0;
};
