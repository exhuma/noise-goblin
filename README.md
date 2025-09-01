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

## Roadmap

### ✅ Milestone 1: Basic Prototype

#### Hardware & Basic UI

- Wiring components up to be able to play sound-files from the Internet
- Connect to a WiFi access point
- Play an online web-radio stream

#### Code

- Basic PlatformIO/Arduino setup

### ✅ Milestone 2: Trigger Button

- Wire up a button to trigger sound playback
- Hard coded list of sound-byte URLs
- Fetch a random sound-byte from the list when the button is pressed

#### Code

- Introduce option to run a dummy implementation directly on Posix for quicker
  feedback loop

### Milestone 3: Dynamic Sound List

#### Hardware

- Battery Pack

#### Code

- Introduce dependency injection for improved platform abstraction.
- Provide a configuration option for the sound-byte list
- Replace hard-coded list with the dynamic online result

### Milestone 4: Cleanup

#### Hardware

- Get a big fat button
- Case

#### Code

- TBD

### Milestone 5: More controls & Independenc

#### Hardware

- Add a volume control
- Add SD-Card Reader

#### Code

- TBD


## Development Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) for building and flashing
- [w64devkit](https://github.com/skeeto/w64devkit) for Windows development (GCC
  toolchain)
- [Python](https://python.org/) for pre-commit hooks

## Platform Support

- **ESP32**: IoT and embedded applications
- **POSIX**: Linux, macOS, and Windows (via MinGW/w64devkit). For testing
  purposes only.
