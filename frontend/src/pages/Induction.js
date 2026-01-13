import DataTable from "../components/DataTable";

export const columnOrder = ["userID", "isInductedNao", "isInductedBooster", "isInductedVRHeadset"];
export const columnLabels = {
    userID: "User ID",
    isInductedNao: "Nao Inducted",
    isInductedBooster: "Booster Inducted",
    isInductedVRHeadset: "VR Headset Inducted"
};
export const booleanColumns = new Set(["isInductedNao", "isInductedBooster", "isInductedVRHeadset"]);

export default function Induction({ sortBy, sortOrder, userMap, filters, onRowsLoaded }) {
  return (
    <div>
      <DataTable
        path="/induction"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"userID"}
        initialSortOrder={"asc"}
        userMap={userMap}
        filters={filters}
        onRowsLoaded={onRowsLoaded}
      />
    </div>
  );
}
