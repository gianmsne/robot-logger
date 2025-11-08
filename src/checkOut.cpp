#include "checkOut.h"

#include <sqlite3.h>


void addCheckOutRecord(std::string userID, std::string robotName) {

    time_t now = time(0);

    std::cout << "\n------------- CHECK OUT ROBOT -------------" << std::endl;
    std::cout << " >> User ID: " << userID << std::endl;
    std::cout << " >> Robot Name: " << robotName << std::endl;
    std::cout << " >> Check Out Time: " << ctime(&now) << std::endl;

    try {
        sqlite3* db;
        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Check if robot is available
        const char* availabilityQuery =
            "SELECT isAvailable FROM robots WHERE robotName = ?;";

        sqlite3_stmt* availabilityStmt;
        int rc = sqlite3_prepare_v2(db, availabilityQuery, -1, &availabilityStmt, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }
        // Bind robot name
        sqlite3_bind_text(availabilityStmt, 1, robotName.c_str(), -1, SQLITE_TRANSIENT);
        // Execute
        rc = sqlite3_step(availabilityStmt);
        if(rc != SQLITE_ROW) {
            std::cerr << "Robot not found: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(availabilityStmt);
            throw(0);
        }
        int isAvailable = sqlite3_column_int(availabilityStmt, 0);
        sqlite3_finalize(availabilityStmt);

        if(isAvailable == 0) {
            std::cout << " >> Robot " << robotName << " is currently not available for check out." << std::endl;
            sqlite3_close(db);
            return;
        }


        // Insert check out record into database
        const char* query1 = 
            "INSERT INTO logs (userID, robotName, checkOut) "
            "VALUES (?, ?, ?);";

        sqlite3_stmt* stmt1;
        rc = sqlite3_prepare_v2(db, query1, -1, &stmt1, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Bind values
        sqlite3_bind_text(stmt1, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt1, 2, robotName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt1, 3, static_cast<sqlite3_int64>(now));

        // Execute
        rc = sqlite3_step(stmt1);
        if(rc != SQLITE_DONE) {
            std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt1);
            throw(0);
        }

        sqlite3_finalize(stmt1);


        // Change availability status of robot to checked out
        const char* query2 =
            "UPDATE robots SET isAvailable = 0 WHERE robotName = ?;";

        sqlite3_stmt* stmt2;
        rc = sqlite3_prepare_v2(db, query2, -1, &stmt2, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Bind values
        sqlite3_bind_text(stmt2, 1, robotName.c_str(), -1, SQLITE_TRANSIENT);
        // Execute
        rc = sqlite3_step(stmt2);
        if(rc != SQLITE_DONE) {
            std::cerr << "Update failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt2);
            throw(0);
        }
        sqlite3_finalize(stmt2);

        sqlite3_close(db);

        std::cout << "Check out record added successfully!" << std::endl;

    } catch (...) {
        std::cout << "There was an error while attempting to add the check out record." << std::endl;
    }
}