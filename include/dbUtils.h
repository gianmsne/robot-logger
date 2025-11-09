#include <sqlite3.h>
#include <string>
#include <iostream>

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
void updateGivenName(const std::string& id, const std::string& newGivenName);
void updateFamilyName(const std::string& id, const std::string& newFamilyName);
void updateAdminStatus(const std::string& id, int isAdmin);
void updateInductionStatus(const std::string& id, int inducted);