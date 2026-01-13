import DataTable from "../components/DataTable";

export const columnOrder = ["equipmentName", "checkOutUserID", "checkOut", "checkInUserID", "checkIn"];
export const columnLabels = {
    equipmentName: "Equipment Name",
    checkOutUserID: "Check Out User",
    checkOut: "Check Out Time",
    checkInUserID: "Check In User",
    checkIn: "Check In Time"
};
export const filterColumns = new Set(["equipmentName"]);

export default function Logs({ sortBy, sortOrder, userMap, filters, onRowsLoaded }) {
  const timeColumns = new Set(["checkOut", "checkIn"]);
  
  return (
    <div>
      <DataTable
        path="/logs"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        filterColumns={filterColumns}
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
