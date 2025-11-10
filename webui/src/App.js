import './App.css';
import './App.scss';
import { useEffect, useState } from "react";
import { Container, Stack } from "react-bootstrap";
import Users from './pages/Users';

function App() {
  return (
    <Stack style={{ minHeight: "100vh", backgroundColor: "black" }}>
      <img
        alt="RMIT Redbackbots Logo"
        src="/RedbacksLogo.png" // Adjust the path based on your folder structure
        width="128"
        height="51"
        className="d-inline-block align-top mt-4 ms-5"
      />
      
      <Container className="page-container">
        <Container className="database-container">
          <Users />
        </Container>
        
      </Container>
    </Stack>
  )
}

export default App;
