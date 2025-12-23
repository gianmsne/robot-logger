// Prefer runtime-injected value from window._env_, then fall back to build-time env.
export const API_BASE = `http://${window._env_.REACT_APP_LAN_IP}:5001/backend`;

export async function fetchJson(path) {
  const normalizedPath = path.startsWith("/") ? path : `/${path}`;
  const url = `${API_BASE}${normalizedPath}`;
  console.log("Fetching from:", url); // check the URL
  const res = await fetch(url);
  if (!res.ok) {
    const txt = await res.text();
    throw new Error(`HTTP ${res.status}: ${txt}`);
  }
  return res.json();
}