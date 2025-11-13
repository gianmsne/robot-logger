import { useEffect, useState } from "react";
import DataTable from "../components/DataTable";


export default function Robots() {
  const columnOrder = ["robotName", "robotType", "robotCondition", "rmitID", "location", "isAvailable"];
  const columnLabels = {
      robotName: "Robot Name",
      robotType: "Robot Type",
      robotCondition: "Robot Condition",
      rmitID: "RMIT ID",
      location: "Location",
      isAvailable: "Is Available"
  };
  const booleanColumns = new Set(["isAvailable"]);

  return (
    <div>
      <DataTable
        path="/robots"
        columnOrder={columnOrder}
        columnLabels={columnLabels}
        booleanColumns={booleanColumns}
      />
    </div>
  );
}
