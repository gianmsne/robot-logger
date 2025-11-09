#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>
#include <string>
#include <vector>

// Function declarations
int getIntInput(int min, int max);
char getResponse();

void printStartText();
void printMainMenu(const std::string& id, bool isAdmin);
void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot);
void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes);
void printRobotType();
std::string getRobotType(int typeChoice);
void printRobotCondition();
std::string getRobotCondition(int conditionChoice);
void printModifyRobotMenu();
void printModifyUserMenu();

#endif // MENUUTILS_H