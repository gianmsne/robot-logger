#ifndef ADDUSER_H
#define ADDUSER_H

#include "User.h"
#include <iostream>
#include <optional>

void addUser();
void modifyUser(const std::string loggedInUserID);
std::optional<User> logIn(const std::string& id);

#endif // ADDUSER_H