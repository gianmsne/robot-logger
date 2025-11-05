#include <sqlite3.h>
#include <string>
#include <iostream>

bool insertRobot(
    sqlite3* db,
    const std::string& robotName,
    const std::string& robotID,
    const std::string& location,
    const std::string& robotType,
    const std::string& robotCondition
);

bool insertUser(
    sqlite3* db,
    const std::string& userID,
    const std::string& userGivenName,
    const std::string& userFamilyName,
    int isAdmin,
    int inducted
);