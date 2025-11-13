import React, { useEffect, useState } from "react";
import { fetchJson } from "../components/APIFetcher";

export default function DataTable({ path, columnOrder = [], columnLabels = {}, booleanColumns = new Set(), timeColumns = new Set() }) {
  const [rows, setRows] = useState([]);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    let mounted = true;
    setLoading(true);
    fetchJson(path)
      .then(data => { if (mounted) setRows(Array.isArray(data) ? data : []); })
      .catch(err => { if (mounted) setError(err.message); })
      .finally(() => { if (mounted) setLoading(false); });
    return () => { mounted = false; };
  }, [path]);

  const booleanToYesNo = v => (v ? "Yes" : "No");

  const formatCTime = v => {
    if (typeof v !== "number" || isNaN(v)) return v;
    try {
      // If value looks like milliseconds, divide by 1000
      const ts = v > 1e12 ? Math.floor(v / 1000) : v;
      const date = new Date(ts * 1000);
      return date.toLocaleString();
    } catch {
      return v;
    }
  };

  if (loading) return <div>Loading...</div>;
  if (error) return <div style={{ color: "salmon" }}>Error: {error}</div>;
  if (rows.length === 0) return <div>No records to display.</div>;

  return (
    <table className="table-bordered users-table">
      <thead>
        <tr>
          {columnOrder.map(col => (
            <th key={col} style={{ textAlign: "left", padding: "6px" }}>
              {columnLabels[col] || col}
            </th>
          ))}
        </tr>
      </thead>
      <tbody>
        {rows.map((row, i) => (
          <tr key={i}>
            {columnOrder.map((col, j) => {
              let val = row[col];

              if (booleanColumns.has(col)) {
                val = booleanToYesNo(Boolean(val));
              } else if (timeColumns.has(col)) {
                val = formatCTime(val);
              } else if (val === null || val === undefined) {
                val = "";
              } else if (typeof val === "object") {
                val = JSON.stringify(val);
              } else {
                val = String(val);
              }

              return <td key={j} style={{ padding: "6px" }}>{val}</td>;
            })}
          </tr>
        ))}
      </tbody>
    </table>
  );
}