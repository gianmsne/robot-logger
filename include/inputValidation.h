#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include <iostream>
#include <limits>
#include <string>

bool vector_contains(const std::vector<std::string>& vec, const std::string& value);
bool is_number(const std::string& s);
int getIntInput(int min, int max);
std::string stringToLower(std::string str);
char getYesNo();

#endif // INPUT_VALIDATION_H