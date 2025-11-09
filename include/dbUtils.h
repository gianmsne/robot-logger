#include <sqlite3.h>
#include <string>
#include <iostream>
#include "menuUtils.h"

bool insertRobot(
    const std::string& robotName,
    const std::string& robotID,
    const std::string& location,
    const std::string& robotType,
    const std::string& robotCondition
);

bool insertUser(
    const std::string& userID,
    const std::string& userGivenName,
    const std::string& userFamilyName,
    int isAdmin,
    int inducted
);

bool openDBConnection();
void closeDBConnection();

bool existenceCheck(std::string tableName, std::string columnName, std::string value);
std::string getUserFromID(const std::string &id, std::string &givenName);
std::vector<std::string> getRobots();
bool getAdminStatus(const std::string &id);

// Update Users
void updateGivenName(const std::string& id, const std::string& newGivenName);
void updateFamilyName(const std::string& id, const std::string& newFamilyName);
void updateAdminStatus(const std::string& id, const int& isAdmin);
void updateInductionStatus(const std::string& id, const int& inducted);

// Update Robots
void updateType(const std::string& robotName, const std::string& robotType);
void updateCondition(const std::string& robotName, const std::string& robotCondition);
void updateRobotID(const std::string& robotName, const std::string& robotID);
void updateLocation(const std::string& robotName, const std::string& location);
void updateAvailability(const std::string& robotName, const int& isAvailable);

void removeRobot(const std::string& robotName);
void removeUser(const std::string& userID);