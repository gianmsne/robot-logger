#include "menuUtils.h"

int getIntInput(int min, int max) {
    int input;
    bool valid = false;
    
    while (!valid) {
        std::cin >> input;
        if(std::cin.fail() || input < min || input > max){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number between " << min << " and " << max << ": ";
        } else { 
            valid = true;
        }
    }
    
    return input;
}

void printStartText() {
    std::cout << std::endl;
    std::cout << " >> Tap Your User ID Card (or enter manually): s";
}

void printMainMenu(const std::string& id, bool isAdmin) {
    std::cout << std::endl;
    std::cout << "------------- User: " << id << " -------------" << std::endl;
    std::cout << "1) Check Out" << std::endl;
    std::cout << "2) Check In" << std::endl;
    if(isAdmin){
        std::cout << "3) Add Robot" << std::endl;
        std::cout << "4) Add User" << std::endl;
        std::cout << "5) Exit" << std::endl;
    } else {
        std::cout << "3) Exit" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: ";
}

void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot) {
    std::cout << std::endl;
    std::cout << "------------- CHECK-OUT -------------" << std::endl;

    for (unsigned int i = 0; i < robots.size(); i++) {
        std::cout << i + 1 << ") " << robots[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Enter robot item to continue: ";
    int num;
    std::cin >> num;
    
    if(num >= 1 && num <= (int)robots.size()) {
        pickedRobot = robots[num - 1];
    } else {
        pickedRobot = "";
    }
}

void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes) {
    std::cout << std::endl;
    std::cout << "------------- CHECK-IN -------------" << std::endl;

    if(robots.empty()) {
        std::cout << " >> There are no available robots to check in." <<  std::endl;
        pickedRobot = "";
        return;
    } else {
        for (unsigned int i = 0; i < robots.size(); i++) {
            std::cout << i + 1 << ") " << robots[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << "Enter robot item to continue: ";
    int num;
    std::cin >> num;
    
    if(num >= 1 && num <= (int)robots.size()) {
        pickedRobot = robots[num - 1];
    } else {
        pickedRobot = "";
    }
    
    std::cout << std::endl;
    std::cout << "Enter any notes about the robot's condition: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::getline(std::cin, notes);
    std::cin.clear();
}

void printRobotType() {
    std::cout << ">> Choose Robot Type: " << std::endl;
    std::cout << "     1) Booster" << std::endl;
    std::cout << "     2) Nao" << std::endl;
    std::cout << ">> Select: ";
}

void printRobotCondition() {
    std::cout << ">> Choose Robot Condition: " << std::endl;
    std::cout << "     1) Functional" << std::endl;
    std::cout << "     2) Minor Issue" << std::endl;
    std::cout << "     3) Needs Checking" << std::endl;
    std::cout << "     4) Non-functional" << std::endl;
    std::cout << "     5) Retired" << std::endl;
    std::cout << ">> Select: ";
}