const envBase = process.env.REACT_APP_API_BASE; // optional override from .env
const envPort = "5001"; // backend port exposed on host

export const API_BASE = (() => {
  if (envBase) return envBase.replace(/\/$/, "");

  if (process.env.NODE_ENV === "production") return "/backend";

  const proto = window.location?.protocol || "http:";
  let host = window.location?.hostname || "localhost";

  // If localhost, use optional LAN IP
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