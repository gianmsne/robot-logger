#include "menuUtils.h"

int getIntInput(int min, int max) {
    int input;
    bool valid = false;
    
    while (!valid) {
        std::cin >> input;
        if(std::cin.fail() || input < min || input > max){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " Please enter a number between " << min << " and " << max << ": ";
        } else { 
            valid = true;
        }
    }
    
    return input;
}

char getResponse() {
    std::string input;
    char inputChar;

    while (true) {
        std::cout << " Select (y/n): ";
        std::getline(std::cin, input); // getting entire line here to avoid repeating error messages

        if (input.empty()) continue;  // user pressed Enter

        inputChar = toupper(input[0]);  // take first character only

        if (inputChar == 'Y' || inputChar == 'N')
            return inputChar;

        std::cout << " Invalid input. Please enter y or n.\n";
    }
}

void printStartText() {
    std::cout << std::endl;
    std::cout << "  >> Scan Your User ID Card" << std::endl;
}

void printMainMenu(const std::string& id, bool isAdmin) {
    std::cout << std::endl;
    std::cout << " ------------- User: " << id << " -------------" << std::endl;
    std::cout << " 1) Check Out" << std::endl;
    std::cout << " 2) Check In" << std::endl;
    if(isAdmin){
        std::cout << "\n >> Robot Options: " << std::endl;
        std::cout << " 3) Add Robot" << std::endl;
        std::cout << " 4) Modify Robot" << std::endl;
        std::cout << "\n >> User Options: "<< std::endl;
        std::cout << " 5) Add User" << std::endl;
        std::cout << " 6) Modify User" << std::endl;
        std::cout << std::endl;
        std::cout << " 7) Logout" << std::endl;
    } else {
        std::cout << " 3) Exit" << std::endl;
    }
    std::cout << std::endl;
    std::cout << " Enter Menu item to continue: ";
}

void printCheckOutMenu(const std::vector<std::string>& robots, std::string& pickedRobot) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-OUT -------------" << std::endl;

    for (unsigned int i = 0; i < robots.size(); i++) {
        std::cout << " " << i + 1 << ") " << robots[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << " Enter robot item to continue: ";
    int num;
    std::cin >> num;
    
    if(num >= 1 && num <= (int)robots.size()) {
        pickedRobot = robots[num - 1];
    } else {
        pickedRobot = "";
    }
}

void printCheckInMenu(const std::vector<std::string>& robots, std::string& pickedRobot, std::string &notes) {
    std::cout << std::endl;
    std::cout << " ------------- CHECK-IN -------------" << std::endl;

    if(robots.empty()) {
        std::cout << "  >> There are no available robots to check in." <<  std::endl;
        pickedRobot = "";
        return;
    } else {
        for (unsigned int i = 0; i < robots.size(); i++) {
            std::cout << " " << i + 1 << ") " << robots[i] << std::endl;
        }
    }

    std::cout << std::endl;
    std::cout << " Enter robot item to continue: ";
    int num;
    std::cin >> num;
    
    if(num >= 1 && num <= (int)robots.size()) {
        pickedRobot = robots[num - 1];
    } else {
        pickedRobot = "";
    }
    
    std::cout << std::endl;
    std::cout << " Enter any notes about the robot's condition: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::getline(std::cin, notes);
    std::cin.clear();
}

void printRobotType() {
    std::cout << " >> Choose Robot Type: " << std::endl;
    std::cout << "     1) Booster" << std::endl;
    std::cout << "     2) Nao" << std::endl;
    std::cout << " >> Select: ";
}

std::string getRobotType(int typeChoice) {
    switch(typeChoice){
        case 1:
            return "booster";
        case 2:
            return "nao";
        default:
            return "unknown";
    }
}

void printRobotCondition() {
    std::cout << " >> Choose Robot Condition: " << std::endl;
    std::cout << "     1) Functional" << std::endl;
    std::cout << "     2) Minor Issue" << std::endl;
    std::cout << "     3) Needs Checking" << std::endl;
    std::cout << "     4) Non-functional" << std::endl;
    std::cout << "     5) Retired" << std::endl;
    std::cout << " >> Select: ";
}

std::string getRobotCondition(int conditionChoice) {
    switch(conditionChoice){
        case 1:
            return "Functional";
        case 2:
            return "Minor Issue";
        case 3:
            return "Needs Checking";
        case 4:
            return "Non-functional";
        case 5:
            return "Retired";
        default:
            return "Unknown";
    }
}

void printModifyUserMenu() {
    std::cout << " >> Choose user detail to modify: " << std::endl;
    std::cout << "     1) Given Name" << std::endl;
    std::cout << "     2) Family Name" << std::endl;
    std::cout << "     3) Admin Status" << std::endl;
    std::cout << "     4) Induction Status" << std::endl;
    std::cout << "     5) Delete User" << std::endl;
    std::cout << "     6) Done" << std::endl;
    std::cout << " >> Select: ";
}

void printModifyRobotMenu() {
    std::cout << " >> Choose robot detail to modify: " << std::endl;
    std::cout << "     1) Type" << std::endl;
    std::cout << "     2) Condition" << std::endl;
    std::cout << "     3) ID" << std::endl;
    std::cout << "     4) Location" << std::endl;
    std::cout << "     5) Availability" << std::endl;
    std::cout << "     6) Delete Robot" << std::endl;
    std::cout << "     7) Done" << std::endl;
    std::cout << " >> Select: ";
}

void printLogin(){

    for(int i = 0; i < 30; i++) {
        std::cout << std::endl;
    }

    std::cout << R"(
                                                                                                    
                                                                                                    
                                            @@@@@@@@@@@@                                            
                                       @@@@%%%%%%%%%%%%%@@@@@@                                      
                                  @@@@@==##%%###########%%%#++@@@                                   
                              @@@@%%%%##*+===++********++====*#%%@@@@                               
                           @@@%%%################*****#############%%@@@                            
                       @@@@%%%%#######################################%%@@@@                        
                      @@%%%%%###############*************##############%%%%@@@                      
                   @@@%%%%%%#############*******************############%%%%%%@@                    
            @@@@@@@- :%%%%%%############********************#############%%%%%%@@@                  
         @@%         -%%%%%%#############*******************#############%%%%%%.  +@@@@             
        @.            @%%%%%%##############***************##############%%%%%%%        :@@@         
       @@             :@%%%%%%####################*###################%%%%%%%%=           @@@       
       @%              .@%%%%%%%%###################################%%%%%%%%%#              @@      
       @                 #%%%%%%%%%%#############@%@#############%%%%%%%%%%%@               @@      
       @                  .@%%%%%%%%%%%%%%######@%@%@######%%%%%%%%%%%%%%%%*                *@      
      @@                     %@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@*                  +@      
      @@                        :@@@%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%                      -@      
      @@                                                                                    .@      
      @@    .......    ....... ..........     ... ..      .. .       .. .       .  .       . @      
      @@.....................======+=............................:-:..................... ...@      
      @*...................==========+:.......................-+=======.....................:@      
      @-..................=====@@@@====......................============...................=@      
      @...................===%@@@@@@*=+.....................:===@@@@@@===:..................#@      
      @...................===#@@@@@@+=+.....................===@@@@@@@*==-..................@@      
      @...................:+===@@@@==+=.....................-===@@@@@@===:..................@@      
      @-...................:=+=====+=:.......................============...................@@      
      @*......................:---:...........................:========:....................@@      
      @@:::::::::::::::::::::::::::::::::::::::::::::.::......:::::::.......................@       
      @@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::-@       
      @@:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::#@       
       @:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::@@       
       @@----------------------------------------@%-----::::----:::::::::::::::::::::::::::@@       
       @@--------------------------------------%%%@@+--------------------------------------@@       
        @*-------------------------------------=+++%---------------------------------------@        
        @@+=============================--------------------------------------------------@@        
          @@@@@@@@=======================================================================@@         
                  @@@@@@=========================================================@@@@@@@@           
                        @@@@#+++++++++++++++++++++++++++++++++++++++++++++*@@@@@                    
                            @@@++++++++++++++++++++++++++++++++++++++*@@@@                          
                               @@@******+++++*++++++++++++++++++++@@@@                              
                                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                                                                  
                                                                                                    
                                                                                                    
)" << std::endl;

    // Print heading
    std::cout << "                             ------------- Robot Logger -------------\n" << std::endl;
}