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

bool existenceCheck(sqlite3* db, std::string tableName, std::string columnName, std::string value);

std::string getUserFromID(sqlite3* db, const std::string &id);

std::vector<std::string> getRobots();

bool getAdminStatus(const std::string &id);