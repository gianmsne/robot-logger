#include <sqlite3.h>
#include <string>
#include <iostream>
#include "menuUtils.h"

bool insertEquipment(
    const std::string& equipmentName,
    const std::string& equipmentID,
    const std::string& location,
    const std::string& equipmentType,
    const std::string& equipmentCondition
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
std::vector<std::string> getEquipments();
std::string getCheckOutIdFromEquipment(const std::string &equipmentName);
bool getAdminStatus(const std::string &id);
std::string getEquipmentStatus(const std::string &equipmentName);

// Update Users
void updateGivenName(const std::string& id, const std::string& newGivenName);
void updateFamilyName(const std::string& id, const std::string& newFamilyName);
void updateAdminStatus(const std::string& id, const int& isAdmin);
void updateInductionStatus(const std::string& id, const int& inducted);

// Update Equipments
void updateType(const std::string& equipmentName, const std::string& equipmentType);
void updateCondition(const std::string& equipmentName, const std::string& equipmentCondition);
void updateEquipmentID(const std::string& equipmentName, const std::string& equipmentID);
void updateLocation(const std::string& equipmentName, const std::string& location);
void updateAvailability(const std::string& equipmentName, const int& isAvailable);

void removeEquipment(const std::string& equipmentName);
void removeUser(const std::string& userID);

void addNote(const std::string &equipmentName, const std::string &noteText, const std::string &noteLeftBy);
std::string getMostRecentNote(const std::string &equipmentName);
std::vector<std::string> getEquipmentNotes(const std::string &equipmentName);