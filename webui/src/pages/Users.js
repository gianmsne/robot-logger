import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Users() {
  const columnOrder = ["userID", "givenName", "familyName", "isInducted", "isAdmin"];
  const columnLabels = {
      userID: "User ID",
      givenName: "Given Name",
      familyName: "Family Name",
      isInducted: "Is Inducted",
      isAdmin: "Is Admin"
  };
  const booleanColumns = new Set(["isInducted", "isAdmin"]);

  return (
    <div>
      <DataTable
        path="/users"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
      />
    </div>
  );
}
