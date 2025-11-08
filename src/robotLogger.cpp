#include "menuUtils.h"
#include "dbUtils.h"
#include "User.h"
#include "robotUtils.h"
#include "userUtils.h"
#include "checkOut.h"
#include "checkIn.h"

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <fstream>


enum States{
    ST_Main,
    ST_CheckOut,
    ST_CheckIn,
    ST_AddRobot,
    ST_AddUser,
    ST_Exit
};

void pressEnterToContinue() {
    std::cout << "\nPress ENTER to continue...";
    if (std::cin.peek() == '\n') { // if leftover newline
        std::cin.get();           // consume it
    }
    std::cin.get();               // wait for Enter
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
                int menuItem = 0;
                printMainMenu(loggedInUser->getFullname(), loggedInUser->isAdmin());

                if (loggedInUser->isAdmin()) {
                    menuItem = getIntInput(1, 5);
                    switch (menuItem) {
                        case 1: currState = ST_CheckOut; break;
                        case 2: currState = ST_CheckIn;  break;
                        case 3: currState = ST_AddRobot; break;
                        case 4: currState = ST_AddUser;  break;
                        case 5: currState = ST_Exit;     break;
                    }
                } else {
                    menuItem = getIntInput(1, 3);
                    switch (menuItem) {
                        case 1: currState = ST_CheckOut; break;
                        case 2: currState = ST_CheckIn;  break;
                        case 3: currState = ST_Exit;     break;
                    }
                }
            
                break;
            }

            case ST_CheckOut: {
                std::string pickedRobot;
                printCheckOutMenu(robots, pickedRobot);
                addCheckOutRecord(loggedInUser->getID(), pickedRobot);

                pressEnterToContinue();
                currState = ST_Main;
                break;
            }

            
            case ST_CheckIn: {
                std::string pickedRobot;
                std::string notes;
                std::vector<std::string> checkedOutRobots = getRobotsCurrentlyCheckedOut();
                printCheckInMenu(checkedOutRobots, pickedRobot, notes);
                
                if(pickedRobot == "") {
                    pressEnterToContinue();
                    currState = ST_Main;
                    break;
                }

                addCheckInRecord(loggedInUser->getID(), pickedRobot, notes);
                pressEnterToContinue();
                
                currState = ST_Main;
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