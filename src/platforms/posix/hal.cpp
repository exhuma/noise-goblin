#include "../hal.h"
#include <stdio.h>
#include <unistd.h>

void hal_print(const char *message) {
    printf("%s\n", message);
}

void setup() {
    // Dummy setup for POSIX/PC
    hal_print("Noise goblin setup (PC)");
}

void loop() {
    // Dummy loop for POSIX/PC
    hal_print("Noise goblin loop (PC)");
    sleep(1);
}
