#!/bin/sh
set -e

# Determine the LAN IP to expose to the client at runtime.
: "${REACT_APP_LAN_IP:=${REACT_APP_LAN_IP:-localhost}}"

cat > /usr/share/nginx/html/env.js <<EOF
window._env_ = {
  REACT_APP_LAN_IP: "${REACT_APP_LAN_IP}"
};
EOF

exec nginx -g 'daemon off;'
