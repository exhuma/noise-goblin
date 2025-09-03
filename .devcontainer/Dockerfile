FROM ubuntu:latest

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Configure apt and install packages
RUN apt-get update \
    && apt-get -y install --no-install-recommends \
        python3 \
        python3-pip \
        python3.10-venv \
        git \
        curl \
        udev \
        build-essential \
        libusb-1.0-0 \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO
RUN python3 -m pip install -U platformio

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog

# Create udev rules for USB devices
RUN echo 'SUBSYSTEM=="usb", MODE="0666"' > \
    /etc/udev/rules.d/99-platformio-udev.rules
