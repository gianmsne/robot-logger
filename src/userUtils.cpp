#include "userUtils.h"
#include "menuUtils.h"
#include "dbUtils.h"

#include <sqlite3.h>

void addUser() {

    std::string userID;
    std::string userGivenName;
    std::string userFamilyName;
    int isAdmin;
    int inducted;

    char input;

    std::cout << std::endl;
    std::cout << "------------- ADD USER -------------" << std::endl;

    std::cout << " >> Tap Your User ID Card (or enter manually): ";
    std::cin >> userID;

    std::cout << " >> Enter User Given Name: ";
    std::cin >> userGivenName;

    std::cout << " >> Enter User Family Name: ";
    std::cin >> userFamilyName;

    std::cout << " >> Is Admin (y/n): ";
    std::cin >> input;
    if (input == 'y' || input == 'Y') isAdmin = 1;
    else isAdmin = 0;

    std::cout << " >> Is Inducted (y/n): ";
    std::cin >> input;
    if (input == 'y' || input == 'Y') inducted = 1;
    else inducted = 0;

    std::cout << std::endl;

    try {
        sqlite3* db;
        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }

        if(insertUser(userID, userGivenName, userFamilyName, isAdmin, inducted)) {
            std::cout << "User added successfully!" << std::endl;
        } else {
            throw(0);
        }

        sqlite3_close(db);

    } catch (...) {
        std::cout << "There was an error while attempting to add the user." << std::endl;
        std::cout << "Please try again" << std::endl;
    }
}

void modifyUser() {

    std::string userID;
    std::string userGivenName;
    std::string userFamilyName;
    int isAdmin = 0;
    int inducted = 0;

    char input;

    std::cout << std::endl;
    std::cout << "------------- MODIFY USER -------------" << std::endl;

    std::cout << ">> Tap User ID Card (or enter manually) to modify: s";
    std::cin >> userID;

    while(!existenceCheck("users", "userID", userID)){
        std::cout << "User ID does not exist. Try Again: s";
        std::cin >> userID;
    };

    int choice = -1;
    while(choice != 5){
        std::cout << "\nUser Settings for: s" + userID + ", " + getUserFromID(userID, userGivenName) << std::endl;
        printModifyUserMenu();

        choice = getIntInput(1,5);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        switch(choice){
            case 1:
                std::cout << "Enter new Given Name: ";
                std::cin >> userGivenName;
                updateGivenName(userID, userGivenName);
                break;
            case 2:
                std::cout << "Enter new Family Name: ";
                std::cin >> userFamilyName;
                updateFamilyName(userID, userFamilyName);
                break;
            case 3:
                std::cout << "Admin Status - ";
                input = getResponse();
                if (input == 'y' || input == 'Y') { isAdmin = 1; } 
                updateAdminStatus(userID, isAdmin);
                break;
            case 4:
                std::cout << "Induction Status - ";
                input = getResponse();
                if (input == 'y' || input == 'Y') { inducted = 1; } 
                updateInductionStatus(userID, inducted);
                break;
            case 5:
                return;
        }
    }
}


/*
    Check if userID exists in the database, and return associated name
*/
std::optional<User> logIn(const std::string& id) {
    sqlite3* db = nullptr;
    std::string givenName = "";

    if (sqlite3_open("database/robot_logger.db", &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return std::nullopt;
    }

    std::string username = getUserFromID(id, givenName);
    bool adminStatus = getAdminStatus(id);
    sqlite3_close(db);

    if (username.empty()) return std::nullopt;

    return User{id, givenName, username, adminStatus};
}

