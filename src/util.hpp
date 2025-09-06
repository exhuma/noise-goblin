#include <functional>
#include <istream>
#include <string>

void collectResources(std::istream &inStream,
                      std::function<void(const std::string &)> callback);
