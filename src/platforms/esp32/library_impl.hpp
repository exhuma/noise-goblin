#include "../library.hpp"
#include "../logging.hpp"

struct Esp32Library : ILibrary {
    explicit Esp32Library(ILogging& logger) : logger(logger) {
    }

    void setup() override;
    std::string getRandomSound() override;

  private:
    ILogging& logger;
};
