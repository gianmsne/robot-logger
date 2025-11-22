const envBase = process.env.REACT_APP_API_BASE; // optional override
const envPort = "5001";

export const API_BASE = (() => {
  // Use explicit env variable if set
  if (envBase && !envBase.includes("localhost")) return envBase.replace(/\/$/, "");

  // Production relative path
  if (process.env.NODE_ENV === "production") return "/backend";

  // Dev: use the current page hostname (LAN IP if accessed via LAN)
  const proto = window.location?.protocol || "http:";
  let host = window.location?.hostname || "localhost";

  // If localhost, optionally override with LAN IP from env
  if (host === "localhost" || host === "127.0.0.1") {
    host = process.env.REACT_APP_LAN_IP || host;
  }

  return `${proto}//${host}:${envPort}/backend`;
})();

export async function fetchJson(path) {
  const normalizedPath = path.startsWith("/") ? path : `/${path}`;
  const url = `${API_BASE}${normalizedPath}`;
  const res = await fetch(url);
  if (!res.ok) {
    const txt = await res.text();
    throw new Error(`HTTP ${res.status}: ${txt}`);
  }
  return res.json();
}