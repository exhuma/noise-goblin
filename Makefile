# Makefile for Noise Goblin multi-platform project
# Supports both POSIX (local) and ESP32 (via PlatformIO) builds

# Compiler settings for POSIX build
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++11
INCLUDES = -Isrc -Isrc/core -Isrc/platforms

# Source discovery (replace hard-coded filenames)
# Top-level and platform source discovery. This keeps the POSIX build
# limited to app, core and posix platform implementations so we don't
# accidentally link ESP32-specific sources when building locally.
SRC_DIR := src
APP_SRCS := $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.c)
CORE_SRCS := $(wildcard $(SRC_DIR)/core/*.cpp $(SRC_DIR)/core/*.c)
POSIX_SRCS := $(wildcard $(SRC_DIR)/platforms/posix/*.cpp $(SRC_DIR)/platforms/posix/*.c)

# All sources to link for the POSIX target (main + core + posix HAL)
POSIX_LINK_SRCS := $(APP_SRCS) $(CORE_SRCS) $(POSIX_SRCS)

# Find the application's main file (first match of *main.cpp)
MAIN_SRC := $(firstword $(filter %main.cpp,$(APP_SRCS)))

# Discover headers used for change-based rebuilding (optional)
CORE_HEADERS := $(wildcard $(SRC_DIR)/core/*.h $(SRC_DIR)/platforms/*.h $(SRC_DIR)/platforms/*/*.h)

# Output targets
POSIX_TARGET = goblin.exe
ESP32_ENV = esp32-s3-devkitc

# PlatformIO executable (adjust path if needed)
PIO = "C:\Users\miche\.platformio\penv\Scripts\platformio.exe"

# Default target
.DEFAULT_GOAL := help

# Help target
help:
	@echo "Available targets:"
	@echo "  posix     - Build for POSIX systems (Windows/Linux/macOS)"
	@echo "  esp32     - Build for ESP32 using PlatformIO"
	@echo "  clean     - Clean build artifacts"
	@echo "  clean-all - Clean both POSIX and ESP32 build artifacts"
	@echo "  upload    - Build and upload to ESP32"
	@echo "  monitor   - Open serial monitor for ESP32"
	@echo "  check     - Run PlatformIO code analysis"
	@echo "  help      - Show this help message"

# POSIX build target
posix: $(POSIX_TARGET)

$(POSIX_TARGET): $(POSIX_LINK_SRCS) $(CORE_HEADERS)
	@echo "Building for POSIX..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(POSIX_TARGET) \
		$(POSIX_LINK_SRCS)
	@echo "POSIX build complete: $(POSIX_TARGET)"

# OS-aware helper variables for running/removing the POSIX target
ifeq ($(OS),Windows_NT)
RUN_EXE = .\\$(POSIX_TARGET)
DEL_CMD = -del $(POSIX_TARGET) 2>nul || true
else
RUN_EXE = ./$(POSIX_TARGET)
DEL_CMD = rm -f $(POSIX_TARGET) 2>/dev/null || true
endif

# ESP32 build target
esp32:
	@echo "Building for ESP32..."
	$(PIO) run -e $(ESP32_ENV)
	@echo "ESP32 build complete"

# Upload to ESP32
upload: esp32
	@echo "Uploading to ESP32..."
	$(PIO) run -e $(ESP32_ENV) --target upload

# Open serial monitor
monitor:
	@echo "Opening serial monitor..."
	$(PIO) device monitor

# Clean POSIX build

clean-posix:
	@echo "Cleaning POSIX build..."
	$(DEL_CMD)

# Clean ESP32 build
clean-esp32:
	@echo "Cleaning ESP32 build..."
	$(PIO) run --target clean

# Clean both builds
clean: clean-posix

clean-all: clean-posix clean-esp32

# Test POSIX build
test-posix: posix
	@echo "Running POSIX build..."
	$(RUN_EXE)

# Check code with PlatformIO
check:
	@echo "Running PlatformIO check..."
	$(PIO) check

.PHONY: help posix esp32 upload monitor clean clean-posix clean-esp32 clean-all test-posix check
