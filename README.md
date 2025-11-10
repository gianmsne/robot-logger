# Robot Check-In / Check-Out System

This is a C++ command-line application that allows users to check out and check in robots, track usage, and leave notes about robot conditions. It uses an SQLite database to store user, robot, log, and note information.
</br></br>

<p align="center">
  <img src="nao-icon.png" width="160">
</p>
</br>

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


# Installation

Clone the project repository:
```
git clone https://github.com/gianmsne/robot-logger.git
cd robot-logger
```

## Raspberry Pi / Debian / Ubuntu

Update system packages and install build tools and dependencies:
```
sudo apt update
sudo apt upgrade -y
sudo apt install -y g++ make pkg-config cmake git libsqlite3-dev libopencv-dev libzbar-dev 
```

*Project specific dependencies:*
```
sudo apt install libsqlite3-dev libopencv-dev libzbar-dev
```

Build the project and run the program:
```
make
./robotLogger
```


## macOS (Homebrew)

Install dependencies:
```
brew update
brew install opencv zbar sqlite3
```

Build the project and run the program::
```
make
./robotLogger
```

---