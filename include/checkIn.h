#ifndef CHECKIN_H
#define CHECKIN_H

#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include "User.h"


void addCheckInRecord(std::string userID, std::string robotName, std::string notes);
std::vector<std::string> getRobotsCurrentlyCheckedOut();

#endif // CHECKOUT_H