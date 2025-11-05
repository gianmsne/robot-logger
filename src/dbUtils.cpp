#include "dbUtils.h"

bool insertRobot(
    sqlite3* db,
    const std::string& robotName,
    const std::string& robotType,
    const std::string& robotCondition,
    const std::string& robotID,
    const std::string& location
) {
    const char* sql = 
        "INSERT INTO robots (Robot_Name, Robot_Type, Robot_Condition, RMIT_ID, Location, Last_Used_Person, Last_Used_Time, Notes, Is_Available) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
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
    sqlite3_bind_null(stmt, 6);
    sqlite3_bind_null(stmt, 7);
    sqlite3_bind_null(stmt, 8);
    sqlite3_bind_int(stmt, 9, 1); // is_Available is set to 1 (True) by default

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
    const char* sql = 
        "INSERT INTO users (User_ID, Given_Name, Family_Name, Is_Admin, Inducted) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
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