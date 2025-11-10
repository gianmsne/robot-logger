import { useEffect, useState } from "react";

export default function Users() {
  const [users, setUsers] = useState([]);
  const columnOrder = ["userID", "givenName", "familyName", "isInducted", "isAdmin"];

const columnLabels = {
    userID: "User ID",
    givenName: "Given Name",
    familyName: "Family Name",
    isInducted: "Is Inducted",
    isAdmin: "Is Admin"
  };

  useEffect(() => {
    async function loadUsers() {
      try {
        const res = await fetch("http://localhost:5000/api/users");
        if (!res.ok) throw new Error(`HTTP ${res.status}`);
        const data = await res.json();
        setUsers(data);
      } catch (err) {
        console.error("Failed to load users:", err);
        setUsers([]);
      }
    }
    loadUsers();
}, []);

  // build unified column list (preserve insertion order)
  const columns = [];
  const seen = new Set();
  users.forEach(u => {
    Object.keys(u).forEach(k => {
      if (!seen.has(k)) {
        seen.add(k);
        columns.push(k);
      }
    });
  });

  return (
    <div>
      {users.length === 0 ? (
        <div>No users to display.</div>
      ) : (
        <table className="table-bordered">
          <thead>
            <tr>
              {columnOrder.map(col => <th key={col} style={{ textAlign: "left", padding: "6px" }}>{columnLabels[col] || col}</th>)}
            </tr>
          </thead>
          <tbody>
            {users.map((row, i) => (
              <tr key={i}>
                {columnOrder.map((col, j) => {
                  const val = row[col];
                  const cell = val === null || val === undefined
                    ? ""
                    : (typeof val === "object" ? JSON.stringify(val) : String(val));
                  return <td key={j} style={{ padding: "6px" }}>{cell}</td>;
                })}
              </tr>
            ))}
          </tbody>
        </table>
      )}
    </div>
  );
}
