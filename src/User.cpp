#include "User.h"

// Constructor
User::User(std::string ID, std::string fullname, bool adminStatus){
    this->ID = ID;
    this->fullname = fullname;
    this->adminStatus = adminStatus;
}

std::string User::getID(){
    return ID;
}
        
std::string User::getFullname(){
    return fullname;
}

bool User::isAdmin(){
    return adminStatus;
}