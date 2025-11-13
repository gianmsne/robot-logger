import './App.css';
import './App.scss';
import { useEffect, useState } from "react";
import { Container, Stack } from "react-bootstrap";
import { FaSortAmountDown } from "react-icons/fa";
import { FaFilter } from "react-icons/fa6";
import Button from 'react-bootstrap/Button';

import Users from './pages/Users';
import Robots from './pages/Robots';
import Logs from './pages/Logs';
import Notes from './pages/Notes';

function App() {
  const [activeTable, setActiveTable] = useState("Users");
  
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
          <Button className='table-button' onClick={() => setActiveTable("Users")}>Users</Button>
          <Button className='table-button' onClick={() => setActiveTable("Robots")}>Robots</Button>
          <Button className='table-button' onClick={() => setActiveTable("Logs")}>Logs</Button>
          <Button className='table-button' onClick={() => setActiveTable("Notes")}>Notes</Button>
        </div>
        <div className='sort-filter-box'>
          <FaSortAmountDown className="icon-button" />
          <FaFilter className="icon-button" />
        </div>

        <Container className="database-container">
          {activeTable === "Users" && <Users />}
          {activeTable === "Robots" && <Robots />}
          {activeTable === "Logs" && <Logs />}
          {activeTable === "Notes" && <Notes />}
        </Container>
        
      </Container>
    </Stack>
  )
}

export default App;
