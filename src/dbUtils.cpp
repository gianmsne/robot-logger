#include "dbUtils.h"

sqlite3 *globalDB = nullptr;

bool openDBConnection()
{
    if (globalDB != nullptr)
    {
        return true; // already open
    }

    if (sqlite3_open("database/robot_logger.db", &globalDB) != SQLITE_OK)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(globalDB) << std::endl;
        return false;
    }

    return true;
}

void closeDBConnection()
{
    if (globalDB != nullptr)
    {
        sqlite3_close(globalDB);
        globalDB = nullptr;
    }
}


/*
    Check-in and Check-out Database Utilities
*/
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
        const char* availabilityQuery = "SELECT isAvailable FROM equipment WHERE equipmentName = ?;";
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
                "UPDATE equipment SET permanentStatus = ? WHERE equipmentName = ?;";
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
        const char* equipmentUpdateQuery = "UPDATE equipment SET isAvailable = 1 WHERE equipmentName = ?;";
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

void addCheckOutRecord(std::string userID, std::string equipmentName) {

    time_t now = time(0);
    std::string name;

    std::cout << "\n------------- CHECK OUT CONFIRMED -------------" << std::endl;
    std::cout << " >> User: " << userID << ", " << getUserFromID(userID, name) <<  std::endl;
    std::cout << " >> Equipment Name: " << equipmentName << std::endl;
    std::cout << " >> Check Out Time: " << ctime(&now);

    try {
        sqlite3* db;
        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Check if equipment is available
        const char* availabilityQuery =
            "SELECT isAvailable FROM equipment WHERE equipmentName = ?;";

        sqlite3_stmt* availabilityStmt;
        int rc = sqlite3_prepare_v2(db, availabilityQuery, -1, &availabilityStmt, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }
        // Bind equipment name
        sqlite3_bind_text(availabilityStmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        // Execute
        rc = sqlite3_step(availabilityStmt);
        if(rc != SQLITE_ROW) {
            std::cerr << "Equipment not found: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(availabilityStmt);
            throw(0);
        }
        int isAvailable = sqlite3_column_int(availabilityStmt, 0);
        sqlite3_finalize(availabilityStmt);

        if(isAvailable == 0) {
            std::cout << " >> Equipment " << equipmentName << " is currently not available for check out." << std::endl;
            sqlite3_close(db);
            return;
        }


        // Insert check out record into database
        const char* query1 = 
            "INSERT INTO logs (checkOutUserID, equipmentName, checkOut) "
            "VALUES (?, ?, ?);";

        sqlite3_stmt* stmt1;
        rc = sqlite3_prepare_v2(db, query1, -1, &stmt1, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Bind values
        sqlite3_bind_text(stmt1, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt1, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt1, 3, static_cast<sqlite3_int64>(now));

        // Execute
        rc = sqlite3_step(stmt1);
        if(rc != SQLITE_DONE) {
            std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt1);
            throw(0);
        }

        sqlite3_finalize(stmt1);


        // Change availability status of equipment to checked out
        const char* query2 =
            "UPDATE equipment SET isAvailable = 0 WHERE equipmentName = ?;";

        sqlite3_stmt* stmt2;
        rc = sqlite3_prepare_v2(db, query2, -1, &stmt2, nullptr);
        if(rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        // Bind values
        sqlite3_bind_text(stmt2, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
        // Execute
        rc = sqlite3_step(stmt2);
        if(rc != SQLITE_DONE) {
            std::cerr << "Update failed: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt2);
            throw(0);
        }
        sqlite3_finalize(stmt2);

        sqlite3_close(db);

        // std::cout << "Check out record added successfully!" << std::endl;

    } catch (...) {
        std::cout << "There was an error while attempting to add the check out record." << std::endl;
    }
}


std::vector<std::string> getEquipmentCurrentlyCheckedOut() {
    std::vector<std::string> equipment;

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
            equipment.push_back(reinterpret_cast<const char*>(equipmentName));
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);

    } catch (...) {
        std::cerr << "Error while retrieving currently checked-out equipment." << std::endl;
    }

    return equipment;
}



/*
    Logger Functions
*/
bool insertEquipment(
    const std::string &equipmentName,
    const std::string &equipmentType,
    const std::string &equipmentCondition,
    const std::string &location)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query =
        "INSERT INTO equipment (equipmentName, equipmentType, equipmentCondition, location, isAvailable) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, equipmentType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, equipmentCondition.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, 1); // is_Available is set to 1 (True) by default

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
};

bool insertUser(
    const std::string &userID,
    const std::string &userGivenName,
    const std::string &userFamilyName,
    int isAdmin,
    int inductedNao,
    int inductedBooster,
    int inductedVRHeadset)
{
    openDBConnection();
    sqlite3 *db = globalDB;
    sqlite3_stmt *stmt;
    int rc;

    // Start transaction
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    {
        // -------- Insert into users --------
        const char *query =
            "INSERT INTO users (userID, givenName, familyName, isAdmin) "
            "VALUES (?, ?, ?, ?);";

        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) goto rollback;

        // Bind values
        sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, userGivenName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, userFamilyName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, isAdmin);


        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) goto rollback;
    }

    {
        // -------- Insert into inductions --------
        const char *query2 =
            "INSERT INTO inductions (userID, isInductedNao, isInductedBooster, isInductedVRHeadset) "
            "VALUES (?, ?, ?, ?);";

        rc = sqlite3_prepare_v2(db, query2, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) goto rollback;

        sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, inductedNao);
        sqlite3_bind_int(stmt, 3, inductedBooster);
        sqlite3_bind_int(stmt, 4, inductedVRHeadset);

        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if (rc != SQLITE_DONE) goto rollback;
    }

    // Commit transaction
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;

rollback:
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    if (stmt) sqlite3_finalize(stmt);
    std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
    return false;
};

/*
    Check for existing item in a table.
        Parameters include the table, column and value to search for existing
*/
bool existenceCheck(std::string tableName, std::string columnName, std::string value)
{
    bool exists = false;

    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query = "SELECT COUNT(*) FROM " + tableName +
                        " WHERE " + columnName + " = '" + value + "'";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        exists = false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        std::cerr << "Existence Check Failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        exists = false;
    }

    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    // If count > 0, value exists
    if (count > 0)
    {
        exists = true;
    };
    return exists;
}

std::string getUserFromID(const std::string &id, std::string &givenName)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query =
        "SELECT givenName, familyName FROM users WHERE userID = '" + id + "';";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        return "";
    }

    // Read first column: Given_Name
    const unsigned char *givenNameText = sqlite3_column_text(stmt, 0);
    givenName = givenNameText ? reinterpret_cast<const char *>(givenNameText) : "";

    // Read second column: Family_Name
    const unsigned char *familyNameText = sqlite3_column_text(stmt, 1);
    std::string familyName = familyNameText ? reinterpret_cast<const char *>(familyNameText) : "";

    sqlite3_finalize(stmt);

    // Return concatenated full name
    return givenName + " " + familyName;
}

std::string getCheckOutIdFromEquipment(const std::string &equipmentName)
{
    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query =
        "SELECT checkOutUserID FROM logs WHERE equipmentName = ? ORDER BY checkOut DESC LIMIT 1;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Use parameter binding to safely insert equipmentName
    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_STATIC);

    std::string checkOutId;

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text)
            checkOutId = reinterpret_cast<const char *>(text);
    }
    else if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute query: "
                  << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    closeDBConnection();

    return checkOutId;
}

std::vector<std::string> getEquipment()
{
    std::vector<std::string> equipmentList;

    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query = "SELECT equipmentName FROM equipment WHERE isAvailable = 1;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return equipmentList; // return empty vector on error
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text)
        {
            equipmentList.push_back(reinterpret_cast<const char *>(text));
        }
    }

    if (rc != SQLITE_DONE)
    {
        std::cerr << "Error reading equipment: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return equipmentList;
}

std::string getEquipmentStatus(const std::string &equipmentName)
{
    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query =
        "SELECT permanentStatus FROM equipment WHERE equipmentName = ?";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: "
                  << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Use parameter binding to safely insert equipmentName
    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_STATIC);

    std::string status;

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        if (text)
            status = reinterpret_cast<const char *>(text);
    }
    else if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute query: "
                  << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    closeDBConnection();

    return status;
}

bool getAdminStatus(const std::string &id)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    // SQL query to get Is_Admin (0 or 1)
    std::string query = "SELECT isAdmin FROM users WHERE userID = '" + id + "';";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    rc = sqlite3_step(stmt);
    bool isAdmin = false;

    if (rc == SQLITE_ROW)
    {
        // Read the first column as int
        int val = sqlite3_column_int(stmt, 0);
        isAdmin = (val != 0); // 1 = true, 0 = false
    }
    // else
    // {
    //     //std::cerr << "No user found for ID: " << id << std::endl;
    // }

    sqlite3_finalize(stmt);

    return isAdmin;
}

void updateGivenName(const std::string &id, const std::string &newGivenName)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE users SET givenName = ? WHERE userID = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, newGivenName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
}

void updateFamilyName(const std::string &id, const std::string &newFamilyName)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE users SET familyName = ? WHERE userID = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, newFamilyName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateAdminStatus(const std::string &id, const int& isAdmin)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE users SET isAdmin = ? WHERE userID = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_int(stmt, 1, isAdmin);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateInductionsStatus(const std::string &id, const int& inducted, const std::string &inductionsType)
{

    openDBConnection();
    sqlite3 *db = globalDB;

    std::string query = "UPDATE inductions SET " + inductionsType + " = ? WHERE userID = ?;";


    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_int(stmt, 1, inducted);
    sqlite3_bind_text(stmt, 2, id.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateType(const std::string& equipmentName, const std::string& equipmentType){
    
    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE equipment SET equipmentType = ? WHERE equipmentName = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, equipmentType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateCondition(const std::string& equipmentName, const std::string& equipmentCondition){

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE equipment SET equipmentCondition = ? WHERE equipmentName = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, equipmentCondition.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateLocation(const std::string& equipmentName, const std::string& location){

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE equipment SET location = ? WHERE equipmentName = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, location.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};

void updateAvailability(const std::string& equipmentName, const int& isAvailable){

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "UPDATE equipment SET isAvailable = ? WHERE equipmentName = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_int(stmt, 1, isAvailable);
    sqlite3_bind_text(stmt, 2, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    return;
};


void removeEquipment(const std::string& equipmentName){

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "DELETE FROM equipment WHERE equipmentName = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
    std::cout << "Equipment " + equipmentName + " has been removed from the database." << std::endl;
    return;
};

void removeUser(const std::string& id){

    openDBConnection();
    sqlite3 *db = globalDB;

    const char *query = "DELETE FROM users WHERE userID = ? ";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Delete failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
        std::cout << "User " + id + " has been removed from the database." << std::endl;
    return;
};


void addNote(const std::string &equipmentName,
             const std::string &noteText,
             const std::string &noteLeftBy)
{
    if (!openDBConnection()) {
        std::cerr << "DB connection failed\n";
        return;
    }

    time_t now = time(0); // example: "Tue Nov 12 21:21:52 2025\n"

    sqlite3 *db = globalDB;

    const char *notesQuery =
        "INSERT INTO notes (equipmentName, note, noteLeftBy, timeOfNote) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, notesQuery, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, noteText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, noteLeftBy.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 4, static_cast<sqlite3_int64>(now));

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insert note failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);
}

std::vector<std::string> getEquipmentNotes(const std::string &equipmentName) {
    std::vector<std::string> notesList;

    if (!openDBConnection()) {
        std::cerr << "DB connection failed\n";
        return notesList;
    }

    sqlite3* db = globalDB;

    const char* query =
        "SELECT note, noteLeftBy, timeOfNote FROM notes "
        "WHERE equipmentName = ? ORDER BY timeOfNote DESC;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return notesList;
    }

    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* noteText = sqlite3_column_text(stmt, 0);
        const unsigned char* noteByID = sqlite3_column_text(stmt, 1);
        int64_t timeOfNote = sqlite3_column_int64(stmt, 2);

        std::string noteByName;
        if (noteByID) {
            std::string dummy;
            noteByName = getUserFromID(reinterpret_cast<const char*>(noteByID), dummy);
        }

        std::string formattedNote;
        if (noteText) formattedNote += reinterpret_cast<const char*>(noteText);
        formattedNote += " | ";
        formattedNote += noteByName;
        formattedNote += " | ";
        formattedNote += ctime(reinterpret_cast<time_t*>(&timeOfNote));

        if (!formattedNote.empty() && formattedNote.back() == '\n') {
            formattedNote.pop_back();
        }

        notesList.push_back(formattedNote);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error reading notes: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return notesList;
}

std::string getMostRecentNote(const std::string &equipmentName) {
    if (!openDBConnection()) {
        std::cerr << "DB connection failed\n";
        return "";
    }

    sqlite3* db = globalDB;

    const char* query =
        "SELECT note, noteLeftBy FROM notes "
        "WHERE equipmentName = ? "
        "ORDER BY timeOfNote DESC LIMIT 1;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    sqlite3_bind_text(stmt, 1, equipmentName.c_str(), -1, SQLITE_TRANSIENT);

    std::string result;

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* noteText = sqlite3_column_text(stmt, 0);
        const unsigned char* noteByID = sqlite3_column_text(stmt, 1);

        std::string noteStr = noteText ? reinterpret_cast<const char*>(noteText) : "";
        std::string userStr;

        if (noteByID) {
            std::string dummyGivenName;
            userStr = getUserFromID(reinterpret_cast<const char*>(noteByID), dummyGivenName);
        } else {
            userStr = "Unknown";
        }

        result = noteStr + " (" + userStr + ")";
    } else if (rc != SQLITE_DONE) {
        std::cerr << "Error reading most recent note: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return result;
}