#include "robotUtils.h"
#include "menuUtils.h"
#include "dbUtils.h"

#include <sqlite3.h>

void addRobot() {

    int input;

    std::string robotName;
    std::string robotID;
    std::string location;
    std::string robotType;
    std::string robotCondition;
    std::string robotStatus;
    
    sqlite3* db;
    std::string tableName = "robots";
    std::string columnName = "robotName";

    try {

        if(sqlite3_open("database/robot_logger.db", &db)) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            throw(0);
        }
    
    } catch (...) {
        std::cout << "There was an error while attempting to add the robot." << std::endl;
    }
    
    std::cout << "\n------------- ADD ROBOT -------------" << std::endl;

    // Get Robot Name
    std::cout << " >> Enter Robot Name: ";
    std::cin >> robotName;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(existenceCheck(db, tableName, columnName, robotName)) {
        std::cout << " >> Robot " + robotName + " already exists!" << std::endl;
        return;
    }
    
    // Get Robot ID
    std::cout << " >> Enter Robot RMIT ID (Optional): ";
    std::getline(std::cin, robotID);
    if (robotID.empty()) { robotID = "None"; }

    // Get robot location
    std::cout << " >> Enter Location ( [Enter] for \"RACE Hub\" ): ";
    std::getline(std::cin, location);
    if (location.empty()) { location = "Race Hub"; }
    
    // Get Robot Type (Nao/Booster)
    printRobotType();
    input = getIntInput(1, 2);
    switch(input){
        case 1:
            robotType = "booster";
            break;
        case 2:
            robotType = "nao";
            break;
        default:
            robotType = "unknown";
            break;
    }

    // Get Robot Condition
    printRobotCondition();
    input = getIntInput(1, 5);

    switch(input){
        case 1:
            robotCondition = "Perfect";
            break;
        case 2:
            robotCondition = "Slight Damage";
            break;
        case 3:
            robotCondition = "Partially Functional";
            break;
        case 4:
            robotCondition = "Needs Repair";
            break;
        case 5:
            robotCondition = "Retired";
            break;
        default:
            robotCondition = "Unknown"; // fallback value
            break;
    }
    std::cout << std::endl;

    if(insertRobot(db, robotName, robotType, robotCondition, robotID, location)) {
        std::cout << "Robot added successfully!" << std::endl;
    } else {
        std::cout << "Error! Could not add robot." << std::endl;
    }

    sqlite3_close(db);

}