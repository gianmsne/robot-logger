import { Dropdown, Form } from "react-bootstrap";

export default function FiltersDropdown({ 
  filters, 
  setFilters, 
  columnOrder = [], 
  booleanColumns = new Set(),
  columnLabels = {},
  filterColumns = new Set(),
  rows = [],
}) {
  const toggleFilterBoolean = (key, value) => {
    setFilters(prev => {
      const next = { ...prev };
      if (key in next && next[key] === value) delete next[key];
      else next[key] = value;
      return next;
    });
  };

  const toggleFilterString = (key, value) => {
    setFilters(prev => {
      const next = { ...prev };
      if (value === "") delete next[key];
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
              style={{ marginBottom:"12px" }}
            />
          </>
        ))}

        {columnOrder.filter(col => filterColumns.has(col)).map(col => {
          const uniqueValues = [...new Set(rows.map(r => r[col]))].filter(v => v != null);
          return (
            <>
              <Form.Label>{columnLabels[col] || col}</Form.Label>
              <Form.Control
                as="select"
                value={filters[col] || ""}
                onChange={e => toggleFilterString(col, e.target.value)}
                style={{ marginBottom:"12px" }}
              >
                <option value="">All</option>
                {uniqueValues.map(val => (
                  <option key={val} value={val}>{val}</option>
                ))}
              </Form.Control>
            </>
          );
        })}
      </Form>
    </Dropdown>
  );
}
