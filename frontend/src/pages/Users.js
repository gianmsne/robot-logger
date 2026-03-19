import DataTable from "../components/DataTable";

export const columnOrder = ["userID", "givenName", "familyName", "isAdmin"];
export const columnLabels = {
    userID: "User ID",
    givenName: "Given Name",
    familyName: "Family Name",
    isAdmin: "Is Admin"
};
export const booleanColumns = new Set(["isAdmin"]);

export default function Users({ sortBy, sortOrder, filters }) {
  return (
    <div>
      <DataTable
        path="/users"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
        sortBy={sortBy}
        sortOrder={sortOrder}
        initialSortBy={"userID"}
        initialSortOrder={"asc"}
        filters={filters}
      />
    </div>
  );
}
