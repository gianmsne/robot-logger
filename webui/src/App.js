import './App.css';
import './App.scss';
import { useEffect, useState } from "react";
import { Container, Stack } from "react-bootstrap";
import { FaSortAmountDown } from "react-icons/fa";
import { FaFilter } from "react-icons/fa6";
import { BsArrowUp, BsArrowDown } from "react-icons/bs";
import Button from 'react-bootstrap/Button';
import Dropdown from 'react-bootstrap/Dropdown';

import Users from './pages/Users';
import Robots from './pages/Robots';
import Logs from './pages/Logs';
import Notes from './pages/Notes';

import { fetchJson } from "./components/APIFetcher";

function App() {
  const [activeTable, setActiveTable] = useState("Users");

  const [sortBy, setSortBy] = useState(null);
  const [sortOrder, setSortOrder] = useState('asc');

  const [userMap, setUserMap] = useState({});
  
  const handleActiveTable = (tableName) => {
    setActiveTable(tableName);
    setSortBy(null);
    setSortOrder(null);
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
          <Button className='table-button' onClick={() => handleActiveTable("Robots")}>Robots</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Logs")}>Logs</Button>
          <Button className='table-button' onClick={() => handleActiveTable("Notes")}>Notes</Button>
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
              ) : activeTable === "Robots" ? (
                <>
                  <Dropdown.Item onClick={() => applySort("robotName", "asc")}><BsArrowUp /> Robot Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("robotName", "desc")}><BsArrowDown /> Robot Name</Dropdown.Item>
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
                  <Dropdown.Item onClick={() => applySort("robotName", "asc")}><BsArrowUp /> Robot Name</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("robotName", "desc")}><BsArrowDown /> Robot Name</Dropdown.Item>
                  <Dropdown.Divider />
                  <Dropdown.Item onClick={() => applySort("timeOfNote", "asc")}><BsArrowUp /> Time Of Note</Dropdown.Item>
                  <Dropdown.Item onClick={() => applySort("timeOfNote", "desc")}><BsArrowDown /> Time Of Note</Dropdown.Item>
                </>
              ) : null}

            </Dropdown.Menu>
          </Dropdown>

          <Dropdown className='dropdown-button'>
            <Dropdown.Toggle
              variant="link"
              className="no-caret"
            >
              <FaFilter size={25} />
            </Dropdown.Toggle>

            <Dropdown.Menu>

            </Dropdown.Menu>
          </Dropdown>
        </div>

        <Container className="database-container">
          {activeTable === "Users" && <Users sortBy={sortBy} sortOrder={sortOrder} />}
          {activeTable === "Robots" && <Robots sortBy={sortBy} sortOrder={sortOrder} />}
          {activeTable === "Logs" && <Logs sortBy={sortBy} sortOrder={sortOrder} userMap={userMap} />}
          {activeTable === "Notes" && <Notes sortBy={sortBy} sortOrder={sortOrder} userMap={userMap} />}
        </Container>
        
      </Container>
    </Stack>
  )
}

export default App;
