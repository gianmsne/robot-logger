import './App.css';
import './App.scss';
import { useEffect, useState } from "react";
import { Container, Stack } from "react-bootstrap";
import { FaSortAmountDown } from "react-icons/fa";
import { FaFilter } from "react-icons/fa6";
import { BsArrowUp, BsArrowDown } from "react-icons/bs";
import Button from 'react-bootstrap/Button';
import Dropdown from 'react-bootstrap/Dropdown';

import Users, { booleanColumns as userBooleanColumns, columnOrder as userColumnOrder, columnLabels as userColumnLabels } from './pages/Users';
import Equipment, { booleanColumns as equipmentBooleanColumns, columnOrder as equipmentColumnOrder, columnLabels as equipmentColumnLabels, filterColumns as equipmentFilterColumns } from './pages/Equipment';
import Logs, { columnOrder as logsColumnOrder, columnLabels as logsColumnLabels, filterColumns as logsFilterColumns } from './pages/Logs';
import Notes, { columnOrder as notesColumnOrder, columnLabels as notesColumnLabels, filterColumns as notesFilterColumns } from './pages/Notes';
import Inductions, { columnOrder as inductionsColumnOrder, columnLabels as inductionsColumnLabels, booleanColumns as inductionsBooleanColumns } from './pages/Inductions';
import FiltersDropdown from './components/FilterDropdown';

import { fetchJson } from "./components/APIFetcher";

function App() {
  const [activeTable, setActiveTable] = useState("Users");

  const [sortBy, setSortBy] = useState(null);
  const [sortOrder, setSortOrder] = useState('asc');

  const [filters, setFilters] = useState({});
  const [tableRows, setTableRows] = useState([]);

  const [userMap, setUserMap] = useState({});
  
  const handleActiveTable = (tableName) => {
    setActiveTable(tableName);
    setSortBy(null);
    setSortOrder(null);
    setFilters({});
  }

  const applySort = (column, dir = 'asc') => {
    setSortBy(column);
    setSortOrder(dir);
  }

  useEffect(() => {
    fetchJson("/users")
    .then(users => {
      const map = {};
      users.forEach(u => {
        map[u.userID] = `${u.givenName} ${u.familyName[0]}`;
      });
      setUserMap(map);
    })
    .catch(err => console.error("User fetch failed:", err));
  }, []);

  return (
    <Stack style={{ minHeight: "100vh", backgroundColor: "black" }}>
      <img
        alt="RMIT Redbackbots Logo"
        src="/RedbacksLogo.png" // Adjust the path based on your folder structure
        width="170"
        className="d-inline-block align-top mt-4 ms-5"
      />
      
      <Container className="page-container">
        <div className='table-selection-box'>
          <Button className='table-button' onClick={() => handleActiveTable("Users")}>Users</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Equipment")}>Equipment</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Logs")}>Logs</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Notes")}>Notes</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Inductions")}>Inductions</Button>
        </div>
        <div className='sort-filter-box'>
          <Dropdown className='dropdown-button'>
            <Dropdown.Toggle
              variant="link"
              className="no-caret"
            >
              <FaSortAmountDown size={25} />
            </Dropdown.Toggle>

            <Dropdown.Menu>
              {activeTable === "Users" ? (
                <>
                  <Dropdown.Item onClick={() => applySort("userID", "asc")}><BsArrowUp /> UserID</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("userID", "desc")}><BsArrowDown /> UserID</Dropdown.Item>
                  <Dropdown.Divider />
                  <Dropdown.Item onClick={() => applySort("givenName", "asc")}><BsArrowUp /> Given Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("givenName", "desc")}><BsArrowDown /> Given Name</Dropdown.Item>
                  <Dropdown.Divider />
                  <Dropdown.Item onClick={() => applySort("familyName", "asc")}><BsArrowUp /> Family Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("familyName", "desc")}><BsArrowDown /> Family Name</Dropdown.Item>
                </>
              ) : activeTable === "Equipment" ? (
                <>
                  <Dropdown.Item onClick={() => applySort("equipmentName", "asc")}><BsArrowUp /> Equipment Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("equipmentName", "desc")}><BsArrowDown /> Equipment Name</Dropdown.Item>
                </>
               ) : activeTable === "Logs" ? (
                <>
                  <Dropdown.Item onClick={() => applySort("checkOutTime", "asc")}><BsArrowUp /> Check Out Time</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("checkOutTime", "desc")}><BsArrowDown /> Check Out Time</Dropdown.Item>
                  <Dropdown.Divider />
                  <Dropdown.Item onClick={() => applySort("checkInTime", "asc")}><BsArrowUp /> Check In Time</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("checkInTime", "desc")}><BsArrowDown /> Check In Time</Dropdown.Item>
                </>
              ) : activeTable === "Notes" ? (
                <>
                  <Dropdown.Item onClick={() => applySort("equipmentName", "asc")}><BsArrowUp /> Equipment Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("equipmentName", "desc")}><BsArrowDown /> Equipment Name</Dropdown.Item>
                  <Dropdown.Divider />
                  <Dropdown.Item onClick={() => applySort("timeOfNote", "asc")}><BsArrowUp /> Time Of Note</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("timeOfNote", "desc")}><BsArrowDown /> Time Of Note</Dropdown.Item>
                </>
              ) : null}

            </Dropdown.Menu>
          </Dropdown>

          <Dropdown autoClose="outside" className='dropdown-button'>
            <Dropdown.Toggle
              variant="link"
              className="no-caret"
            >
              <FaFilter size={25} />
            </Dropdown.Toggle>

            <Dropdown.Menu className='p-0'>
              {activeTable === "Users" ? (
                <>
                  <FiltersDropdown 
                    filters={filters} 
                    setFilters={setFilters} 
                    columnOrder={ userColumnOrder } 
                    booleanColumns={ userBooleanColumns }
                    columnLabels={ userColumnLabels }
                  />
                </>
                
              ) : activeTable === "Equipment" ? (
                <>
                  <FiltersDropdown 
                    filters={ filters } 
                    setFilters={ setFilters } 
                    columnOrder={ equipmentColumnOrder } 
                    booleanColumns={ equipmentBooleanColumns }
                    columnLabels={ equipmentColumnLabels }
                    filterColumns={ equipmentFilterColumns }
                    rows={ tableRows }
                  />
                </>
              ) : activeTable === "Logs" ? (
                <>
                  <FiltersDropdown
                    filters={ filters }
                    setFilters={ setFilters }
                    columnOrder={ logsColumnOrder }
                    columnLabels={ logsColumnLabels }
                    filterColumns={ logsFilterColumns }
                    rows={ tableRows }
                  />
                </>
              ) : activeTable === "Notes" ? (
                <>
                  <FiltersDropdown
                    filters={ filters }
                    setFilters={ setFilters }
                    columnOrder={ notesColumnOrder }
                    columnLabels={ notesColumnLabels }
                    filterColumns={ notesFilterColumns }
                    rows={ tableRows }
                  />
                </>
              ) : activeTable === "Inductions" ? (
                <>
                    <FiltersDropdown
                    filters={ filters }
                    setFilters={ setFilters }
                    columnOrder={ inductionsColumnOrder }
                    booleanColumns={ inductionsBooleanColumns }
                    columnLabels={ inductionsColumnLabels }
                    rows={ tableRows }
                  />
                </>
              ) : null}
            </Dropdown.Menu>
          </Dropdown>
        </div>

        <Container className="database-container">
          {activeTable === "Users" && <Users sortBy={sortBy} sortOrder={sortOrder} filters={filters} /> }
          {activeTable === "Equipment" && <Equipment sortBy={sortBy} sortOrder={sortOrder} filters={filters} onRowsLoaded={setTableRows} /> }
          {activeTable === "Logs" && <Logs sortBy={sortBy} sortOrder={sortOrder} userMap={userMap} filters={filters} onRowsLoaded={setTableRows} />}
          {activeTable === "Notes" && <Notes sortBy={sortBy} sortOrder={sortOrder} userMap={userMap} filters={filters} onRowsLoaded={setTableRows} />}
          {activeTable === "Inductions" && <Inductions sortBy={sortBy} sortOrder={sortOrder} userMap={userMap} filters={filters} onRowsLoaded={setTableRows} />}
        </Container>
        
      </Container>
    </Stack>
  )
}

export default App;
