# Windows Makefile for building the PC (POSIX) version
# Usage: make (requires MinGW or similar GCC toolchain)

CC = gcc
CFLAGS = -Wall -I./src/platforms -I./src/core
SRC = src/main.cpp src/core/core.h src/platforms/posix/hal.cpp src/platforms/posix/hal.cpp
TARGET = goblin.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) \
		src/main.cpp \
		src/platforms/posix/hal.cpp

clean:
	del $(TARGET)

.PHONY: all clean
