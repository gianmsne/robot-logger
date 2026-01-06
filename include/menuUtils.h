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
void printCheckOutMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment);
void printCheckInMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment, std::string &notes, std::string &permStatus);
void printEquipmentType();
void printEquipmentCondition();
void printModifyEquipmentMenu();
void printModifyUserMenu();
std::string getEquipmentType(int typeChoice);
std::string getEquipmentCondition(int conditionChoice);
void printNotesMenu(const std::vector<std::string>& equipments, std::string& pickedEquipment);
void printNotes(const std::string& equipmentName, const std::string& userID);



#endif // MENUUTILS_H