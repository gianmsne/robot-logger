#!/bin/sh
set -e

# Determine values to expose to client
: "${REACT_APP_LAN_IP:=${REACT_APP_LAN_IP:-localhost}}"

# Build env object
cat > /app/public/env.js <<EOF
window._env_ = {
  REACT_APP_LAN_IP: "${REACT_APP_LAN_IP}"
};
EOF

# Start CRA dev server
exec npm start
