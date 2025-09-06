#include <functional>
#include <istream>
#include <string>
#include <vector>

void collectResources(std::istream &inStream,
                      std::function<void(const std::string &)> callback) {
    std::string current;
    int depth = 0;
    char c;

    while (inStream.get(c)) {
        if (c == '{') {
            depth++;
        }
        if (depth > 1) {
            current.push_back(c);
        }
        if (c == '}') {
            depth--;
            if (depth == 1) {
                callback(current);
                current.clear();
            }
        }
    }
}
