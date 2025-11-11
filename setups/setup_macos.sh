#!/bin/bash

# setup_macos.sh - Installs dependencies for robotLogger project on macOS
# REQUIRES: Homebrew (https://brew.sh/). Will install it if not found.

set -e

echo "Checking Homebrew installation..."
if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

echo "Installing dependencies..."
brew install opencv zbar sqlite3 g++ make cmake git

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