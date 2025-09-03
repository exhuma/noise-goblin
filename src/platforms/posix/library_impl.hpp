#include "../library.hpp"
#include "../logging.hpp"

struct PosixLibrary : ILibrary {
    explicit PosixLibrary(ILogging& logger) : logger(logger) {
    }

    void setup() override;
    std::string getRandomSound() override;

  private:
    ILogging& logger;
};
