#include <sqlite3.h>
#include <string>
#include <iostream>
#include "menuUtils.h"

bool insertEquipment(
    const std::string& equipmentName,
    const std::string& location,
    const std::string& equipmentType,
    const std::string& equipmentCondition
);

bool insertUser(
    const std::string& userID,
    const std::string& userGivenName,
    const std::string& userFamilyName,
    int isAdmin,
    int inductedNao,
    int inductedBooster,
    int inductedVRHeadset
);

bool openDBConnection();
void closeDBConnection();

// Checkout and Checkin Utils
void addCheckInRecord(std::string userID, std::string equipmentName, std::string notes, std::string permStatus);
void addCheckOutRecord(std::string userID, std::string equipmentName);
std::vector<std::string> getEquipmentCurrentlyCheckedOut();


std::string getUserFromID(const std::string &id, std::string &givenName);
std::string getCheckOutIdFromEquipment(const std::string &equipmentName);
std::string getEquipmentStatus(const std::string &equipmentName);
std::vector<std::string> getEquipment();
bool existenceCheck(std::string tableName, std::string columnName, std::string value);
bool getAdminStatus(const std::string &id);

// Update Users
void updateGivenName(const std::string& id, const std::string& newGivenName);
void updateFamilyName(const std::string& id, const std::string& newFamilyName);
void updateAdminStatus(const std::string& id, const int& isAdmin);
void updateInductionsStatus(const std::string& id, const int& inducted, const std::string& inductionsType);

// Update Equipment
void updateType(const std::string& equipmentName, const std::string& equipmentType);
void updateCondition(const std::string& equipmentName, const std::string& equipmentCondition);
void updateLocation(const std::string& equipmentName, const std::string& location);
void updateAvailability(const std::string& equipmentName, const int& isAvailable);

void removeEquipment(const std::string& equipmentName);
void removeUser(const std::string& userID);

void addNote(const std::string &equipmentName, const std::string &noteText, const std::string &noteLeftBy);
std::string getMostRecentNote(const std::string &equipmentName);
std::vector<std::string> getEquipmentNotes(const std::string &equipmentName);