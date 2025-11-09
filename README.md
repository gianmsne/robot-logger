# Robot Check-In / Check-Out System

This is a C++ command-line application that allows users to check out and check in robots, track usage, and leave notes about robot conditions. It uses an SQLite database to store user, robot, log, and note information.

<p align="center">
  <img src="nao-icon.png" width="160">
</p>

---

## Features

- **User Login**
  - Users can log in using their ID (simulating a card tap or manual entry).
  - Admin users have additional privileges.

- **Robot Check-Out**
  - View a list of available robots.
  - Check out a robot and record the transaction.

- **Robot Check-In**
  - View robots currently checked out by the logged-in user.
  - Check in robots and optionally leave notes about their condition.

- **Admin Features**
  - Add new robots.
  - Add new users.

- **Notes**
  - Users can leave notes on robot condition when checking in.
  - Notes are stored with the robot name, user ID, and timestamp.

- **Database**
  - SQLite is used for data storage.
  - Tables include `users`, `robots`, `logs`, and `notes`.

---
