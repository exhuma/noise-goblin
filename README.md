# Noise Goblin

A multi-platform C++ project for ESP32 and POSIX systems built with PlatformIO.

## Development Setup

### Prerequisites

- [PlatformIO](https://platformio.org/) for building and flashing
- [w64devkit](https://github.com/skeeto/w64devkit) for Windows development (GCC
  toolchain)
- [Python](https://python.org/) for pre-commit hooks

### Code Formatting

This project uses **clang-format** for consistent code formatting across all C++
files.

#### Installing clang-format

**Windows (using winget):**
```powershell
winget install LLVM.ClangFormat
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt install clang-format
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf install clang-tools-extra
```

**macOS:**
```bash
brew install clang-format
```

### Building

#### PlatformIO (Recommended)

```bash
# Build for default environment
pio run
```

#### Manual Build (with w64devkit)

```bash
make
```

## Project Structure

```
├── src/
│   ├── main.cpp              # Main application entry point
│   ├── config.h              # Configuration settings
│   ├── core                  # Core functionality
│   └── platforms/            # Platform-specific implementations
│       ├── hal.h             # Interface for hardware abstraction layer
│       ├── esp32/            # Implementation for ESP32 board
│       └── posix/            # Implementation for POSIX systems
├── .clang-format            # Code formatting rules
├── .pre-commit-config.yaml  # Pre-commit hooks configuration
├── platformio.ini           # PlatformIO configuration
└── Makefile                 # Build automation
```

## Platform Support

- **ESP32**: IoT and embedded applications
- **POSIX**: Linux, macOS, and Windows (via MinGW/w64devkit)

## Contributing

1. Ensure clang-format is installed and configured
2. Install pre-commit hooks: `pre-commit install`
3. Make your changes
4. Code will be automatically formatted on commit
5. Submit a pull request
