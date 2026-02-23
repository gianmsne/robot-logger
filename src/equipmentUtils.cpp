#include "equipmentUtils.h"
#include "menuUtils.h"
#include "dbUtils.h"
#include "inputValidation.h"

#include <sqlite3.h>

void addEquipment() {

    int input;

    std::string equipmentName;
    std::string location;
    std::string equipmentType;
    std::string equipmentCondition;
    std::string equipmentStatus;
    
    std::string tableName = "equipment";
    std::string columnName = "equipmentName";
    
    std::cout << "\n ------------- ADD EQUIPMENT -------------" << std::endl;

    // Get Equipment Name
    std::cout << " >> Enter Equipment Name: ";
    std::cin >> equipmentName;
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(existenceCheck(tableName, columnName, equipmentName)) {
        std::cout << " >> Equipment " + equipmentName + " already exists!" << std::endl;
        return;
    }

    // Get equipment location
    std::cout << " >> Enter Location ( [Enter] for \"RACE Hub\" ): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, location);
    if (location.empty()) { location = "RACE Hub"; }
    
    // Get Equipment Type (Nao/Booster)
    printEquipmentType();
    input = getIntInput(1, 2);
    switch(input){
        case 1:
            equipmentType = "booster";
            break;
        case 2:
            equipmentType = "nao";
            break;
        default:
            equipmentType = "unknown";
            break;
    }

    // Get Equipment Condition
    printEquipmentCondition();
    input = getIntInput(1, 5);

    switch(input){
        case 1:
            equipmentCondition = "Perfect";
            break;
        case 2:
            equipmentCondition = "Slight Damage";
            break;
        case 3:
            equipmentCondition = "Partially Functional";
            break;
        case 4:
            equipmentCondition = "Needs Repair";
            break;
        case 5:
            equipmentCondition = "Retired";
            break;
        default:
            equipmentCondition = "Unknown"; // fallback value
            break;
    }
    std::cout << std::endl;

    if(insertEquipment(equipmentName, equipmentType, equipmentCondition, location)) {
        std::cout << " Equipment added successfully!" << std::endl;
    } else {
        std::cout << " Error! Could not add equipment." << std::endl;
    }

}

void modifyEquipment() {
    std::string equipmentName;
    std::string equipmentType;
    std::string equipmentCondition;
    std::string location;
    int isAvailable;

    char input;

    std::cout << std::endl;
    std::cout << " ------------- MODIFY EQUIPMENT -------------" << std::endl;

    std::cout << " >> Enter equipment name to modify: ";
    std::cin >> equipmentName;

    while(!existenceCheck("equipment", "equipmentName", equipmentName)){
        std::cout << " Equipment " + equipmentName + " does not exist. Try Again: ";
        std::cin >> equipmentName;
    };

    int choice = -1;
    while(choice != 6){
        std::cout << "\n Equipment details for: " + equipmentName << std::endl;
        printModifyEquipmentMenu();

        choice = getIntInput(1,6);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        switch(choice){
            case 1:
                printEquipmentType();

                choice = getIntInput(1,2);
                equipmentType = getEquipmentType(choice);

                updateType(equipmentName, equipmentType);
                break;
            case 2:
                printEquipmentCondition();

                choice = getIntInput(1,5);
                equipmentCondition = getEquipmentCondition(choice);

                if(equipmentCondition == "Retired" || equipmentCondition == "Non-functional") {
                    std::cout << " Setting equipment as unavailable due to condition." << std::endl;
                    updateAvailability(equipmentName, 0); // Set to unavailable
                } else {
                    updateAvailability(equipmentName, 1); // Set to available
                }

                updateCondition(equipmentName, equipmentCondition);
                break;
            case 3:
                std::cout << " Enter new location: ";
                std::getline (std::cin, location);
                updateLocation(equipmentName, location);
                break;
            case 4:
                std::cout << " Availability Status - ";
                input = getYesNo();
                if (input == 'y' || input == 'Y') { 
                    isAvailable = 1; 
                } else {
                    isAvailable = 0;
                } 
                updateAvailability(equipmentName, isAvailable);
                break;
            case 5:
                std::cout << " Are you sure you want to delete " + equipmentName + "?" << std::endl;
                input = getYesNo();
                if (input == 'y' || input == 'Y') { 
                    removeEquipment(equipmentName);
                    choice = 7; // Exit after deletion
                } else {
                   return;
                } 
                break;
            case 6:
                break;
        }   
    }
}