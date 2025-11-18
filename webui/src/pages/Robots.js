import DataTable from "../components/DataTable";

export const columnOrder = ["robotName", "robotType", "robotCondition", "rmitID", "location", "isAvailable"];
export const columnLabels = {
    robotName: "Robot Name",
    robotType: "Robot Type",
    robotCondition: "Robot Condition",
    rmitID: "RMIT ID",
    location: "Location",
    isAvailable: "Is Available"
};
export const booleanColumns = new Set(["isAvailable"]);
export const filterColumns = new Set(["robotType", "robotCondition", "location"]);

export default function Robots({ sortBy, sortOrder, filters }) {
  return (
    <div>
      <DataTable
        path="/robots"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"robotName"}
        initialSortOrder={"asc"}
        filters={filters}
      />
    </div>
  );
}
