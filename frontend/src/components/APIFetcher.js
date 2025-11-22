
export const API_BASE = process.env.NODE_ENV === "production" ? `${process.env.REACT_APP_LAN_IP}/backend` : "/backend"

export async function fetchJson(path) {
  console.log(API_BASE)
  const normalizedPath = path.startsWith("/") ? path : `/${path}`;
  const url = `${API_BASE}${normalizedPath}`;
  const res = await fetch(url);
  if (!res.ok) {
    const txt = await res.text();
    throw new Error(`HTTP ${res.status}: ${txt}`);
  }
  return res.json();
}