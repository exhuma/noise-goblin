#include "../library.hpp"
#include "../logging.hpp"

struct PosixLibrary : ILibrary {
    explicit PosixLibrary(ILogging& logger) : logger(logger) {
    }

    void setup() override;
    auto getRandomSound() -> std::string override;

  private:
    ILogging& logger;
};
