import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Notes({ sortBy, sortOrder }) {
  const columnOrder = ["robotName", "noteLeftBy", "timeOfNote", "Note"];
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
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"timeOfNote"}
        initialSortOrder={"desc"}
      />
    </div>
  );
}
