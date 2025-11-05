#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

class User {

    private:
        std::string ID;
        std::string fullname;
        std::vector<std::string> robotList;

    public:
        User(std::string ID, std::string fullname);
        std::string getID();
        std::string getFullname();

};

#endif // USER.H