import React, { useEffect, useState } from "react";
import { fetchJson } from "../components/APIFetcher";

export default function DataTable({ path, columnOrder = [], columnLabels = {}, booleanColumns = new Set() }) {
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
              const val = row[col];
              const cell = booleanColumns.has(col)
                ? booleanToYesNo(Boolean(val))
                : (val === null || val === undefined ? "" : (typeof val === "object" ? JSON.stringify(val) : String(val)));
              return <td key={j} style={{ padding: "6px" }}>{cell}</td>;
            })}
          </tr>
        ))}
      </tbody>
    </table>
  );
}