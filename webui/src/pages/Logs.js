import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Logs({ sortBy, sortOrder, userMap }) {
  const columnOrder = ["robotName", "checkOutUserID", "checkOut", "checkInUserID", "checkIn"];
  const columnLabels = {
      robotName: "Robot Name",
      checkOutUserID: "Check Out User",
      checkOut: "Check Out Time",
      checkInUserID: "Check In User",
      checkIn: "Check In Time"
  };
  const timeColumns = new Set(["checkOut", "checkIn"]);

  return (
    <div>
      <DataTable
        path="/logs"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        timeColumns={timeColumns}
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"checkOut"}
        initialSortOrder={"desc"}
        userMap={userMap}
      />
    </div>
  );
}
