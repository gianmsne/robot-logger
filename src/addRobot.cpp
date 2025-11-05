#include "addRobot.h"
#include "menuUtils.h"

void menuSequence() {

    std::string robotName;
    std::string robotID;
    std::string location;
    std::string robotType;
    std::string robotCondition;
    std::string robotStatus;

    int input;

    std::cout << std::endl;
    std::cout << "------------- ADD ROBOT -------------" << std::endl;

    std::cout << " >> Enter Robot Name: ";
    std::cin >> robotName;

    std::cout << " >> Enter Robot RMIT ID ( [Enter] for none ): ";
    std::cin >> robotID;

    std::cout << " >> Enter Location: ";
    std::cin >> location;
    
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

    try {
        printf(
        "Robot Info:\n"
        "Name: %s\n"
        "ID: %s\n"
        "Location: %s\n"
        "Type: %s\n"
        "Condition: %s\n",
        // "Status: %s\n"
        // "Last Used Person: %s\n"
        // "Last Used Time: %s\n"
        // "Notes: %s\n"
        // "Available: %s\n",
        robotName.c_str(),
        robotID.c_str(),
        location.c_str(),
        robotType.c_str(),
        robotCondition.c_str(),
        robotStatus.c_str()
        );

    } catch (...) {
        std::cout << "There was an error while attempting to add the robot." << std::endl;
        std::cout << "Please try again" << std::endl;
    }


}