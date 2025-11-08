#include "checkIn.h"

void addCheckInRecord(std::string userID, std::string robotName, std::string notes) {

    time_t now = time(0);

    std::cout << "\n------------- CHECK IN ROBOT -------------" << std::endl;
    std::cout << " >> User ID: " << userID << std::endl;
    std::cout << " >> Robot Name: " << robotName << std::endl;
    std::cout << " >> Check In Time: " << ctime(&now) << std::endl;

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

        if(isAvailable == 1) {
            std::cout << " >> Robot " << robotName << " is not currently checked out." << std::endl;
            sqlite3_close(db);
            return;
        }

        // Insert check out record into database
        // Update the logs table: set checkIn time for this user/robot
        const char* query1 =
            "UPDATE logs "
            "SET checkIn = ?, notes = ? "
            "WHERE userID = ? AND robotName = ? AND checkIn IS NULL;";

        sqlite3_stmt* stmt1;
        rc = sqlite3_prepare_v2(db, query1, -1, &stmt1, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Bind values
        sqlite3_bind_int64(stmt1, 1, static_cast<sqlite3_int64>(now));
        sqlite3_bind_text(stmt1, 2, notes.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt1, 3, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt1, 4, robotName.c_str(), -1, SQLITE_TRANSIENT);

        // Execute
        rc = sqlite3_step(stmt1);
        if(rc != SQLITE_DONE) {
            std::cerr << "Update failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt1);
            throw(0);
        }

        sqlite3_finalize(stmt1);


        // Change availability status of robot to checked in
        const char* query2 =
            "UPDATE robots SET isAvailable = 1 WHERE robotName = ?;";

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

        std::cout << "Check in record added successfully!" << std::endl;

    } catch (...) {
        std::cout << "There was an error while attempting to add the check in record." << std::endl;
    }
}


std::vector<std::string> getRobotsCheckedOutByUser(const std::string& userID) {
    std::vector<std::string> robots;

    try {
        sqlite3* db;

        // --- OPEN DATABASE ---
        if (sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // --- SQL QUERY ---
        const char* query =
            "SELECT robotName "
            "FROM logs "
            "WHERE userID = ? AND checkIn IS NULL;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // --- BIND ---
        sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);

        // --- EXECUTE ---
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            const unsigned char* robotName = sqlite3_column_text(stmt, 0);
            robots.push_back(reinterpret_cast<const char*>(robotName));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (...) {
        std::cerr << "Error while retrieving robots checked out by user." << std::endl;
    }

    return robots;
}