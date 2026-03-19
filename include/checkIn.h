#ifndef CHECKIN_H
#define CHECKIN_H

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "User.h"


void addCheckInRecord(std::string userID, std::string equipmentName, std::string notes, std::string permStatus);
std::vector<std::string> getEquipmentCurrentlyCheckedOut();

#endif // CHECKIN_H