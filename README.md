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
  - Users can log in using their ID (Tapping their card on an RFID reader or with manual entry).
  - Admin users have additional privileges.

- **Robot Check-Out**
  - View a list of available robots.
  - Check out a robot and record the action as a log.

- **Robot Check-In**
  - View robots currently checked out by the logged-in user.
  - Check in robots and optionally leave notes about their condition.

- **Admin Features**
  - Add/modify robots.
  - Add/modify users.

- **Notes**
  - Users can leave notes on robot condition when checking in.
  - Notes are stored with the robot name, user ID, and timestamp.

- **Database**
  - SQLite is used for data storage.
  - Tables include `users`, `robots`, `logs`, and `notes`.


### Command-Line Flags

This application supports optional flags that modify its behavior at runtime. Flags are passed when running the executable:

```
./robotLogger [flags]
```

By default, the program runs with both RFID scanning and manual entry enabled.

#### Available Flags

| Flag        | Description                                                                 |
|------------|-----------------------------------------------------------------------------|
| `-camera`    | Enables barcode scanning through the camera. A GUI will appear while waiting for a barcode. Requires a camera to operate. |
| `-headless` | Runs the barcode scanning in headless mode. The barcode scanner will not display a camera window, useful for terminals without a GUI. If the program does not detect a barcode in 10 seconds, it will request manual entry. Requires a camera to operate. |



# Installation

Clone the project repository:
```
git clone https://github.com/gianmsne/robot-logger.git
cd robot-logger
```

#### Dependencies: 
- **SQLite3 development libraries** (`libsqlite3-dev`)  
  - Required for database storage and management of users, robots, logs, and notes.
- **OpenCV** (`libopencv-dev`)  
  - Provides computer vision support for camera input when scanning barcodes.  
- **ZBar** (`libzbar-dev`)  
  - Barcode scanning library used for reading user ID card barcodes.

*You can either manually install dependencies or run the provided setup scripts for your OS:*

## Raspberry Pi / Debian / Ubuntu

#### Option 1 - Manual Installation:

Update system packages and install build tools and libraries:
```
sudo apt update
sudo apt upgrade -y
sudo apt install -y g++ make pkg-config cmake git libsqlite3-dev libopencv-dev libzbar-dev
```

#### Option 2 - Run the setup scripts:
```
chmod +x setups/setup_linux.sh setups/setup_database.sh
./setups/setup_linux.sh && ./setups/setup_database.sh
```

#### Build and Run:
```
make
./robotLogger
```

## macOS (Homebrew)


#### Option 1 - Manual Installation:
Using homebrew:
```
brew update
brew install opencv zbar sqlite3
```


#### Option 2 - Run the setup scripts:
```
chmod +x setups/setup_linux.sh setups/setup_database.sh
./setups/setup_macos.sh && ./setups/setup_database.sh
```


#### Build and Run:
```
make
./robotLogger
```

---

# Running the react webapp for database access

## Building the webapp for the first time

Make sure you have Docker installed and running. In the root directory of the repository, run:
```
docker compose build
```

## Run the webapp
Run the Docker Compose with:
```
docker compose up
```
