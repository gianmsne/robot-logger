#include "User.h"

// Constructor
User::User(std::string ID, std::string givenName, std::string fullname, bool adminStatus){
    this->ID = ID;
    this->givenName = givenName;
    this->fullname = fullname;
    this->adminStatus = adminStatus;
}

std::string User::getID(){
    return ID;
}
        
std::string User::getFullname(){
    return fullname;
}

std::string User::getGivenName(){
    return givenName;
}

bool User::isAdmin(){
    return adminStatus;
}