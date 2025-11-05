#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>

int getIntInput(int min, int max) {
    int input;
    bool valid = false;
    
    while(!valid){
        try{
            std::cin >> input;
            if(input < min || input > max){
                throw (input);
            }

            valid = true;
            
        } catch(...){
            std::cout << "Please enter a number between " << min << " and " << max << ": ";
        }
    }
    
    return input;
}

void printStartText(void){
    std::cout << std::endl;
    std::cout << "Please scan your student ID" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void printMainMenu(std::string id){
    std::cout << std::endl;
    std::cout << "------------- User: " << id << " -------------" << std::endl;
    std::cout << "1) Check In" << std::endl;
    std::cout << "2) Check Out" << std::endl;
    std::cout << "3) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printCheckOutMenu(std::vector<std::string> robots, std::string& pickedRobot){
    std::cout << std::endl;
    std::cout << "------------- CHECK-IN -------------" << std::endl;

    for(unsigned int i = 0; i < robots.size(); i++){
        std::cout << i+1 << ") " << robots[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Enter robot item to continue: " << std::endl;
    int num;
    std::cin >> num;
    
    pickedRobot = robots[num-1];
}


void printCheckInMenu(void){
    std::cout << std::endl;
    std::cout << "------------- PICK ROBOT -------------" << std::endl;
    std::cout << "1) kessel" << std::endl;
    std::cout << "2) jupiter" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Pick robot number: " << std::endl;
}

void printExitMessage(void){
    std::cout << "Goodbye" << std::endl;
}

#endif