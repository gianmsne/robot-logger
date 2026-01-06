#include "checkIn.h"

void addCheckInRecord(std::string userID, std::string equipmentName, std::string notes, std::string permStatus) {
    time_t now = time(0);

    std::cout << "\n------------- CHECK IN CONFIRMED -------------" << std::endl;
    std::cout << " >> User ID: " << userID << std::endl;
    std::cout << " >> Equipment Name: " << equipmentName << std::endl;
    std::cout << " >> Check In Time: " << ctime(&now);
    if(!notes.empty()) { std::cout << " >> Note left: " << notes << std::endl; }
    else { std::cout << " >> No note left." << std::endl; }
    if(!permStatus.empty()) { std::cout << " >> New Permanent Status: " << permStatus << std::endl; }

    try {
        sqlite3* db;
        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Start transaction
        char* errMsg = nullptr;
        if(sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Failed to begin transaction: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            throw(0);
        }

        // Check equipment availability
        const char* availabilityQuery = "SELECT isAvailable FROM equipments WHERE equipmentName = ?;";
        sqlite3_stmt* availabilityStmt;
        int rc = sqlite3_prepare_v2(db, availabilityQuery, -1, &availabilityStmt, nullptr);
        if(rc != SQLITE_OK) throw(0);
        sqlite3_bind_text(availabilityStmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(availabilityStmt);
        if(rc != SQLITE_ROW) {
            std::cerr << "Equipment not found" << std::endl;
            sqlite3_finalize(availabilityStmt);
            throw(0);
        }
        int isAvailable = sqlite3_column_int(availabilityStmt, 0);
        sqlite3_finalize(availabilityStmt);

        if(isAvailable == 1) {
            std::cout << " >> Equipment " << equipmentName << " is not currently checked out." << std::endl;
            sqlite3_close(db);
            return;
        }

        // Update logs table
        const char* logsQuery =
            "UPDATE logs SET checkIn = ?, checkInUserID = ? WHERE equipmentName = ? AND checkIn IS NULL;";
        sqlite3_stmt* logsStmt;
        rc = sqlite3_prepare_v2(db, logsQuery, -1, &logsStmt, nullptr);
        if(rc != SQLITE_OK) throw(0);
        sqlite3_bind_int64(logsStmt, 1, static_cast<sqlite3_int64>(now));
        sqlite3_bind_text(logsStmt, 2, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(logsStmt, 3, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(logsStmt);
        if(rc != SQLITE_DONE) {
            std::cerr << "Update logs failed" << std::endl;
            sqlite3_finalize(logsStmt);
            throw(0);
        }
        sqlite3_finalize(logsStmt);

        // Insert note if any
        if(!notes.empty()) {
            const char* notesQuery =
                "INSERT INTO notes (equipmentName, note, noteLeftBy, timeOfNote) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* notesStmt;
            rc = sqlite3_prepare_v2(db, notesQuery, -1, &notesStmt, nullptr);
            if(rc != SQLITE_OK) throw(0);
            sqlite3_bind_text(notesStmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(notesStmt, 2, notes.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(notesStmt, 3, userID.c_str(), -1, SQLITE_TRANSIENT); // use userID
            sqlite3_bind_int64(notesStmt, 4, static_cast<sqlite3_int64>(now));
            rc = sqlite3_step(notesStmt);
            if(rc != SQLITE_DONE) {
                std::cerr << "Insert note failed" << std::endl;
                sqlite3_finalize(notesStmt);
                throw(0);
            }
            sqlite3_finalize(notesStmt);
        }

        // Update permanent status if any
        if(!permStatus.empty()) {
            const char* statusQuery =
                "UPDATE equipments SET permanentStatus = ? WHERE equipmentName = ?;";
            sqlite3_stmt* statusStmt;
            rc = sqlite3_prepare_v2(db, statusQuery, -1, &statusStmt, nullptr);
            if(rc != SQLITE_OK) throw(0);
            sqlite3_bind_text(statusStmt, 1, permStatus.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(statusStmt, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

            rc = sqlite3_step(statusStmt);
            if(rc != SQLITE_DONE) {
                std::cerr << "Insert note failed" << std::endl;
                sqlite3_finalize(statusStmt);
                throw(0);
            }
            sqlite3_finalize(statusStmt);
        }

        // Update equipment availability
        const char* equipmentUpdateQuery = "UPDATE equipments SET isAvailable = 1 WHERE equipmentName = ?;";
        sqlite3_stmt* equipmentStmt;
        rc = sqlite3_prepare_v2(db, equipmentUpdateQuery, -1, &equipmentStmt, nullptr);
        if(rc != SQLITE_OK) throw(0);
        sqlite3_bind_text(equipmentStmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(equipmentStmt);
        if(rc != SQLITE_DONE) {
            std::cerr << "Update equipment failed" << std::endl;
            sqlite3_finalize(equipmentStmt);
            throw(0);
        }
        sqlite3_finalize(equipmentStmt);

        // Commit transaction
        if(sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::cerr << "Commit failed: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            throw(0);
        }

        sqlite3_close(db);

    } catch (...) {
        std::cout << "There was an error while attempting to add the check in record." << std::endl;
    }
}


std::vector<std::string> getEquipmentsCurrentlyCheckedOut() {
    std::vector<std::string> equipments;

    try {
        sqlite3* db;

        // --- OPEN DATABASE ---
        if (sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // --- SQL QUERY ---
        const char* query =
            "SELECT equipmentName "
            "FROM logs "
            "WHERE checkIn IS NULL;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // --- EXECUTE ---
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            const unsigned char* equipmentName = sqlite3_column_text(stmt, 0);
            equipments.push_back(reinterpret_cast<const char*>(equipmentName));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (...) {
        std::cerr << "Error while retrieving currently checked-out equipments." << std::endl;
    }

    return equipments;
}