import { Dropdown, Form } from "react-bootstrap";

export default function FiltersDropdown({ 
  filters, 
  setFilters, 
  columnOrder = [], 
  booleanColumns = new Set(),
  columnLabels = {},
}) {
  const toggleFilterBoolean = (key, value) => {
    setFilters(prev => {
      const next = { ...prev };
      if (key in next && next[key] === value) delete next[key];
      else next[key] = value;
      return next;
    });
  };

  return (
    <Dropdown autoClose={false}>
      <Form className="p-3">
        {columnOrder.filter(col => booleanColumns.has(col)).map(col => (
          <>
            <Form.Label>{columnLabels[col] || col}</Form.Label>
            <Form.Check
              type="checkbox"
              label="Yes"
              checked={filters[col] === true}
              onChange={() => toggleFilterBoolean(col, true)}
            />
            <Form.Check
              type="checkbox"
              label="No"
              checked={filters[col] === false}
              onChange={() => toggleFilterBoolean(col, false)}
              style={{ paddingBottom:"10px" }}
            />
          </>
        ))}

      
      </Form>
    </Dropdown>
  );
}
