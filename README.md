# Noise Goblin

## Description

An application for the ESP32-S3 board to trigger random sound-bytes.

The project is inspired by a "toy-button" that can be loaded up with MP3-files
which it then plays randomly on each button press. Another big inspiration comes
an online collection of sound-bytes accessible via a private RESTful API.

This project combines both: Pressing the button will fetch a random sound-byte
from this online collection and play it.

## Wiring

See https://app.cirkitdesigner.com/project/fc8b766f-41c9-44f6-97f5-d1bfeb38f578

## Project Plan

See GitHub milestones at https://github.com/exhuma/noise-goblin/milestones

## Development Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) for building and flashing
- [w64devkit](https://github.com/skeeto/w64devkit) for Windows development (GCC
  toolchain)
- [Python](https://python.org/) for pre-commit hooks

### Dev-Container

The project comes with a dev-container configuration for Visual Studio Code.
This allows you to develop inside a container with all necessary dependencies
pre-installed.

If the PlatformIO integration fails, you can still build and run the project
using the command line inside the dev-container (see below).

It seems that having the PlatformIO extension installed on the host-machine can
cause issues with the integration. If you encounter problems, try disabling the
extension in your host environment and reloading the dev-container.

### Building and running

This project uses PlatformIO environments defined in `platformio.ini`.

- Build for ESP32:

  ```
  pio run -e esp32-s3-devkitc
  ```

- Upload to ESP32:

  ```
  pio run -e esp32-s3-devkitc --target upload
  ```

- Open serial monitor for ESP32:

  ```
  pio device monitor -e esp32-s3-devkitc
  ```

- Build POSIX (local test binary):

  ```
  pio run -e posix
  ```

- Clean build artifacts:

  ```
  # clean ESP32
  pio run -e esp32-s3-devkitc -t clean

  # clean POSIX
  pio run -e posix -t clean
  ```

- Static analysis / code check (PlatformIO):

  ```
  pio check -e esp32-s3-devkitc
  ```

## Platform Support

- **ESP32**: IoT and embedded applications
- **POSIX**: Linux, macOS, and Windows (via MinGW/w64devkit). For testing
  purposes only.
