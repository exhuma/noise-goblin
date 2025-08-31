#include "../logging.h"
#include <stdio.h>
#include <unistd.h>

void log(const char *msg) {
    printf("%s", msg);
}

void logln(const char *msg) {
    printf("%s\n", msg);
}

void setup_logging() {
    logln("setup_logging called");
}
