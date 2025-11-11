#include "menuUtils.h"
#include "dbUtils.h"
#include "User.h"
#include "robotUtils.h"
#include "userUtils.h"
#include "checkOut.h"
#include "checkIn.h"
#include "dbUtils.h"
#include "barcodeUtils/barcodeScanner.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>


enum States{
    ST_Login,
    ST_Main,
    ST_CheckOut,
    ST_CheckIn,
    ST_AddRobot,
    ST_ModifyRobot,
    ST_AddUser,
    ST_ModifyUser,
    ST_Exit
};

void pressEnterToContinue() {
    std::cout << "\n Press ENTER to continue...";
    if (std::cin.peek() == '\n') { // if leftover newline
        std::cin.get();           // consume it
    }
    std::cin.get();               // wait for Enter
}


int main(int argc, char* argv[]) {

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR); //suppress opencv warnings
    
    bool camera = false;
    bool headless = false;

    // Parse flags
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-camera") camera = true;
        else if (arg == "-headless") headless = true;
    }

    States currState = ST_Login;

    if (!openDBConnection()) {
        return 1;
    }


    std::vector<std::string> robots = getRobots();
    std::optional<User> loggedInUser;
    std::string studentId; 


    // Menu Handler
    while (currState != ST_Exit) {

        while (currState == ST_Login) {
        printLogin();
        
        if(camera || headless){
             studentId = scanRobotBarcode(camera, headless);
        } else {
            printStartText();
            std::cin >> studentId;
        }
        

        if (studentId.empty()) {
            printStartText();
            std::cin >> studentId;
        }

        loggedInUser = logIn(studentId);

        if (loggedInUser.has_value()) {
            currState = ST_Main;
        } else {
            std::cout << " Invalid ID or User does not exist. Try again.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        }

        switch (currState) {

            case ST_Main: {
                int menuItem = 0;
                loggedInUser = logIn(studentId); // Refresh user info
                printMainMenu(loggedInUser->getFullname(), loggedInUser->isAdmin());

                if (loggedInUser->isAdmin()) {
                    menuItem = getIntInput(1, 7);
                    switch (menuItem) {
                        case 1: currState = ST_CheckOut; break;
                        case 2: currState = ST_CheckIn;  break;
                        case 3: currState = ST_AddRobot; break;
                        case 4: currState = ST_ModifyRobot;  break;
                        case 5: currState = ST_AddUser;  break;
                        case 6: currState = ST_ModifyUser;  break;
                        case 7: currState = ST_Login;     break;
                    }
                } else {
                    menuItem = getIntInput(1, 3);
                    switch (menuItem) {
                        case 1: currState = ST_CheckOut; break;
                        case 2: currState = ST_CheckIn;  break;
                        case 3: currState = ST_Login;     break;
                    }
                }
            
                break;
            }

            case ST_CheckOut: {
                robots = getRobots(); // Refresh robot list
                std::string pickedRobot;
                printCheckOutMenu(robots, pickedRobot);

                if(pickedRobot == "") {
                    pressEnterToContinue();
                    currState = ST_Main;
                    break;
                }

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

            case ST_ModifyRobot: {
                modifyRobot();
                currState = ST_Main;
                break;
            }

            case ST_AddUser: {
                addUser();
                currState = ST_Main;
                break;
            }

            case ST_ModifyUser: {
                modifyUser(loggedInUser->getID());
                currState = ST_Main;
                break;
            }

            case ST_Login: {
                loggedInUser.reset();
                currState = ST_Login;
                break;
            }

            default:
                currState = ST_Login;
                break;
        }
    }

    loggedInUser.reset();
    closeDBConnection();
    return 0;
}