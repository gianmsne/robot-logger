import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Logs() {
  const columnOrder = ["robotName", "checkOutUserID", "checkOut", "checkInUserID", "checkIn"];
  const columnLabels = {
      robotName: "Robot Name",
      checkOutUserID: "Check Out User ID",
      checkOut: "Check Out Time",
      checkInUserID: "Check In User ID",
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
      />
    </div>
  );
}
