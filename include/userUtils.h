#ifndef ADDUSER_H
#define ADDUSER_H

#include <iostream>
#include "User.h"

void addUser();
void modifyUser();
std::optional<User> logIn(const std::string& id);

#endif // ADDUSER_H