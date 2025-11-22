const envBase = process.env.REACT_APP_API_BASE; // set in .env or docker-compose
const envPort = "5001";

export const API_BASE = (() => {
  if (envBase) return envBase.replace(/\/$/, "");
  // In production the frontend should be served from the same host => use relative path
  if (process.env.NODE_ENV === "production") return "/backend";
  // Dev: construct using the page hostname so other devices on the LAN can reach backend
  const proto = (typeof window !== "undefined" && window.location && window.location.protocol) || "http:";
  const host = (typeof window !== "undefined" && window.location && window.location.hostname) || "localhost";
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