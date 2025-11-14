#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <limits>

void printStartText();
void printStartTextScan();
void printLogin();
void printMainMenu(const std::string& id, bool isAdmin);
void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot);
void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes, std::string &permStatus);
void printRobotType();
void printRobotCondition();
void printModifyRobotMenu();
void printModifyUserMenu();
std::string getRobotType(int typeChoice);
std::string getRobotCondition(int conditionChoice);



#endif // MENUUTILS_H