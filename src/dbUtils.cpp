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
    sqlite3_bind_text(stmt, 1, robotName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, robotType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, robotCondition.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, robotID.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, location.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, "None", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, "None", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, "None", -1, SQLITE_STATIC);
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
}