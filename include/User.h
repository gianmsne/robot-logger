#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

class User {

    private:
        std::string ID;
        std::string fullname;
        bool adminStatus;

    public:
        User(std::string ID, std::string fullname, bool adminStatus);
        std::string getID();
        std::string getFullname();
        bool isAdmin();
};

#endif // USER.H