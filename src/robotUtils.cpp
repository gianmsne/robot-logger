#include "robotUtils.h"
#include "menuUtils.h"
#include "dbUtils.h"
#include "inputValidation.h"

#include <sqlite3.h>

void addRobot() {

    int input;

    std::string robotName;
    std::string robotID;
    std::string location;
    std::string robotType;
    std::string robotCondition;
    std::string robotStatus;
    
    std::string tableName = "robots";
    std::string columnName = "robotName";
    
    std::cout << "\n ------------- ADD ROBOT -------------" << std::endl;

    // Get Robot Name
    std::cout << " >> Enter Robot Name: ";
    std::cin >> robotName;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(existenceCheck(tableName, columnName, robotName)) {
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
    if (location.empty()) { location = "RACE Hub"; }
    
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

    if(insertRobot(robotName, robotType, robotCondition, robotID, location)) {
        std::cout << " Robot added successfully!" << std::endl;
    } else {
        std::cout << " Error! Could not add robot." << std::endl;
    }

}

void modifyRobot() {
    std::string robotName;
    std::string robotType;
    std::string robotCondition;
    std::string robotID;
    std::string location;
    int isAvailable;

    char input;

    std::cout << std::endl;
    std::cout << " ------------- MODIFY ROBOT -------------" << std::endl;

    std::cout << " >> Enter robot name to modify: ";
    std::cin >> robotName;

    while(!existenceCheck("robots", "robotName", robotName)){
        std::cout << " Robot " + robotName + " does not exist. Try Again: ";
        std::cin >> robotName;
    };

    int choice = -1;
    while(choice != 7){
        std::cout << "\n Robot details for: " + robotName << std::endl;
        printModifyRobotMenu();

        choice = getIntInput(1,7);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        switch(choice){
            case 1:
                printRobotType();

                choice = getIntInput(1,2);
                robotType = getRobotType(choice);

                updateType(robotName, robotType);
                break;
            case 2:
                printRobotCondition();

                choice = getIntInput(1,5);
                robotCondition = getRobotCondition(choice);

                if(robotCondition == "Retired" || robotCondition == "Non-functional") {
                    std::cout << " Setting robot as unavailable due to condition." << std::endl;
                    updateAvailability(robotName, 0); // Set to unavailable
                } else {
                    updateAvailability(robotName, 1); // Set to available
                }

                updateCondition(robotName, robotCondition);
                break;
            case 3:
                std::cout << " Enter new ID: ";
                std::cin >> robotID;
                updateRobotID(robotName, robotID);
                break;
            case 4:
                std::cout << " Enter new location: ";
                std::getline (std::cin, location);
                updateLocation(robotName, location);
                break;
            case 5:
                std::cout << " Availability Status - ";
                input = getYesNo();
                if (input == 'y' || input == 'Y') { 
                    isAvailable = 1; 
                } else {
                    isAvailable = 0;
                } 
                updateAvailability(robotName, isAvailable);
                break;
            case 6:
                std::cout << " Are you sure you want to delete " + robotName + "?" << std::endl;
                input = getYesNo();
                if (input == 'y' || input == 'Y') { 
                    removeRobot(robotName);
                    choice = 7; // Exit after deletion
                } else {
                   return;
                } 
                break;
            case 7:
                break;
        }   
    }
}