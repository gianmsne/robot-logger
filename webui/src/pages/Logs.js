import DataTable from "../components/DataTable";

export const columnOrder = ["robotName", "checkOutUserID", "checkOut", "checkInUserID", "checkIn"];
export const columnLabels = {
    robotName: "Robot Name",
    checkOutUserID: "Check Out User",
    checkOut: "Check Out Time",
    checkInUserID: "Check In User",
    checkIn: "Check In Time"
};
export const filterColumns = new Set(["robotName"]);

export default function Logs({ sortBy, sortOrder, userMap, filters, onRowsLoaded }) {
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
        filters={filters}
        onRowsLoaded={onRowsLoaded}
      />
    </div>
  );
}
