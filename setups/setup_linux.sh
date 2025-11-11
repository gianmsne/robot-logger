#!/bin/bash

# setup_linux.sh - Installs dependencies for robotLogger project on Debian/Ubuntu/Raspberry Pi OS

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

echo "Creating working database from template..."
DB_DIR="database"
TEMPLATE_DB="$DB_DIR/database_template.db"
WORKING_DB="$DB_DIR/robot_logger.db"

mkdir -p "$DB_DIR"

if [ ! -f "$TEMPLATE_DB" ]; then
    echo "Error: Template database $TEMPLATE_DB not found!"
    exit 1
fi

if [ -f "$WORKING_DB" ]; then
    echo "Working database $WORKING_DB already exists. Skipping creation."
else
    cp "$TEMPLATE_DB" "$WORKING_DB"
    echo "Created $WORKING_DB from template."
fi

echo "To build the project, run:"
echo "    make"