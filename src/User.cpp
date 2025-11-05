#include "User.h"

// Constructor
User::User(std::string ID, std::string fullname){
    this->ID = ID;
    this->fullname = fullname;
}

std::string User::getID(){
    return ID;
}
        
std::string User::getFullname(){
    return fullname;
}