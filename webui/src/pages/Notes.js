import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Notes() {
  const columnOrder = ["robotName", "Note", "noteLeftBy", "timeOfNote"];
  const columnLabels = {
      robotName: "Robot Name",
      Note: "Note",
      noteLeftBy: "Note Left By",
      timeOfNote: "Time Of Note",
  };
  const timeColumns = new Set(["timeOfNote"]);

  return (
    <div>
      <DataTable
        path="/notes"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        timeColumns={timeColumns}
      />
    </div>
  );
}
