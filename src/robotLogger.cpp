#include "menuUtils.h"
#include "dbUtils.h"
#include "User.h"
#include "robotUtils.h"
#include "userUtils.h"

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <fstream>


enum States{
    ST_Main,
    ST_CheckIn,
    ST_CheckOut,
    ST_AddRobot,
    ST_AddUser,
    ST_Exit
};

std::vector<std::string> getRobots(){
    std::vector<std::string> robotList;
    std::string robot;
    
    std::ifstream robots("robots.txt");

    while(std::getline (robots, robot) ) {
        robotList.push_back(robot);
    }

    robots.close();

    return robotList;
}

int main() {
    States currState = ST_Main;
    std::vector<std::string> robots = getRobots();


    std::optional<User> loggedInUser;
    std::string studentId;

    while (true) {
        printStartText();
        std::cin >> studentId;

        loggedInUser = logIn(studentId);

        if (loggedInUser.has_value())
            break;

        std::cout << "Invalid ID or User does not exist. Try again.\n";
    }


    // Menu Handler
    while (currState != ST_Exit) {

        switch (currState) {

            case ST_Main: {
                printMainMenu(loggedInUser->getFullname());

                int menuItem = getIntInput(1, 5);
                switch (menuItem) {
                    case 1: 
                        currState = ST_CheckOut; 
                        break;
                    case 2: 
                        currState = ST_CheckIn; 
                        break;
                    case 3: 
                        currState = ST_AddRobot;
                        break;
                    case 4:
                        currState = ST_AddUser;
                        break;
                    case 5:
                        currState = ST_Exit; 
                        break;
                }
                break;
            }

            case ST_CheckOut: {
                std::string pickedRobot;
                printCheckOutMenu(robots, pickedRobot);
                
                std::cout << "PICKED ROBOT: " << pickedRobot << std::endl;

                currState = ST_Main;
            }

            
            case ST_CheckIn: {
                printCheckInMenu();
                int menuItem = getIntInput(1, 3);
                switch (menuItem) {
                    case 1: 
                        // Handle checkout option 1
                        break;
                    case 2: 
                        // Handle checkout option 2
                        break;
                    case 3: 
                        currState = ST_Main; 
                        break;
                }
                break;
            }

            case ST_AddRobot: {
                addRobot();
                currState = ST_Main;
                break;
            }

            case ST_AddUser: {
                addUser();
                break;
            }

            default:
                currState = ST_Exit;
                break;
        }
    }

    loggedInUser.reset();
    return 0;
}