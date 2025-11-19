import DataTable from "../components/DataTable";

export const columnOrder = ["robotName", "noteLeftBy", "timeOfNote", "note"];
export const columnLabels = {
    robotName: "Robot Name",
    note: "Note",
    noteLeftBy: "Note Left By",
    timeOfNote: "Time Of Note",
};
export const filterColumns = new Set(["robotName"])

export default function Notes({ sortBy, sortOrder, userMap, filters, onRowsLoaded }) {
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
        userMap={userMap}
        filters={filters}
        onRowsLoaded={onRowsLoaded}
      />
    </div>
  );
}
