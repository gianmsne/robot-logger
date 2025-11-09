#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>
#include <string>
#include <vector>

// Function declarations
int getIntInput(int min, int max);

void printStartText();
void printMainMenu(const std::string& id, bool isAdmin);
void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot);
void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes);
void printRobotType();
void printRobotCondition();
void printModifyMenu();

#endif // MENUUTILS_H