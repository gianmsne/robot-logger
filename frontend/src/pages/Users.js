import DataTable from "../components/DataTable";

export const columnOrder = ["userID", "givenName", "familyName", "isInducted", "isAdmin"];
export const columnLabels = {
    userID: "User ID",
    givenName: "Given Name",
    familyName: "Family Name",
    isInducted: "Is Inducted",
    isAdmin: "Is Admin"
};
export const booleanColumns = new Set(["isInducted", "isAdmin"]);

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
