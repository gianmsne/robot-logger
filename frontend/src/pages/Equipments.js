import DataTable from "../components/DataTable";

export const columnOrder = ["equipmentName", "equipmentType", "equipmentCondition", "location", "isAvailable", "permanentStatus"];
export const columnLabels = {
    equipmentName: "Equipment Name",
    equipmentType: "Equipment Type",
    equipmentCondition: "Equipment Condition",
    location: "Location",
    isAvailable: "Is Available",
    permanentStatus: "Permanent Status"
};
export const booleanColumns = new Set(["isAvailable"]);
export const filterColumns = new Set(["equipmentType", "equipmentCondition", "location"]);

export default function Equipments({ sortBy, sortOrder, filters, onRowsLoaded }) {
  return (
    <div>
      <DataTable
        path="/equipments"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
        filterColumns={filterColumns}
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"equipmentName"}
        initialSortOrder={"asc"}
        filters={filters}
        onRowsLoaded={onRowsLoaded}
      />
    </div>
  );
}
