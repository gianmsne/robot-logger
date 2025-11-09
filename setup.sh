#!/bin/bash

# setup.sh - Installs dependencies for robotLogger project (Raspberry Pi OS/Debian/Ubuntu)
# MACOS: brew install opencv zbar sqlite3.

set -e

echo "Updating system packages..."
sudo apt update
sudo apt upgrade -y

echo "Installing build tools and libraries..."
sudo apt install -y g++ make pkg-config cmake git

echo "Installing SQLite3 development libraries..."
sudo apt install -y libsqlite3-dev

echo "Installing OpenCV development libraries..."
sudo apt install -y libopencv-dev

echo "Installing ZBar barcode scanner library..."
sudo apt install -y libzbar-dev

echo "All dependencies installed successfully!"

echo "To build the project, run:"
echo "    make"