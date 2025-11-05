#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <iostream>
#include <string>
#include <vector>

// Function declarations
int getIntInput(int min, int max);

void printStartText();
void printMainMenu(const std::string& id);
void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot);
void printCheckInMenu();
void printExitMessage();
void printRobotType();
void printRobotCondition();

#endif // MENUUTILS_H