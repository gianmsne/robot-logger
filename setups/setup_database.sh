#!/bin/bash

# Ensures robot_logger.db exists in the database/ folder.
# If missing, copies database_template.db as the new database.

set -euo pipefail  # Safe bash: exit on error, undefined var, pipe failure

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DB_DIR="$PROJECT_ROOT/database"
TARGET_DB="$DB_DIR/robot_logger.db"
TEMPLATE_DB="$DB_DIR/database_template.db"

echo " >> Checking database setup in: $DB_DIR"

mkdir -p "$DB_DIR"

if [[ -f "$TARGET_DB" ]]; then
    echo -e "${GREEN}✓ Database already exists:${NC} $TARGET_DB"
    exit 0
fi

if [[ ! -f "$TEMPLATE_DB" ]]; then
    echo -e "${RED}✗ Error: Template not found!${NC}"
    echo "   Expected: $TEMPLATE_DB"
    echo "   Please add 'database_template.db' to the database/ folder."
    exit 1
fi

echo -e "${YELLOW}➜ Creating new database from template...${NC}"
cp "$TEMPLATE_DB" "$TARGET_DB"

echo -e "${GREEN}✓ Success! Created:${NC}"
echo "   $TARGET_DB"
echo "   (copied from template)"

exit 0