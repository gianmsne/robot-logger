#include "dbUtils.h"

bool insertRobot(
    sqlite3* db,
    const std::string& robotName,
    const std::string& robotType,
    const std::string& robotCondition,
    const std::string& robotID,
    const std::string& location
) {
    const char* query = 
        "INSERT INTO robots (robotName, robotType, robotCondition, rmitID, location, isAvailable) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if(rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, robotName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, robotType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, robotCondition.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, robotID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, 1); // is_Available is set to 1 (True) by default

    // Execute
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
};

bool insertUser(
    sqlite3* db,
    const std::string& userID,
    const std::string& userGivenName,
    const std::string& userFamilyName,
    int isAdmin,
    int inducted
) {
    const char* query = 
        "INSERT INTO users (userID, givenName, familyName, isAdmin, isInducted) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if(rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, userGivenName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, userFamilyName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, isAdmin);
    sqlite3_bind_int(stmt, 5, inducted);

    // Execute
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE) {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
};


/*
    Check for existing item in a table.
        Parameters include the table, column and value to search for existing
*/
bool existenceCheck(sqlite3* db, std::string tableName, std::string columnName, std::string value){
    std::string query = "SELECT COUNT(*) FROM " + tableName +
                        " WHERE " + columnName + " = '" + value + "'";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_ROW) {
        std::cerr << "Existence Check Failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    // If count > 0, value exists
    return count > 0;
}


std::string getUserFromID(sqlite3* db, const std::string& id, std::string& givenName) {
    std::string query =
        "SELECT givenName, familyName FROM users WHERE userID = '" + id + "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return "";
    }

    // Read first column: Given_Name
    const unsigned char* givenNameText = sqlite3_column_text(stmt, 0);
    givenName = givenNameText ? reinterpret_cast<const char*>(givenNameText) : "";

    // Read second column: Family_Name
    const unsigned char* familyNameText = sqlite3_column_text(stmt, 1);
    std::string familyName = familyNameText ? reinterpret_cast<const char*>(familyNameText) : "";

    sqlite3_finalize(stmt);

    // Return concatenated full name
    return givenName + " " + familyName;
}


std::vector<std::string> getRobots(){
    std::vector<std::string> robotList;

    sqlite3* db;
    try {

        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }
    
    } catch (...) {
        std::cout << "There was an error while attempting to add the robot." << std::endl;
    }

    
    std::string query = "SELECT robotName FROM robots;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return robotList; // return empty vector on error
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            robotList.push_back(reinterpret_cast<const char*>(text));
        }
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error reading robots: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return robotList;
}

bool getAdminStatus(const std::string &id) {
    sqlite3* db = nullptr;

    if (sqlite3_open("database/robot_logger.db", &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // SQL query to get Is_Admin (0 or 1)
    std::string query = "SELECT isAdmin FROM users WHERE userID = '" + id + "';";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    bool isAdmin = false;

    if (rc == SQLITE_ROW) {
        // Read the first column as int
        int val = sqlite3_column_int(stmt, 0);
        isAdmin = (val != 0);  // 1 = true, 0 = false
    } else {
        std::cerr << "No user found for ID: " << id << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return isAdmin;
}