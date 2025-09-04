#include "../library.hpp"
#include "../logging.hpp"

struct Esp32Library : ILibrary {
    explicit Esp32Library(ILogging& logger) : logger(logger) {
    }

    void setup() override;
    auto getRandomSound() -> std::string override;

  private:
    ILogging& logger;
};
